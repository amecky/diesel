#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	ds::vec3 p;
	ds::vec2 uv;

	Vertex() : p(0.0f), uv(0.0f) {}
	Vertex(const ds::vec3& pv, float u, float v) : p(pv), uv(u, v) {}
};

struct CubeConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
};

struct PostProcessBuffer {
	ds::vec4 data;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	Vertex vertices[4] = {
		Vertex(ds::vec3(-0.2f,-0.2f,0.0f),1.0f,0.0f),
		Vertex(ds::vec3(-0.2f,0.2f,0.0f),0.0f,0.0f),
		Vertex(ds::vec3(0.2f,0.2f,0.0f),0.0f,1.0f),
		Vertex(ds::vec3(0.2f,-0.2f,0.0f),1.0f,1.0f),
	};

	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Screenshake demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);

	RID rtID = ds::createRenderTarget(1024, 768, ds::Color(0.0f, 0.0f, 0.0f, 1.0f));
	RID rts[] = { rtID };

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 0.0f, -1.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	RID rtPass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::ENABLED, rts, 1);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	RID ppPass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::DISABLED);

	int x, y, n;
	unsigned char *data = stbi_load("..\\common\\cube_map.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);

	RID objVertexShader = ds::loadVertexShader("..\\..\\examples\\obj\\Obj_vs.cso");
	RID objPixelShader = ds::loadPixelShader("..\\..\\examples\\obj\\Obj_ps.cso");


	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	CubeConstantBuffer constantBuffer;
	
	constantBuffer.viewprojectionMatrix = ds::matTranspose(ds::getViewProjectionMatrix(rtPass));
	constantBuffer.worldMatrix = ds::matTranspose(ds::matIdentity());

	RID rid = ds::createVertexDeclaration(decl, 2, objVertexShader);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);
	RID indexBufferID = ds::createQuadIndexBuffer(6);
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, 4, sizeof(Vertex), vertices);
	RID floorBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, sizeof(Vertex), vertices);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	RID fsVertexShader = ds::loadVertexShader("Fullscreen_vs.cso");
	RID fsPixelShader = ds::loadPixelShader("Fullscreen_ps.cso");

	PostProcessBuffer ppBuffer;
	RID ppCBID = ds::createConstantBuffer(sizeof(PostProcessBuffer), &ppBuffer);
	
	float t = 0.0f;

	
		
	RID rasterizerStateID = ds::createRasterizerState(ds::CullMode::BACK, ds::FillMode::SOLID, true, false, 0.0f, 0.0f);

	

	RID staticGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.indexBuffer(indexBufferID)
		.constantBuffer(cbid, objVertexShader, 0)
		.vertexShader(objVertexShader)
		.pixelShader(objPixelShader)
		.vertexBuffer(floorBuffer)
		.samplerState(ssid,objPixelShader)
		.texture(textureID, objPixelShader, 0)
		.build();

	ds::DrawCommand staticCmd = { 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
	RID staticItem = ds::compile(staticCmd, staticGroup);

	ppBuffer.data = ds::vec4(abs(sin(t*0.5f)), 0.0f, 0.0f, 0.0f);

	RID ppGroup = ds::StateGroupBuilder()
		.inputLayout(NO_RID)
		.indexBuffer(NO_RID)
		.vertexBuffer(NO_RID)
		.textureFromRenderTarget(rtID, fsPixelShader, 0)
		.vertexShader(fsVertexShader)
		.pixelShader(fsPixelShader)
		.samplerState(ssid,fsPixelShader)
		.constantBuffer(ppCBID, fsPixelShader, 0)
		.build();
	
	

	ds::DrawCommand ppCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };
	RID ppItem = ds::compile(ppCmd, ppGroup);
	
	ds::printResources();

	while (ds::isRunning()) {
		ds::begin();
		t += static_cast<float>(ds::getElapsedSeconds());
		//ds::setRenderTarget(rtID);		
		ds::submit(rtPass, staticItem);
		//ds::restoreBackBuffer();
		//ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
		ppBuffer.data = ds::vec4(abs(sin(t*0.5f)), 0.0f, 0.0f, 0.0f);
		ds::submit(ppPass, ppItem);		
		//ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		ds::end();
	}
	ds::shutdown();
}