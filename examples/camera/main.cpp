#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#include "..\common\Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\Grid.h"
#include "..\common\Geometry.h"
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
	Vertex(const ds::vec3& pv, const ds::vec2& uvv) : p(pv), uv(uvv) {}
};

struct CubeConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
};

// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	ds::TextureInfo info = { x,y,n,data,ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(info);
	stbi_image_free(data);
	return textureID;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {


	ds::vec3 positions[24];
	ds::vec2 uvs[24];
	ds::matrix m = ds::matIdentity();
	geometry::buildCube(m, positions, uvs);

	Vertex v[24];
	for (int i = 0; i < 24; ++i) {
		v[i] = Vertex(positions[i], uvs[i]);
	}

	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Camera Demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 1;
	rs.supportDebug = true;
	ds::init(rs);

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 0.0f, -6.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	ds::Camera camera = {
		viewMatrix,
		projectionMatrix,
		viewMatrix * projectionMatrix,
		ds::vec3(0,0,-12),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};

	ds::ViewportInfo vpInfo = { 1024,768,0.0f,100.0f };
	RID vp = ds::createViewport(vpInfo);
	ds::RenderPassInfo rpInfo = { &camera, vp, ds::DepthBufferState::ENABLED, 0, 0 };
	RID basicPass = ds::createRenderPass(rpInfo);

	RID textureID = loadImage("..\\common\\cube_map.png");
	RID cubeTextureID = loadImage("..\\common\\grid.png");

	ds::BlendStateInfo blendStateInfo {ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
	RID bs_id = ds::createBlendState(blendStateInfo);

	ds::ShaderInfo vsInfo = { 0, Textured_VS_Main, sizeof(Textured_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID vertexShader = ds::createShader(vsInfo);
	ds::ShaderInfo psInfo = { 0, Textured_PS_Main, sizeof(Textured_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID pixelShader = ds::createShader(psInfo);

	Grid grid(&camera);
	ds::vec3 gridPositions[] = {
		ds::vec3(-4.0f, -1.0f, -3.5f),
		ds::vec3(-4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f, -3.5f)
	};
	grid.create(gridPositions, 4, vertexShader, pixelShader, textureID, basicPass);

	ds::InputLayoutDefinition decl[] = {
		{ "POSITION", 0, ds::BufferAttributeType::FLOAT3 },
		{ "TEXCOORD", 0, ds::BufferAttributeType::FLOAT2 }
	};
	ds::InputLayoutInfo layoutInfo = { decl,2,vertexShader };
	RID rid = ds::createInputLayout(layoutInfo);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);
	RID indexBuffer = ds::createQuadIndexBuffer(256);
	ds::VertexBufferInfo vbInfo = { ds::BufferType::STATIC, 24, sizeof(Vertex), v };
	RID cubeBuffer = ds::createVertexBuffer(vbInfo);
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::WRAP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);

	ds::vec3 scale(1.0f, 1.0f, 1.0f);
	ds::vec3 rotation(0.0f, 0.0f, 0.0f);
	ds::vec3 pos(0.0f, 0.0f, 0.0f);
	FPSCamera fpsCamera(&camera);
	fpsCamera.setPosition(ds::vec3(0, 1, -6));
	fpsCamera.buildView();

	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.constantBuffer(cbid, vertexShader, 0)
		.vertexBuffer(cubeBuffer)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.indexBuffer(indexBuffer)
		.texture(cubeTextureID,pixelShader,0)
		.build();

	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	RID drawItem = ds::compile(drawCmd, stateGroup);

	while (ds::isRunning()) {
		ds::begin();

		fpsCamera.update(static_cast<float>(ds::getElapsedSeconds()));
		grid.render();

		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;
		constantBuffer.viewprojectionMatrix = ds::matTranspose(camera.viewProjectionMatrix);
		// spinning cube
		ds::matrix world = ds::matIdentity();
		rotation.y += 2.0f  * static_cast<float>(ds::getElapsedSeconds());
		rotation.x += 1.0f  * static_cast<float>(ds::getElapsedSeconds());
		ds::matrix rotY = ds::matRotationY(rotation.y);
		ds::matrix rotX = ds::matRotationX(rotation.x);
		ds::matrix rotZ = ds::matRotationZ(rotation.z);
		ds::matrix s = ds::matScale(scale);
		ds::matrix w = rotZ * rotY * rotX * s * world;
		constantBuffer.worldMatrix = ds::matTranspose(w);
		ds::submit(basicPass, drawItem);

		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());
		ds::dbgPrint(0, 1, "Use W A S D to move camera and hold mouse button to rotate");

		ds::end();
	}
	ds::shutdown();
	return 0;
}