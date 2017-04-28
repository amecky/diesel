#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "Fullscreen_PS_Main.h"
#include "Fullscreen_VS_Main.h"
#include "desaturate_CSMain.h"
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
int main(int argc, char *argv[]) {
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Spinning cube";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.supportDebug = true;
	ds::init(rs);

	int x, y, n;
	unsigned char *data = stbi_load("martian_oasis_by_smnbrnr.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);

	RID computeShader = ds::createComputeShader(desaturate_CSMain, sizeof(desaturate_CSMain), "Desaturate");
	RID inputBuffer = ds::createBuffer(x * y * 4, 4, data, "InputBuffer");
	RID inputSRV = ds::createShaderResourceView(inputBuffer, "InputSRV");
	RID outputBuffer = ds::createBuffer(x * y * 4, 4, 0, "OutputBuffer");
	RID outputSRV = ds::createUnorderedAccessView(outputBuffer, "OutputSRV");

	RID csID = ds::ComputeShaderBuilder()
		.shader(computeShader)
		.resourceView(inputSRV, 0)
		.unorderedAccessView(outputSRV, 0)
		.x(32)
		.y(24)
		.z(1)
		.build();

	ds::dispatch(csID);

	byte* nd = ds::getBufferData(outputBuffer);

	RID ext = ds::createTexture(x, y, n, nd, ds::TextureFormat::R8G8B8A8_UNORM);

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 0.0f, -1.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	RID ppPass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::DISABLED);
	
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	RID fsVertexShader = ds::createVertexShader(Fullscreen_VS_Main, sizeof(Fullscreen_VS_Main), "Fullscreen_VS");
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

	

	while (ds::isRunning()) {
		ds::begin();

		ds::submit(ppPass, ppItem);
		
		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());
		ds::dbgPrint(0, 1, "Simple spinning cube demo");

		ds::end();
	}

	stbi_image_free(data);
	ds::shutdown();
}