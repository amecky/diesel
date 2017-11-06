#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "GrayFade_VS_Main.h"
#include "GrayFade_PS_Main.h"
#include "..\common\Textured_VS_Main.h"
#include "..\common\Textured_PS_Main.h"
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
	rs.title = "Postprocess demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);
	ds::RenderTargetInfo rtInfo = { 1024, 768, ds::Color(0, 0, 0, 1) };
	RID rtID = ds::createRenderTarget(rtInfo);
	RID rts[] = { rtID };

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
	ds::ViewportInfo vpInfo = { 1024,768,0.0f,1.0f };
	RID vp = ds::createViewport(vpInfo);
	ds::RenderPassInfo rpInfo = { &camera, vp, ds::DepthBufferState::ENABLED, rts, 1 };
	RID rtPass = ds::createRenderPass(rpInfo);

	ds::BlendStateInfo blendInfo = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
	RID bs_id = ds::createBlendState(blendInfo);

	ds::RenderPassInfo ppPassInfo = { &camera, vp, ds::DepthBufferState::DISABLED, 0, 0 };
	RID ppPass = ds::createRenderPass(ppPassInfo);

	int x, y, n;
	unsigned char *data = stbi_load("..\\common\\cube_map.png", &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(texInfo);
	stbi_image_free(data);

	ds::ShaderInfo vsInfo = { 0, Textured_VS_Main, sizeof(Textured_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID objVertexShader = ds::createShader(vsInfo);
	ds::ShaderInfo psInfo = { 0, Textured_PS_Main, sizeof(Textured_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID objPixelShader = ds::createShader(psInfo);

	ds::InputLayoutDefinition decl[] = {
		{ "POSITION", 0, ds::BufferAttributeType::FLOAT3 },
		{ "TEXCOORD", 0, ds::BufferAttributeType::FLOAT2 }
	};

	CubeConstantBuffer constantBuffer;
	
	constantBuffer.viewprojectionMatrix = ds::matTranspose(camera.viewProjectionMatrix);
	constantBuffer.worldMatrix = ds::matTranspose(ds::matIdentity());

	ds::InputLayoutInfo layoutInfo = { decl, 2, objVertexShader };
	RID rid = ds::createInputLayout(layoutInfo);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);
	RID indexBufferID = ds::createQuadIndexBuffer(6);
	ds::VertexBufferInfo vbInfo = { ds::BufferType::STATIC, 4, sizeof(Vertex), vertices };
	RID vbid = ds::createVertexBuffer(vbInfo);
	ds::VertexBufferInfo fbInfo = { ds::BufferType::STATIC, 4, sizeof(Vertex), vertices };
	RID floorBuffer = ds::createVertexBuffer(fbInfo);
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);

	ds::ShaderInfo fvsInfo = { 0, GrayFade_VS_Main, sizeof(GrayFade_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID fsVertexShader = ds::createShader(fvsInfo);
	ds::ShaderInfo fpsInfo = { 0, GrayFade_PS_Main, sizeof(GrayFade_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID fsPixelShader = ds::createShader(fpsInfo);

	PostProcessBuffer ppBuffer;
	RID ppCBID = ds::createConstantBuffer(sizeof(PostProcessBuffer), &ppBuffer);
	
	float t = 0.0f;

	
	ds::RasterizerStateInfo rsInfo = { ds::CullMode::BACK, ds::FillMode::SOLID, true, false, 0.0f, 0.0f };
	RID rasterizerStateID = ds::createRasterizerState(rsInfo);

	

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
	
	while (ds::isRunning()) {
		ds::begin();
		
		t += static_cast<float>(ds::getElapsedSeconds());
		ds::submit(rtPass, staticItem);

		ppBuffer.data = ds::vec4(abs(sin(t*0.5f)), 0.0f, 0.0f, 0.0f);
		ds::submit(ppPass, ppItem);		
		
		ds::end();
	}
	ds::shutdown();
	return 0;
}