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

	Vertex() : p(0.0f), uv(0.0f) {}
	Vertex(const ds::vec3& pv, float u, float v) : p(pv) , uv(u,v) {}
	Vertex(const ds::vec3& pv, const ds::vec2& uvv) : p(pv), uv(uvv) {}
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

void saveObj(Vertex* vertices, int num) {
	std::vector<int> vertIndices;
	std::vector<int> uvIndices;
	std::vector<ds::vec3> vertCache;
	for (int i = 0; i < num; ++i) {
		bool found = false;
		for (int j = 0; j < vertCache.size(); ++j) {
			if (vertCache[j] == vertices[i].p) {
				found = true;
				vertIndices.push_back(j + 1);
			}
		}
		if (!found) {
			vertCache.push_back(vertices[i].p);
			vertIndices.push_back(vertCache.size());
		}
	}
	std::vector<ds::vec2> uvCache;
	for (int i = 0; i < num; ++i) {
		bool found = false;
		for (int j = 0; j < uvCache.size(); ++j) {
			if (uvCache[j] == vertices[i].uv) {
				found = true;
				uvIndices.push_back(j + 1);
			}
		}
		if (!found) {
			uvCache.push_back(vertices[i].uv);
			uvIndices.push_back(uvCache.size());
		}
	}

	FILE* fp = fopen("test.txt","w");
	if (fp) {
		for (int i = 0; i < vertCache.size(); ++i) {
			const ds::vec3& p = vertCache[i];
			fprintf(fp, "v %g %g %g\n", p.x, p.y, p.z);
		}
		for (int i = 0; i < uvCache.size(); ++i) {
			const ds::vec2& p = uvCache[i];
			fprintf(fp, "vt %g %g\n", p.x, p.y);
		}
		int faces = vertIndices.size() / 4;
		for (int i = 0; i < faces; ++i) {
			int idx = i * 4;
			fprintf(fp, "f %d/%d %d/%d %d/%d %d/%d\n", vertIndices[idx], uvIndices[idx], vertIndices[idx + 1], uvIndices[idx + 1], vertIndices[idx + 2], uvIndices[idx + 2], vertIndices[idx + 3], uvIndices[idx + 3]);
		}
		fclose(fp);
	}
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

	ds::vec3 CP[] = { ds::vec3(-2,0,-1),ds::vec3(-2,0,3),ds::vec3(2,0,-1),ds::vec3(2,0,3) };
	const int numCubes = 4;
	const int totalCubeVertices = numCubes * 24;
	Vertex sv[totalCubeVertices];
	int cnt = 0;
	for(int j = 0; j < numCubes; ++j) {
		ds::matrix m = ds::matTranslate(CP[j]);
		ds::matrix s = ds::matScale(ds::vec3(0.5f,2.0f,0.5f));
		ds::matrix w = s * m;
		geometry::buildCube(w, positions, uvs);
		for (int i = 0; i < 24; ++i) {
			sv[cnt++] = Vertex(positions[i], uvs[i]);
		}
	}
	
	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 1;
	ds::init(rs);

	RID textureID = loadImage("..\\common\\cube_map.png");
	RID cubeTextureID = loadImage("..\\common\\grid.png");
	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);
	
	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "..\\common\\Textured_vs.cso" },
		{ ds::ShaderType::PIXEL, "..\\common\\Textured_ps.cso" }
	};

	RID gridShader = ds::createShader(desc, 2);

	Grid grid;
	ds::vec3 gridPositions[] = {
		ds::vec3(-4.0f, -1.0f, -3.5f),
		ds::vec3(-4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f, -3.5f)
	};
	grid.create(gridPositions, 2, gridShader, textureID);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	RID rid = ds::createVertexDeclaration(decl, 2, gridShader);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBuffer = ds::createQuadIndexBuffer(256);
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), v);
	RID staticCubes = ds::createVertexBuffer(ds::BufferType::STATIC, totalCubeVertices, sizeof(Vertex), sv);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	ds::vec3 vp = ds::vec3(0.0f, 2.0f, -6.0f);
	ds::setViewPosition(vp);

	worldMatrix wm;

	ds::StateGroup* basicGroup = ds::createStateGroup();
	basicGroup->bindLayout(rid);
	basicGroup->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	basicGroup->bindBlendState(bs_id);
	basicGroup->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	basicGroup->bindTexture(cubeTextureID, ds::ShaderType::PIXEL, 0);
	basicGroup->bindShader(gridShader);
	basicGroup->bindIndexBuffer(indexBuffer);

	ds::StateGroup* staticGroup = ds::createStateGroup();
	staticGroup->bindVertexBuffer(staticCubes);

	ds::StateGroup* cubeGroup = ds::createStateGroup();
	cubeGroup->bindVertexBuffer(cubeBuffer);

	ds::DrawCommand staticCmd = { totalCubeVertices / 4 * 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	ds::StateGroup* staticStack[] = { basicGroup, staticGroup };
	ds::DrawItem* staticItem = ds::compile(staticCmd, staticStack, 2);

	ds::StateGroup* cubeStack[] = { basicGroup, cubeGroup };
	ds::DrawItem* cubeItem = ds::compile(drawCmd, cubeStack, 2);

	// prepare IMGUI
	SpriteBuffer spriteBuffer(512);
	RID guiTextureID = loadImage("..\\common\\imgui.png");
	gui::init(&spriteBuffer, guiTextureID);

	ds::vec3 rotation = ds::vec3(0.0f, 0.0f, 0.0f);

	int state = 1;
	while (ds::isRunning()) {
		ds::begin();

		grid.render();
		
		constantBuffer.viewprojectionMatrix = ds::matTranspose(ds::getViewProjectionMatrix());

		// spinning cube
		ds::matrix world = ds::matIdentity();
		constantBuffer.worldMatrix = ds::matTranspose(world);
		ds::submit(staticItem);

		//wm.rotateBy(ds::vec3(static_cast<float>(ds::getElapsedSeconds()), 2.0f  * static_cast<float>(ds::getElapsedSeconds()), 0.0f));
		wm.setRotation(rotation);
		constantBuffer.worldMatrix = wm.getTransposedMatrix();
		ds::submit(cubeItem);

		spriteBuffer.begin();
		gui::start(ds::vec2(0, 750));		
		gui::begin("Rotation", &state);
		if (state == 1) {
			gui::SliderAngle("Rotation X", &rotation.x);
			gui::SliderAngle("Rotation Y", &rotation.y);
			gui::SliderAngle("Rotation Z", &rotation.z);
		}
		gui::end();
		spriteBuffer.flush();

		ds::end();
	}
	ds::shutdown();
}