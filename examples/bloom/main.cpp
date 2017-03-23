#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\SpriteBuffer.h"
#include "..\common\imgui.h"

// Name        Thresh  Bloom  Base  BloomSat BaseSat
// Default     0.25f   1.25f   1      1        1
// Soft        0       1       1      1        1
// Desaturated 0.5     2       1      0        1
// Saturated   0.25    2       1      2        0
// Blurry      0       1       0.1    1        1
// Subtle      0.5     1       1      1        1

const static float KERNELS[16] = { -6.0f, -5.0f, -4.0f,	-3.0f, -2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 0.0f, 0.0f, 0.0f };
const static float WEIGHTS[16] = { 0.002216f, 0.008764f, 0.026995f, 0.064759f, 0.120985f, 0.176033f, 0.199471f, 0.176033f, 0.120985f, 0.064759f, 0.026995f, 0.008764f, 0.002216f, 0.0f, 0.0f, 0.0f };

struct BlurBuffer {
	ds::matrix kernels;
	ds::matrix weights;
	ds::vec2 direction;
	ds::vec2 data;
};

struct BloomExtractBuffer {
	float BloomThreshold;
	float padding1;
	float padding2;
	float padding3;
};

struct BloomBuffer {
	float BloomSaturation;
	float BloomIntensity;
	float OriginalSaturation;
	float OriginalIntensity;
};

// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);
	return textureID;
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Bloom demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);

	//
	// create two render targets to switch between
	//
	RID rt1 = ds::createRenderTarget(1024, 768, ds::Color(0, 0, 0, 0));
	RID rt2 = ds::createRenderTarget(1024, 768, ds::Color(0, 0, 0, 0));
	RID rt3 = ds::createRenderTarget(1024, 768, ds::Color(0, 0, 0, 0));
	//
	// otho pass simply draws a full screen quad using the loaded image and writes to RT1
	//
	RID targets[] = { rt1 };
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(ds::getScreenWidth(), ds::getScreenHeight(), 0.1f, 1.0f);
	RID orthoPass = ds::createRenderPass(orthoView, orthoProjection, ds::DepthBufferState::DISABLED, targets, 1);

	//
	// basic pass 
	//
	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 0.0f, -1.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);

	// render pass using back buffer
	RID ppPass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::DISABLED);

	// render pass using RT1
	RID rt1s[] = { rt1 };
	RID rt1Pass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::DISABLED, rt1s, 1);

	// render pass using RT2
	RID rt2s[] = { rt2 };
	RID rt2Pass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::DISABLED, rt2s, 1);

	// render pass using RT3
	RID rt3s[] = { rt3 };
	RID rt3Pass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::DISABLED, rt3s, 1);

	RID bgTextureID = loadImage("martian_oasis_by_smnbrnr.png");

	RID fsVertexShader = ds::loadVertexShader("Fullscreen_vs.cso");
	RID fsPixelShader = ds::loadPixelShader("Fullscreen_ps.cso");
	RID blurVSShader = ds::loadVertexShader("Blur_vs.cso");
	RID blurPSShader = ds::loadPixelShader("BlurH_ps.cso");
	RID bloomPSShader = ds::loadPixelShader("Bloom_Combine_ps.cso");
	RID bloomExtractPS = ds::loadPixelShader("Bloom_ps.cso");
	//
	// the blur buffer 
	//
	BlurBuffer blurBuffer;
	for (int i = 0; i < 13; ++i) {
		blurBuffer.kernels[i] = KERNELS[i];
		blurBuffer.weights[i] = WEIGHTS[i];
	}
	blurBuffer.direction = ds::vec2(1, 0);
	blurBuffer.data = ds::vec2(1024.0f, 768.0f);
	RID blurBufferID = ds::createConstantBuffer(sizeof(BlurBuffer), &blurBuffer);
	//
	// The bloom settings buffer
	BloomBuffer bloomBuffer;
	bloomBuffer.BloomIntensity = 1.25f;
	bloomBuffer.BloomSaturation = 1.0f;
	bloomBuffer.OriginalIntensity = 1.0f;
	bloomBuffer.OriginalSaturation = 1.0f;

	RID bloomBufferID = ds::createConstantBuffer(sizeof(BloomBuffer), &bloomBuffer);

	//
	// the bloom extract buffer
	//
	BloomExtractBuffer bloomExtractBuffer;
	bloomExtractBuffer.BloomThreshold = 0.25f;
	bloomExtractBuffer.padding1 = 0.0f;
	bloomExtractBuffer.padding2 = 0.0f;
	bloomExtractBuffer.padding3 = 0.0f;

	RID bloomExtractBufferID = ds::createConstantBuffer(sizeof(BloomExtractBuffer), &bloomExtractBuffer);

	//
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	//
	// the full screen draw command
	//
	ds::DrawCommand fsCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };

	// 
	// Basic full screen state group
	//
	RID basicFSGroup = ds::StateGroupBuilder()
		.inputLayout(NO_RID)
		.indexBuffer(NO_RID)
		.vertexBuffer(NO_RID)
		.geometryShader(NO_RID)
		.build();

	// 
	// the state group to draw the background image
	//
	RID backGroup = ds::StateGroupBuilder()
		.texture(bgTextureID, fsPixelShader, 0)
		.vertexShader(fsVertexShader)
		.pixelShader(fsPixelShader)
		.samplerState(ssid, fsPixelShader)
		.build();

	RID backGroups[] = { backGroup, basicFSGroup };
	RID backItem = ds::compile(fsCmd, backGroups, 2);

	// 
	// the state group for bloom extract
	//
	RID extractGroup = ds::StateGroupBuilder()
		.textureFromRenderTarget(rt1, bloomExtractPS, 0)
		.vertexShader(fsVertexShader)
		.pixelShader(bloomExtractPS)
		.samplerState(ssid, bloomExtractPS)
		.constantBuffer(bloomExtractBufferID, bloomExtractPS, 0)
		.build();

	RID extractGroups[] = { extractGroup, basicFSGroup };
	RID extractItem = ds::compile(fsCmd, extractGroups, 2);

	// 
	// the state group for the horizontal blur
	//
	RID blurHGroup = ds::StateGroupBuilder()		
		.vertexShader(blurVSShader)
		.pixelShader(blurPSShader)
		.textureFromRenderTarget(rt2, blurPSShader, 0)
		.samplerState(ssid, blurPSShader)
		.constantBuffer(blurBufferID, blurPSShader, 0)
		.build();
	RID blurHGroups[] = { blurHGroup, basicFSGroup };
	RID blurrHItem = ds::compile(fsCmd, blurHGroups, 2);

	// 
	// the state group for the vertical blur
	//
	RID blurVGroup = ds::StateGroupBuilder()
		.textureFromRenderTarget(rt3, blurPSShader, 0)
		.vertexShader(blurVSShader)
		.pixelShader(blurPSShader)
		.samplerState(ssid, blurPSShader)
		.constantBuffer(blurBufferID, blurPSShader, 0)
		.build();

	RID blurVGroups[] = { blurVGroup, basicFSGroup };
	RID blurrVItem = ds::compile(fsCmd, blurVGroups, 2);

	// 
	// the state group to get the final render target on screen
	//
	RID bloomCombineGroup = ds::StateGroupBuilder()
		.textureFromRenderTarget(rt1, bloomPSShader, 0)
		.textureFromRenderTarget(rt2, bloomPSShader, 1)
		.vertexShader(fsVertexShader)
		.pixelShader(bloomPSShader)
		.constantBuffer(bloomBufferID, bloomPSShader, 1)
		.samplerState(ssid, bloomPSShader)
		.build();
	
	RID bloomCombineGroups[] = { bloomCombineGroup, basicFSGroup };
	RID bloomCombineItem = ds::compile(fsCmd, bloomCombineGroups, 2);

	// 
	// debug state
	//
	RID debugGroup = ds::StateGroupBuilder()
		.vertexShader(fsVertexShader)
		.pixelShader(fsPixelShader)
		.textureFromRenderTarget(rt1, fsPixelShader, 0)
		.samplerState(ssid, fsPixelShader)
		.build();

	RID debugGroups[] = { debugGroup, basicFSGroup };
	RID debugItem = ds::compile(fsCmd, debugGroups, 2);

	// prepare IMGUI
	RID guiTextureID = loadImage("..\\common\\imgui.png");
	SpriteBuffer spriteBuffer(512, guiTextureID);
	gui::init(&spriteBuffer, guiTextureID);

	int state = 1;

	bool useBloom = true;

	while (ds::isRunning()) {

		ds::begin();
		// render image -> rt1
		ds::submit(orthoPass, backItem);
		if (useBloom) {
			// extract bloom -> rt2
			ds::submit(rt2Pass, extractItem);
			// render full screen quad using RT as texture
			blurBuffer.direction = ds::vec2(1, 0);
			// rt3
			ds::submit(rt3Pass, blurrHItem);
			blurBuffer.direction = ds::vec2(0, 1);
			// rt2
			ds::submit(rt2Pass, blurrVItem);
			// render full screen quad using RT as texture
			ds::submit(ppPass, bloomCombineItem);
		}
		else {
			ds::submit(ppPass, debugItem);
		}
		// GUI
		spriteBuffer.begin();
		gui::start(ds::vec2(0, 750));
		gui::begin("Bloom Settings", &state);
		if (state == 1) {
			gui::Input("Threshold", &bloomExtractBuffer.BloomThreshold);
			gui::Input("Bloom Intensity", &bloomBuffer.BloomIntensity);
			gui::Input("Bloom Saturation", &bloomBuffer.BloomSaturation);
			gui::Input("Original Intensity", &bloomBuffer.OriginalIntensity);
			gui::Input("Original Saturation", &bloomBuffer.OriginalSaturation);
			gui::Checkbox("Bloom", &useBloom);
		}
		gui::end();
		spriteBuffer.flush();

		ds::end();
	}
	ds::shutdown();
}