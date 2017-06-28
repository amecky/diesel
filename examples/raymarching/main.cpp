#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "Raymarching_VS_Main.h"
#include "Raymarching_PS_Main.h"
// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	ds::vec3 p;
	ds::vec2 uv;

};

// ---------------------------------------------------------------
// the cube constant buffer
// ---------------------------------------------------------------
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
	rs.width = 800;
	rs.height = 600;
	rs.title = "ray marching demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	ds::BlendStateInfo blendInfo = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
	RID bs_id = ds::createBlendState(blendInfo);
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);

	ds::ShaderInfo vsInfo = { 0, Raymarching_VS_Main, sizeof(Raymarching_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID rayVS = ds::createShader(vsInfo);
	ds::ShaderInfo psInfo = { 0, Raymarching_PS_Main, sizeof(Raymarching_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID rayPS = ds::createShader(psInfo);

	ds::InputLayoutDefinition decl[] = {
		{ "POSITION", 0, ds::BufferAttributeType::FLOAT3 },
		{ "TEXCOORD", 0, ds::BufferAttributeType::FLOAT2 }
	};

	ds::InputLayoutInfo layoutInfo = { decl, 2, rayVS };
	RID rid = ds::createInputLayout(layoutInfo);
	RID indexBuffer = ds::createQuadIndexBuffer(1);

	PostProcessBuffer ppBuffer;
	ppBuffer.data = ds::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	CubeConstantBuffer constantBuffer;

	RID ppCBID = ds::createConstantBuffer(sizeof(PostProcessBuffer), &ppBuffer);
	RID ccbID = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);

	

	ds::matrix viewMatrix = ds::matIdentity();
	ds::matrix projectionMatrix = ds::matOrthoLH(800.0f, 600.0f, 0.1f, 1.0f);
	ds::matrix viewProjectionMatrix = viewMatrix * projectionMatrix;
	ds::Camera camera = {
		viewMatrix,
		projectionMatrix,
		viewMatrix * projectionMatrix,
		ds::vec3(0,3,-6),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	ds::RenderPassInfo rpInfo = { &camera, ds::DepthBufferState::DISABLED, 0, 0 };
	RID basicPass = ds::createRenderPass(rpInfo);

	constantBuffer.viewprojectionMatrix = ds::matTranspose(viewProjectionMatrix);
	ds::matrix world = ds::matIdentity();
	constantBuffer.worldMatrix = ds::matTranspose(world);

	

	Vertex vertices[4] = {
		{ ds::vec3(-400,-300,1),ds::vec2(0,1) },
		{ ds::vec3(-400, 300,1),ds::vec2(0,0) },
		{ ds::vec3( 400, 300,1),ds::vec2(1,0) },
		{ ds::vec3( 400,-300,1),ds::vec2(1,1) },
	};
	ds::VertexBufferInfo vbInfo = { ds::BufferType::STATIC, 4, sizeof(Vertex), vertices };
	RID gridBuffer = ds::createVertexBuffer(vbInfo);
	RID ppGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.indexBuffer(indexBuffer)
		.vertexBuffer(gridBuffer)
		.blendState(bs_id)
		.vertexShader(rayVS)
		.pixelShader(rayPS)
		.samplerState(ssid, rayPS)
		.constantBuffer(ppCBID, rayPS, 0)
		.constantBuffer(ccbID, rayVS, 0)
		.build();

	ds::DrawCommand ppCmd = { 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
	RID ppItem = ds::compile(ppCmd, ppGroup);

	float t = 0.0f;
	
	while (ds::isRunning()) {
		ds::begin();
		t += static_cast<float>(ds::getElapsedSeconds());
		ppBuffer.data = ds::vec4(t, 0.0f, 0.0f, 0.0f);		
		ds::submit(basicPass, ppItem);		
		ds::end();
	}
	ds::shutdown();
	return 0;
}