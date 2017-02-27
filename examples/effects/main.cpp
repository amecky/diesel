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
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	int x, y, n;
	unsigned char *data = stbi_load("..\\common\\cube_map.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);


	RID blockShaderID = ds::createShader();
	ds::loadVertexShader(blockShaderID, "..\\..\\examples\\obj\\Obj_vs.cso");
	ds::loadPixelShader(blockShaderID, "..\\..\\examples\\obj\\Obj_ps.cso");

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	RID rid = ds::createVertexDeclaration(decl, 2, blockShaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBufferID = ds::createQuadIndexBuffer(6);
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, 4, rid, vertices, sizeof(Vertex));
	RID floorBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, rid, vertices, sizeof(Vertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);


	RID shaderID = ds::createShader();
	ds::loadVertexShader(shaderID, "Fullscreen_vs.cso");
	ds::loadPixelShader(shaderID, "Fullscreen_ps.cso");
	RID ppCBID = ds::createConstantBuffer(sizeof(PostProcessBuffer));
	PostProcessBuffer ppBuffer;
	float t = 0.0f;

	RID rtID = ds::createRenderTarget(1024, 768);

	CubeConstantBuffer constantBuffer;
		
	RID rasterizerStateID = ds::createRasterizerState(ds::CullMode::BACK, ds::FillMode::SOLID, true, false, 0.0f, 0.0f);
	v3 vp = v3(0.0f, 0.0f, -1.0f);
	ds::setViewPosition(vp);
		
	while (ds::isRunning()) {
		ds::begin();
			
		t += static_cast<float>(ds::getElapsedSeconds());

		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;

		ds::setRenderTarget(rtID);
		// set floor buffer
		ds::setVertexBuffer(floorBuffer, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
		// this is common to both objects
		ds::setVertexDeclaration(rid);
		ds::setIndexBuffer(indexBufferID);
		ds::setBlendState(bs_id);
		ds::setShader(blockShaderID);
		ds::setSamplerState(ssid);
		ds::setTexture(textureID, ds::ShaderType::PIXEL);
		// update constant buffer use v3(0,0,0) as world position
		constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
		constantBuffer.worldMatrix = mat_Transpose(mat_identity());
		ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
		ds::setConstantBuffer(cbid,ds::ShaderType::VERTEX);
		// draw floor
		ds::drawIndexed(6);

		ds::restoreBackBuffer();
		
		// render post process effect
		ds::setVertexDeclaration(NO_RID);
		ds::setIndexBuffer(NO_RID);
		ds::setVertexBuffer(NO_RID, 0, 0, ds::PrimitiveTypes::TRIANGLE_LIST);
		ds::setBlendState(bs_id);
		ds::setTextureFromRenderTarget(rtID, ds::ShaderType::PIXEL, 0);
		ds::setShader(shaderID);
		ds::setSamplerState(ssid);
		ds::setRasterizerState(rasterizerStateID);
		ppBuffer.data = v4(abs(sin(t*0.5f)), 0.0f, 0.0f, 0.0f);
		ds::updateConstantBuffer(ppCBID,&ppBuffer,sizeof(PostProcessBuffer));
		ds::setConstantBuffer(ppCBID,ds::ShaderType::PIXEL);
		ds::draw(3);
		
		ds::end();
	}
	ds::shutdown();
}