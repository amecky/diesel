#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	v3 p;
	v2 uv;

	Vertex() : p(0.0f), uv(0.0f) {}
	Vertex(const v3& pv, float u, float v) : p(pv), uv(u, v) {}
};

struct CubeConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
};

struct PostProcessBuffer {
	v4 data;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	Vertex vertices[4] = {
		Vertex(v3(-0.2f,-0.2f,0.0f),1.0f,0.0f),
		Vertex(v3(-0.2f,0.2f,0.0f),0.0f,0.0f),
		Vertex(v3(0.2f,0.2f,0.0f),0.0f,1.0f),
		Vertex(v3(0.2f,-0.2f,0.0f),1.0f,1.0f),
	};

	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Postprocess demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	int x, y, n;
	unsigned char *data = stbi_load("..\\common\\cube_map.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "..\\..\\examples\\obj\\Obj_vs.cso" },
		{ ds::ShaderType::PIXEL, "..\\..\\examples\\obj\\Obj_ps.cso" }
	};

	RID blockShaderID = ds::createShader(desc, 2);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	RID rid = ds::createVertexDeclaration(decl, 2, blockShaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBufferID = ds::createQuadIndexBuffer(6);
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, 4, sizeof(Vertex), vertices);
	RID floorBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, sizeof(Vertex), vertices);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	ds::ShaderDescriptor fsdesc[] = {
		{ ds::ShaderType::VERTEX, "Fullscreen_vs.cso" },
		{ ds::ShaderType::PIXEL, "Fullscreen_ps.cso" }
	};

	RID shaderID = ds::createShader(fsdesc, 2);

	RID ppCBID = ds::createConstantBuffer(sizeof(PostProcessBuffer));
	PostProcessBuffer ppBuffer;
	float t = 0.0f;

	RID rtID = ds::createRenderTarget(1024, 768);

	CubeConstantBuffer constantBuffer;
		
	RID rasterizerStateID = ds::createRasterizerState(ds::CullMode::BACK, ds::FillMode::SOLID, true, false, 0.0f, 0.0f);
	v3 vp = v3(0.0f, 0.0f, -1.0f);
	ds::setViewPosition(vp);

	ds::StateGroup* staticGroup = ds::createStateGroup();
	staticGroup->bindLayout(rid);
	staticGroup->bindIndexBuffer(indexBufferID);
	staticGroup->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	staticGroup->bindShader(blockShaderID);
	staticGroup->bindVertexBuffer(floorBuffer);
	staticGroup->bindBlendState(bs_id);
	staticGroup->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	staticGroup->bindTexture(textureID, ds::ShaderType::PIXEL, 0);	

	ds::DrawCommand staticCmd = { 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
	ds::DrawItem* staticItem = ds::compile(staticCmd, staticGroup);

	ds::StateGroup* ppGroup = ds::createStateGroup();
	// render post process effect
	ppGroup->bindLayout(NO_RID);
	ppGroup->bindIndexBuffer(NO_RID);
	ppGroup->bindVertexBuffer(NO_RID);
	ppGroup->bindBlendState(bs_id);
	ppGroup->bindTextureFromRenderTarget(rtID, ds::ShaderType::PIXEL, 0);
	ppGroup->bindShader(shaderID);
	ppGroup->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	//ppGroup->bindRasterizerState(rasterizerStateID);
	ppBuffer.data = v4(abs(sin(t*0.5f)), 0.0f, 0.0f, 0.0f);
	ppGroup->bindConstantBuffer(ppCBID,ds::ShaderType::PIXEL, &ppBuffer);

	ds::DrawCommand ppCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };
	ds::DrawItem* ppItem = ds::compile(ppCmd, ppGroup);
	
	while (ds::isRunning()) {
		ds::begin();
		t += static_cast<float>(ds::getElapsedSeconds());
		ds::setRenderTarget(rtID);
		constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
		constantBuffer.worldMatrix = mat_Transpose(mat_identity());
		ds::submit(staticItem);
		ds::restoreBackBuffer();
		ppBuffer.data = v4(abs(sin(t*0.5f)), 0.0f, 0.0f, 0.0f);
		ds::submit(ppItem);		
		ds::end();
	}
	ds::shutdown();
}