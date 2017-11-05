#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "Fullscreen_PS_Main.h"
#include "Fullscreen_VS_Main.h"
#include "desaturate_CSMain.h"

struct ConstantBuffer {
	float amplitude;
	float frequency;
	float ttl;
	float timer;
};
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
//int main(int argc, char *argv[]) {
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Compute shader example";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.supportDebug = true;
	ds::init(rs);

	int x, y, n;
	unsigned char *data = stbi_load("martian_oasis_by_smnbrnr.png", &x, &y, &n, 4);
	ds::TextureInfo baseInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(baseInfo);
	ds::TextureInfo lowerImage = { 1024, 384, 4, 0, ds::TextureFormat::R8G8B8A8_UNORM, ds::BindFlag::BF_UNORDERED_ACCESS | ds::BindFlag::BF_SHADER_RESOURCE };
	RID ext = ds::createTexture(lowerImage, "OutputTex");

	RID computeShader = ds::createComputeShader(desaturate_CSMain, sizeof(desaturate_CSMain), "Desaturate");
	ds::StructuredBufferInfo inputBufferInfo = { x * y * n, n, true, false, data, NO_RID, NO_RID };
	RID inputBuffer = ds::createStructuredBuffer(inputBufferInfo, "InputStructuredBuffer");
	ds::StructuredBufferInfo outputBufferInfo = { x * y * n, n, false, true, 0, ext, NO_RID };
	RID outputBuffer = ds::createStructuredBuffer(outputBufferInfo, "OutputStructuredBuffer");

	ConstantBuffer buffer;
	buffer.amplitude = 0.0f;
	buffer.frequency = 0.0f;
	buffer.ttl = 0.0f;
	buffer.timer = 0.0f;
	RID cbID = ds::createConstantBuffer(sizeof(ConstantBuffer), &buffer, "CB-Buffer");

	RID csID = ds::ComputeShaderBuilder()
		.shader(computeShader)
		.input(inputBuffer, 0)
		.output(outputBuffer, 0)
		.constantBuffer(cbID,0)
		.build();

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
	ds::ViewportInfo vpInfo = { 1024,768,0.0f,100.0f };
	RID vp = ds::createViewport(vpInfo);
	ds::RenderPassInfo rpInfo = {&camera, vp, ds::DepthBufferState::DISABLED, 0, 0 };
	RID ppPass = ds::createRenderPass(rpInfo);
	
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);

	ds::ShaderInfo vsInfo = { 0, Fullscreen_VS_Main, sizeof(Fullscreen_VS_Main) , ds::ShaderType::ST_VERTEX_SHADER };
	RID fsVertexShader = ds::createVertexShader(Fullscreen_VS_Main, sizeof(Fullscreen_VS_Main), "Fullscreen_VS");
	ds::ShaderInfo psInfo = { 0, Fullscreen_PS_Main, sizeof(Fullscreen_PS_Main) , ds::ShaderType::ST_PIXEL_SHADER };
	RID fsPixelShader = ds::createPixelShader(Fullscreen_PS_Main, sizeof(Fullscreen_PS_Main), "Fullscreen_PS");
	

	RID ppGroup = ds::StateGroupBuilder()
		.inputLayout(NO_RID)
		.indexBuffer(NO_RID)
		.vertexBuffer(NO_RID)
		.texture(textureID, fsPixelShader, 0)
		.texture(ext, fsPixelShader, 1)
		.vertexShader(fsVertexShader)
		.pixelShader(fsPixelShader)
		.samplerState(ssid, fsPixelShader)
		.build();

	ds::DrawCommand ppCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };
	RID ppItem = ds::compile(ppCmd, ppGroup);

	float timer = 0.0f;
	

	while (ds::isRunning()) {
		ds::begin();

		timer += ds::getElapsedSeconds();

		buffer.timer = sin(timer);

		ds::updateConstantBuffer(cbID);

		ds::dispatch(csID, 32, 24, 1);

		ds::submit(ppPass, ppItem);
		
		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());

		ds::end();
	}

	stbi_image_free(data);
	ds::shutdown();
	return 0;
}