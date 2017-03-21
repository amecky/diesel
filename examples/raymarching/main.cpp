#define DS_IMPLEMENTATION
#include "..\..\diesel.h"

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
	rs.title = "Raymarching demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	RID rayVS = ds::loadVertexShader("raymarching_vs.cso");
	RID rayPS = ds::loadPixelShader("raymarching_ps.cso");

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	RID rid = ds::createVertexDeclaration(decl, 2, rayVS);
	RID indexBuffer = ds::createQuadIndexBuffer(1);

	PostProcessBuffer ppBuffer;
	ppBuffer.data = ds::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	CubeConstantBuffer constantBuffer;

	RID ppCBID = ds::createConstantBuffer(sizeof(PostProcessBuffer), &ppBuffer);
	RID ccbID = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);

	

	ds::matrix viewMatrix = ds::matIdentity();
	ds::matrix projectionMatrix = ds::matOrthoLH(800.0f, 600.0f, 0.1f, 1.0f);
	ds::matrix viewProjectionMatrix = viewMatrix * projectionMatrix;

	ds::setViewMatrix(viewMatrix);
	ds::setProjectionMatrix(projectionMatrix);

	constantBuffer.viewprojectionMatrix = ds::matTranspose(viewProjectionMatrix);
	ds::matrix world = ds::matIdentity();
	constantBuffer.worldMatrix = ds::matTranspose(world);

	

	Vertex vertices[4] = {
		{ ds::vec3(-400,-300,1),ds::vec2(0,1) },
		{ ds::vec3(-400, 300,1),ds::vec2(0,0) },
		{ ds::vec3( 400, 300,1),ds::vec2(1,0) },
		{ ds::vec3( 400,-300,1),ds::vec2(1,1) },
	};
	RID gridBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, sizeof(Vertex), vertices);
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
		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
		t += static_cast<float>(ds::getElapsedSeconds());
		ppBuffer.data = ds::vec4(t, 0.0f, 0.0f, 0.0f);		
		ds::submit(ppItem);		
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		ds::end();
	}
	ds::shutdown();
}