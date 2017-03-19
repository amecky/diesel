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
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);
	return textureID;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
int main(int argc, char *argv[]) {

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

	RID textureID = loadImage("..\\common\\cube_map.png");
	RID cubeTextureID = loadImage("..\\common\\grid.png");
	
	RID vertexShader = ds::loadVertexShader("..\\common\\Textured_vs.cso");
	RID pixelShader = ds::loadPixelShader("..\\common\\Textured_ps.cso");

	float gridWidth = 3.0f;
	float gridHeight = 3.0f;
	Grid grid;
	ds::vec3 gridPositions[] = {
		ds::vec3(-gridWidth, -1.0f, -gridHeight),
		ds::vec3(-gridWidth, -1.0f,  gridHeight),
		ds::vec3( gridWidth, -1.0f,  gridHeight),
		ds::vec3( gridWidth, -1.0f, -gridHeight)
	};
	grid.create(gridPositions, 6, vertexShader, pixelShader, cubeTextureID);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	RID rid = ds::createVertexDeclaration(decl, 2, vertexShader);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBuffer = ds::createQuadIndexBuffer(256);
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), v);
	RID staticCubes = ds::createVertexBuffer(ds::BufferType::STATIC, totalCubeVertices, sizeof(Vertex), sv);
	ds::vec3 vp = ds::vec3(0.0f, 2.0f, -8.0f);
	ds::setViewPosition(vp);

	RID basicPass = ds::createRenderPass(ds::getViewMatrix(), ds::getProjectionMatrix(), ds::DepthBufferState::ENABLED);
	// create orthographic render pass
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	RID orthoPass = ds::createRenderPass(orthoView, orthoProjection, ds::DepthBufferState::DISABLED);

	

	worldMatrix wm;

	RID basicGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.constantBuffer(cbid, vertexShader, 0, &constantBuffer)
		.texture(textureID, pixelShader, 0)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.indexBuffer(indexBuffer)
		.build();

	RID staticGroup = ds::StateGroupBuilder()
		.vertexBuffer(staticCubes)
		.build();

	RID cubeGroup = ds::StateGroupBuilder()
		.vertexBuffer(cubeBuffer)
		.build();

	ds::DrawCommand staticCmd = { totalCubeVertices / 4 * 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	RID staticStack[] = { basicGroup, staticGroup };
	RID staticItem = ds::compile(staticCmd, staticStack, 2);

	RID cubeStack[] = { basicGroup, cubeGroup };
	RID cubeItem = ds::compile(drawCmd, cubeStack, 2);

	// prepare IMGUI
	
	RID guiTextureID = loadImage("..\\common\\imgui.png");
	SpriteBuffer spriteBuffer(512, guiTextureID);
	gui::init(&spriteBuffer, guiTextureID);

	ds::vec3 rotation = ds::vec3(0.0f, 0.0f, 0.0f);
	ds::vec3 position = ds::vec3(0.0f, 0.0f, 0.0f);

	
	ds::printResources();

	int state = 1;
	while (ds::isRunning()) {
		ds::begin();
		// grid
		grid.render();
		// static cubes
		constantBuffer.viewprojectionMatrix = ds::matTranspose(ds::getViewProjectionMatrix());
		ds::matrix world = ds::matIdentity();
		constantBuffer.worldMatrix = ds::matTranspose(world);
		ds::submit(staticItem);
		ds::gpu::measure(1);
		// spinning cube
		wm.setRotation(rotation);
		wm.setPosition(position);
		constantBuffer.worldMatrix = wm.getTransposedMatrix();
		ds::submit(cubeItem);
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
			gui::Value("DrawTime", 1000.0f * ds::gpu::getTotalTime(), "%0.4f");
			gui::Value("DT1", 1000.0f * ds::gpu::getAverageTime(1), "%0.4f");
			gui::Value("DT2", 1000.0f * ds::gpu::getAverageTime(2), "%0.4f");
			gui::Value("DT3", 1000.0f * ds::gpu::getAverageTime(3), "%0.4f");
		}
		gui::end();
		spriteBuffer.flush();
		ds::end();
	}
	ds::shutdown();
}