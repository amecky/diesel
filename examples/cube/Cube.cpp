#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#include "Cube_VS_Main.h"
#include "Cube_PS_Main.h"
#include "AmbientLightning_VS_Main.h"
#include "AmbientLightning_PS_Main.h"
/*
	This demo draws a moving coloured cube.
*/

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {
	ds::vec3 p;
	ds::Color color;

	Vertex() : p(0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	Vertex(const ds::vec3& pv, const ds::Color& c) : p(pv), color(c) {}
};

const ds::vec3 CUBE_VERTICES[8] = {
	ds::vec3(-0.5f,-0.5f,-0.5f),
	ds::vec3(-0.5f, 0.5f,-0.5f),
	ds::vec3( 0.5f, 0.5f,-0.5f),
	ds::vec3( 0.5f,-0.5f,-0.5f),
	ds::vec3(-0.5f,-0.5f, 0.5f),
	ds::vec3(-0.5f, 0.5f, 0.5f),
	ds::vec3( 0.5f, 0.5f, 0.5f),
	ds::vec3( 0.5f,-0.5f, 0.5f)
};

const int CUBE_PLANES[6][4] = {
	{ 0, 1, 2, 3 }, // front
	{ 3, 2, 6, 7 }, // left
	{ 1, 5, 6, 2 }, // top
	{ 4, 5, 1, 0 }, // right
	{ 4, 0, 3, 7 }, // bottom
	{ 7, 6, 5, 4 }, // back
};

// ---------------------------------------------------------------
// add plane
// ---------------------------------------------------------------
void addPlane(int side, const ds::Color& color, Vertex* vertices, uint32_t* indices) {
	int idx = side * 4;
	for (int i = 0; i < 4; ++i) {
		int p = idx + i;
		vertices[p] = Vertex(CUBE_VERTICES[CUBE_PLANES[side][i]], color);
	}
	int offset = side * 4;
	int ar[6] = { 0, 1, 2, 2, 3, 0 };
	for (int i = 0; i < 6; ++i) {
		indices[side * 6 + i] = offset + ar[i];
	}
}

// ---------------------------------------------------------------
// the cube constant buffer used by the vertex shader
// ---------------------------------------------------------------
struct CubeConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
};

struct LightBuffer {
	ds::Color ambientColor;
	ds::Color diffuseColor;
	ds::vec3 lightDirection;
	float padding;
};

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	uint32_t p_indices[36];
	Vertex v[24];
	addPlane(0, ds::Color(1.0f, 0.0f, 0.0f, 1.0f), v, p_indices);
	addPlane(1, ds::Color(0.0f, 1.0f, 0.0f, 1.0f), v, p_indices);
	addPlane(2, ds::Color(0.0f, 0.0f, 1.0f, 1.0f), v, p_indices);
	addPlane(3, ds::Color(1.0f, 0.0f, 1.0f, 1.0f), v, p_indices);
	addPlane(4, ds::Color(1.0f, 1.0f, 0.0f, 1.0f), v, p_indices);
	addPlane(5, ds::Color(0.0f, 1.0f, 1.0f, 1.0f), v, p_indices);

	CubeConstantBuffer constantBuffer;
	float t = 0.0f;

	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Spinning cube";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.supportDebug = true;
	ds::init(rs);

	LightBuffer lightBuffer;
	lightBuffer.ambientColor = ds::Color(0.15f, 0.15f, 0.15f, 1.0f);
	lightBuffer.diffuseColor = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
	lightBuffer.padding = 0.0f;
	ds::vec3 lightPos = ds::vec3(1.0f, -0.5f, 1.0f);
	lightBuffer.lightDirection = normalize(lightPos);

	RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer), &lightBuffer);

	RID bs_id = ds::createBlendState(ds::BlendStateDesc()
		.SrcBlend(ds::BlendStates::SRC_ALPHA)
		.SrcAlphaBlend(ds::BlendStates::SRC_ALPHA)
		.DestBlend(ds::BlendStates::INV_SRC_ALPHA)
		.DestAlphaBlend(ds::BlendStates::INV_SRC_ALPHA)
		.AlphaEnabled(true)
	);

	
	RID vertexShader = ds::createShader(ds::ShaderDesc()
		.Data(Cube_VS_Main)
		.DataSize(sizeof(Cube_VS_Main))
		.ShaderType(ds::ShaderType::ST_VERTEX_SHADER)
	);

	RID pixelShader = ds::createShader(ds::ShaderDesc()
		.Data(Cube_PS_Main)
		.DataSize(sizeof(Cube_PS_Main))
		.ShaderType(ds::ShaderType::ST_PIXEL_SHADER)
	);

	RID ambientVertexShader = ds::createShader(ds::ShaderDesc()
		.Data(AmbientLightning_VS_Main)
		.DataSize(sizeof(AmbientLightning_VS_Main))
		.ShaderType(ds::ShaderType::ST_VERTEX_SHADER)
	);

	RID ambientPixelShader = ds::createShader(ds::ShaderDesc()
		.Data(AmbientLightning_PS_Main)
		.DataSize(sizeof(AmbientLightning_PS_Main))
		.ShaderType(ds::ShaderType::ST_PIXEL_SHADER)
	);

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 2.0f, -6.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	ds::Camera camera = {
		viewMatrix,
		projectionMatrix,
		viewMatrix * projectionMatrix,
		ds::vec3(0,2,-6),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0)
	};
	ds::rebuildCamera(&camera);
	
	RID vp = ds::createViewport(ds::ViewportDesc()
		.Top(0)
		.Left(0)
		.Width(1024)
		.Height(768)
		.MinDepth(0.0f)
		.MaxDepth(1.0f)
	);

	
	RID basicPass = ds::createRenderPass(ds::RenderPassDesc()
		.Camera(&camera)
		.Viewport(vp)
		.DepthBufferState(ds::DepthBufferState::ENABLED)
		.RenderTargets(0)
		.NumRenderTargets(0)
	);

	// create buffer input layout
	ds::InputLayoutDefinition decl[] = {
		{ "POSITION", 0, ds::BufferAttributeType::FLOAT3 },
		{ "COLOR"   , 0, ds::BufferAttributeType::FLOAT4 }
	};

	RID rid = ds::createInputLayout(ds::InputLayoutDesc()
		.Declarations(decl)
		.NumDeclarations(2)
		.VertexShader(vertexShader)
	);

	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);
	
	RID iid = ds::createIndexBuffer(ds::IndexBufferDesc()
		.NumIndices(36)
		.BufferType(ds::BufferType::STATIC)
		.IndexType(ds::IndexType::UINT_32)
		.Data(p_indices)
	);

	RID vbid = ds::createVertexBuffer(ds::VertexBufferDesc()
		.BufferType(ds::BufferType::STATIC)
		.Data(v)
		.NumVertices(24)
		.VertexSize(sizeof(Vertex))
	);
	
	RID ssid = ds::createSamplerState(ds::SamplerStateDesc()
		.AddressMode(ds::TextureAddressModes::CLAMP)
		.Filter(ds::TextureFilters::LINEAR)
	);

	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.constantBuffer(cbid, vertexShader, 0)
		.blendState(bs_id)
		.samplerState(ssid, pixelShader)
		.vertexBuffer(vbid)		
		.indexBuffer(iid)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.build();

	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	RID drawItem = ds::compile(drawCmd, stateGroup);

	while (ds::isRunning()) {
		ds::begin();
		// move cube
		t += static_cast<float>(ds::getElapsedSeconds());		
		ds::vec3 cp = ds::vec3(cos(t), 0.0f, sin(t));
		ds::matrix bY = ds::matRotationY(t);
		float scale = 0.7f + sin(t) * 0.3f;
		ds::matrix s = ds::matScale(ds::vec3(scale));
		ds::matrix w = bY *ds::matTranslate(cp);
		
		constantBuffer.viewprojectionMatrix = ds::matTranspose(camera.viewProjectionMatrix);
		//constantBuffer.worldMatrix = ds::matTranspose(w);
		constantBuffer.worldMatrix = ds::matTranspose(ds::matIdentity());

		ds::submit(basicPass, drawItem);

		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());
		ds::dbgPrint(0, 1, "Simple spinning cube demo");

		ds::end();
	}
	ds::shutdown();
	return 0;
}