#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\SpriteBuffer.h"
#include "..\common\imgui.h"
#include "BloomComponent.h"

// Name        Thresh  Bloom  Base  BloomSat BaseSat
// Default     0.25f   1.25f   1      1        1
// Soft        0       1       1      1        1
// Desaturated 0.5     2       1      0        1
// Saturated   0.25    2       1      2        0
// Blurry      0       1       0.1    1        1
// Subtle      0.5     1       1      1        1
// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(texInfo);
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
	// create three render targets to switch between
	//
	RID rt1 = ds::createRenderTarget(1024, 768, ds::Color(0, 0, 0, 0));
	//
	// The bloom settings 
	//
	BloomSettings bloomSettings;
	bloomSettings.BloomIntensity = 1.25f;
	bloomSettings.BloomSaturation = 1.0f;
	bloomSettings.OriginalIntensity = 1.0f;
	bloomSettings.OriginalSaturation = 1.0f;
	//
	// the bloom extract settings
	//
	BloomExtractSettings bloomExtractSettings;
	bloomExtractSettings.BloomThreshold = 0.25f;
	bloomExtractSettings.padding1 = 0.0f;
	bloomExtractSettings.padding2 = 0.0f;
	bloomExtractSettings.padding3 = 0.0f;
	//
	// otho pass simply draws a full screen quad using the loaded image and writes to RT1
	//
	RID targets[] = { rt1 };
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(ds::getScreenWidth(), ds::getScreenHeight(), 0.1f, 1.0f);
	ds::Camera orthoCamera = {
		orthoView,
		orthoProjection,
		orthoView * orthoProjection,
		ds::vec3(0,3,-6),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	ds::RenderPassInfo rpInfo = { &orthoCamera, ds::DepthBufferState::DISABLED, targets, 1 };
	RID orthoPass = ds::createRenderPass(rpInfo);

	//
	// basic pass 
	//
	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 0.0f, -1.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	ds::Camera camera = {
		viewMatrix,
		projectionMatrix,
		viewMatrix * projectionMatrix,
		ds::vec3(0,0,-1),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	// render pass using back buffer
	ds::RenderPassInfo ppInfo = { &camera, ds::DepthBufferState::DISABLED, 0, 0 };
	RID ppPass = ds::createRenderPass(ppInfo);

	// render pass using RT1
	RID rt1s[] = { rt1 };
	ds::RenderPassInfo rt1Info = { &camera, ds::DepthBufferState::DISABLED, rt1s, 1 };
	RID rt1Pass = ds::createRenderPass(rt1Info);

	BloomComponent bloom(&camera, rt1, ppPass, &bloomSettings, &bloomExtractSettings);

	RID bgTextureID = loadImage("martian_oasis_by_smnbrnr.png");

	ds::ShaderInfo vsInfo = { "Fullscreen_vs.cso", 0, 0, ds::ShaderType::ST_VERTEX_SHADER };
	RID fsVertexShader = ds::createShader(vsInfo, "ParticlesVS");
	ds::ShaderInfo psInfo = { "Fullscreen_ps.cso", 0, 0, ds::ShaderType::ST_PIXEL_SHADER };
	RID fsPixelShader = ds::createShader(psInfo, "ParticlesPS");

	//
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);
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
			bloom.render();
		}
		else {
			ds::submit(ppPass, debugItem);
		}
		// GUI
		spriteBuffer.begin();
		gui::start(ds::vec2(0, 750));
		gui::begin("Bloom Settings", &state);
		if (state == 1) {
			gui::Value("FPS", ds::getFramesPerSecond());
			gui::Slider("Threshold", &bloomExtractSettings.BloomThreshold,0.0f,1.0f,2);
			gui::Input("Bloom Intensity", &bloomSettings.BloomIntensity);
			gui::Input("Bloom Saturation", &bloomSettings.BloomSaturation);
			gui::Input("Original Intensity", &bloomSettings.OriginalIntensity);
			gui::Slider("Original Saturation", &bloomSettings.OriginalSaturation,0.0f,2.0f,1);
			gui::Checkbox("Bloom", &useBloom);
		}
		gui::end();
		spriteBuffer.flush();

		ds::end();
	}
	ds::shutdown();
}