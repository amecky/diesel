#define DS_IMPLEMENTATION
#include "..\..\diesel.h"

struct CubeConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
};

struct PostProcessBuffer {
	ds::vec4 data;
};

// https://www.shadertoy.com/view/Xds3zN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Raymarching demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "raymarching_vs.cso" },
		{ ds::ShaderType::PIXEL, "raymarching_ps.cso" }
	};

	RID shaderID = ds::createShader(desc, 2);

	RID ppCBID = ds::createConstantBuffer(sizeof(PostProcessBuffer));
	PostProcessBuffer ppBuffer;

	ds::StateGroup* ppGroup = ds::createStateGroup();
	// render post process effect
	ppGroup->bindLayout(NO_RID);
	ppGroup->bindIndexBuffer(NO_RID);
	ppGroup->bindVertexBuffer(NO_RID);
	ppGroup->bindBlendState(bs_id);
	ppGroup->bindShader(shaderID);
	ppGroup->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	//ppGroup->bindRasterizerState(rasterizerStateID);
	ppBuffer.data = ds::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	ppGroup->bindConstantBuffer(ppCBID,ds::ShaderType::PIXEL, &ppBuffer);

	ds::DrawCommand ppCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };
	ds::DrawItem* ppItem = ds::compile(ppCmd, ppGroup);

	float t = 0.0f;
	
	while (ds::isRunning()) {
		ds::begin();
		t += static_cast<float>(ds::getElapsedSeconds());
		ppBuffer.data = ds::vec4(t, 0.0f, 0.0f, 0.0f);
		ds::submit(ppItem);		
		ds::end();
	}
	ds::shutdown();
}