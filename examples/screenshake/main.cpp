#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "ScreenShakeComponent.h"
#include "Fullscreen_PS_Main.h"
#include "Fullscreen_VS_Main.h"
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Screenshake demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);
	//
	// create three render targets to switch between
	//
	RID rt1 = ds::createRenderTarget(1024, 768, ds::Color(0, 0, 0, 0));
	//
	// orthographic pass simply draws a full screen quad using the loaded image and writes to RT1
	//
	RID targets[] = { rt1 };
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(ds::getScreenWidth(), ds::getScreenHeight(), 0.1f, 1.0f);
	ds::Camera camera = {
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
	ds::RenderPassInfo orthoPassInfo = { &camera, ds::DepthBufferState::DISABLED, targets, 1 };
	RID orthoPass = ds::createRenderPass(orthoPassInfo);
	ds::RenderPassInfo ppPassInfo = { &camera, ds::DepthBufferState::DISABLED, 0, 0 };
	RID ppPass = ds::createRenderPass(ppPassInfo);

	RID bgTextureID = loadImage("martian_oasis_by_smnbrnr.png");

	ds::ShaderInfo vsInfo = { 0, Fullscreen_VS_Main, sizeof(Fullscreen_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID fsVertexShader = ds::createShader(vsInfo);
	ds::ShaderInfo psInfo = { 0, Fullscreen_PS_Main, sizeof(Fullscreen_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID fsPixelShader = ds::createShader(psInfo);

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

	ScreenShakeSettings settings;
	settings.amplitude = 16.0f;
	settings.frequency = 5.0f;
	settings.ttl = 1.0f;
	settings.renderTarget = rt1;
	settings.finalPass = NO_RID;

	ScreenShakeComponent screenShake(&settings);

	while (ds::isRunning()) {

		ds::begin();

		if (ds::isKeyPressed('A')) {
			screenShake.activate();
		}

		if (screenShake.isActive()) {
			float elapsed = static_cast<float>(ds::getElapsedSeconds());
			screenShake.tick(elapsed);
			ds::submit(orthoPass, backItem);
			screenShake.render();
		}
		else {
			ds::submit(ppPass, backItem);
		}
		ds::end();
	}
	ds::shutdown();
}