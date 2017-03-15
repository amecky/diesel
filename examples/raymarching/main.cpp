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

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	RID rid = ds::createVertexDeclaration(decl, 2, shaderID);
	RID indexBuffer = ds::createQuadIndexBuffer(1);

	RID ppCBID = ds::createConstantBuffer(sizeof(PostProcessBuffer));
	RID ccbID = ds::createConstantBuffer(sizeof(CubeConstantBuffer));

	CubeConstantBuffer constantBuffer;
	constantBuffer.viewprojectionMatrix = ds::matTranspose(ds::getViewProjectionMatrix());
	ds::matrix world = ds::matIdentity();
	constantBuffer.worldMatrix = ds::matTranspose(world);

	PostProcessBuffer ppBuffer;

	Vertex vertices[4] = {
		{ ds::vec3(-2,-2,0),ds::vec2(0,1) },
		{ ds::vec3(-2, 2,0),ds::vec2(0,0) },
		{ ds::vec3(2, 2,0),ds::vec2(1,0) },
		{ ds::vec3(2,-2,0),ds::vec2(1,1) },
	};
	RID gridBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, sizeof(Vertex), vertices);
	ds::StateGroup* ppGroup = ds::createStateGroup();
	// render post process effect
	ppGroup->bindLayout(rid);
	ppGroup->bindIndexBuffer(indexBuffer);
	ppGroup->bindVertexBuffer(gridBuffer);
	ppGroup->bindBlendState(bs_id);
	ppGroup->bindShader(shaderID);
	ppGroup->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	//ppGroup->bindRasterizerState(rasterizerStateID);
	ppBuffer.data = ds::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	ppGroup->bindConstantBuffer(ppCBID,ds::ShaderType::PIXEL, &ppBuffer);
	ppGroup->bindConstantBuffer(ccbID, ds::ShaderType::VERTEX, &constantBuffer);

	ds::DrawCommand ppCmd = { 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
	ds::DrawItem* ppItem = ds::compile(ppCmd, ppGroup);

	ds::vec3 vp = ds::vec3(0.0f, 0.0f, -0.1f);
	ds::setViewPosition(vp);

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