#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\Grid.h"
#include "..\common\Geometry.h"
#include "..\common\worldMatrix.h"
#include "..\common\SpriteBuffer.h"
#include "..\common\imgui.h"

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

// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM, ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(texInfo);
	stbi_image_free(data);
	return textureID;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
//int main(int argc, char *argv[]) {

	ds::vec3 positions[24];
	ds::vec2 uvs[24];
	ds::matrix m = ds::matIdentity();
	geometry::buildCube(m, positions, uvs);

	Vertex v[24];
	for (int i = 0; i < 24; ++i) {
		v[i] = { positions[i], uvs[i] * 0.5f };
	}

	ds::vec3 CP[] = { ds::vec3(-2.0f,-0.5f,-2.0f),ds::vec3(-2.0f,-0.5f,2.0f),ds::vec3(2.0f,-0.5f,-2.0f),ds::vec3(2.0f,-0.5f,2.0f) };
	const int numCubes = 4;
	const int totalCubeVertices = numCubes * 24;
	Vertex sv[totalCubeVertices];
	int cnt = 0;
	for(int j = 0; j < numCubes; ++j) {
		ds::matrix m = ds::matTranslate(CP[j]);
		ds::matrix s = ds::matScale(ds::vec3(1.0f,1.0f,1.0f));
		ds::matrix w = s * m;
		geometry::buildCube(w, positions, uvs);
		for (int i = 0; i < 24; ++i) {
			sv[cnt++] = { positions[i], uvs[i] * 0.5f };
		}
	}
	
	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Simple cube texturing demo";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 1;
	rs.useGPUProfiling = true;
	ds::init(rs);

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 2.0f, -8.0f), ds::vec3(0,0,0), ds::vec3(0,1,0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);	
	ds::Camera camera = {
		viewMatrix,
		projectionMatrix,
		viewMatrix * projectionMatrix,
		ds::vec3(0,0,-1),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0)
	};
	ds::RenderPassInfo rpInfo = { &camera,ds::DepthBufferState::ENABLED, 0, 0 };
	RID basicPass = ds::createRenderPass(rpInfo, "BasicPass");

	RID textureID = loadImage("..\\common\\cube_map.png");
	RID cubeTextureID = loadImage("..\\common\\grid.png");
	
	RID vertexShader = ds::loadVertexShader("..\\common\\Textured_vs.cso", "TextureVS");
	RID pixelShader = ds::loadPixelShader("..\\common\\Textured_ps.cso", "TexturePS");

	float gridWidth = 3.0f;
	float gridHeight = 3.0f;
	Grid grid(&camera);
	ds::vec3 gridPositions[] = {
		ds::vec3(-gridWidth, -1.0f, -gridHeight),
		ds::vec3(-gridWidth, -1.0f,  gridHeight),
		ds::vec3( gridWidth, -1.0f,  gridHeight),
		ds::vec3( gridWidth, -1.0f, -gridHeight)
	};
	grid.create(gridPositions, 6, vertexShader, pixelShader, cubeTextureID, basicPass);

	ds::InputLayoutDefinition decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};
	ds::InputLayoutInfo layoutInfo = { decl, 2, vertexShader };
	RID rid = ds::createInputLayout(layoutInfo, "PosTex_Layout");

	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer, "CubeConstantBuffer");
	RID indexBuffer = ds::createQuadIndexBuffer(256, "IndexBuffer");
	ds::VertexBufferInfo cbInfo = { ds::BufferType::STATIC, 24, sizeof(Vertex), v };
	RID cubeBuffer = ds::createVertexBuffer(cbInfo, "CubeBuffer");
	ds::VertexBufferInfo scInfo = { ds::BufferType::STATIC, totalCubeVertices, sizeof(Vertex), sv };
	RID staticCubes = ds::createVertexBuffer(scInfo, "StaticCubes");

	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);
	RID blendStateID = ds::findResource(SID("DefaultBlendState"), ds::ResourceType::RT_BLENDSTATE);

	worldMatrix wm;

	RID basicGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.blendState(blendStateID)
		.constantBuffer(cbid, vertexShader, 0)
		.texture(textureID, pixelShader, 0)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.samplerState(ssid,pixelShader)
		.indexBuffer(indexBuffer)
		.build("BasicGroup");

	RID staticGroup = ds::StateGroupBuilder()
		.vertexBuffer(staticCubes)
		.build("StaticCubeGroup");

	RID cubeGroup = ds::StateGroupBuilder()
		.vertexBuffer(cubeBuffer)
		.build("CubeGroup");

	ds::DrawCommand staticCmd = { totalCubeVertices / 4 * 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	RID staticStack[] = { basicGroup, staticGroup };
	RID staticItem = ds::compile(staticCmd, staticStack, 2, "StaticItem");

	RID cubeStack[] = { basicGroup, cubeGroup };
	RID cubeItem = ds::compile(drawCmd, cubeStack, 2, "CubeItem");

	// prepare IMGUI
	
	RID guiTextureID = loadImage("..\\common\\imgui.png");
	SpriteBuffer spriteBuffer(512, guiTextureID);
	gui::init(&spriteBuffer, guiTextureID);

	ds::vec3 rotation = ds::vec3(0.0f, 0.0f, 0.0f);
	ds::vec3 position = ds::vec3(0.0f, 0.0f, 0.0f);

	
	ds::saveResourcesToFile("log.txt");

	int state = 1;
	while (ds::isRunning()) {
		ds::begin();
		// grid
		grid.render();
		// static cubes
		constantBuffer.viewprojectionMatrix = ds::matTranspose(camera.viewProjectionMatrix);
		ds::matrix world = ds::matIdentity();
		constantBuffer.worldMatrix = ds::matTranspose(world);
		ds::submit(basicPass, staticItem);
		ds::gpu::measure(1);
		// spinning cube
		wm.setRotation(rotation);
		wm.setPosition(position);
		constantBuffer.worldMatrix = wm.getTransposedMatrix();
		ds::submit(basicPass, cubeItem);
		ds::gpu::measure(2);

		// GUI
		spriteBuffer.begin();
		gui::start(ds::vec2(0, 750));		
		gui::begin("Rotation", &state);
		if (state == 1) {
			gui::Value("FPS", ds::getFramesPerSecond());
			gui::SliderAngle("Rotation X", &rotation.x);
			gui::SliderAngle("Rotation Y", &rotation.y);
			gui::SliderAngle("Rotation Z", &rotation.z);
			gui::Input("Position", &position);
		}
		gui::end();
		spriteBuffer.flush();
		ds::gpu::measure(3);
		ds::gpu::waitForData();
		// GUI
		spriteBuffer.begin();
		gui::start(ds::vec2(700, 750));
		gui::begin("Timer", &state);
		if (state == 1) {
			gui::Value("DrawTime", 1000.0f * ds::gpu::totalTime(), "%0.4f");
			gui::Value("DT1", 1000.0f * ds::gpu::dtAvg(1), "%0.4f");
			gui::Value("DT2", 1000.0f * ds::gpu::dtAvg(2), "%0.4f");
			gui::Value("DT3", 1000.0f * ds::gpu::dtAvg(3), "%0.4f");
		}
		gui::end();
		spriteBuffer.flush();
		ds::end();
	}
	ds::shutdown();
}