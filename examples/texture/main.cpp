#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "..\common\Grid.h"
#include "..\common\Geometry.h"

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	float x;
	float y;
	float z;
	float u;
	float v;

};

// ---------------------------------------------------------------
// the cube constant buffer
// ---------------------------------------------------------------
struct CubeConstantBuffer {
	float viewProjectionMatrix[16];
	float worldMatrix[16];
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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	v3 positions[24];
	v2 uvs[24];
	float m[16];
	ds::matIdentity(m);
	geometry::buildCube(m, positions, uvs);

	Vertex v[24];
	for (int i = 0; i < 24; ++i) {
		v[i] = { positions[i].x, positions[i].y, positions[i].z, uvs[i].x,uvs[i].y };
	}

	const int numCubes = 4;
	float CP[numCubes][3] = { {-2,0,-1},{-2,0,3},{2,0,-1},{2,0,3} };
	
	const int totalCubeVertices = numCubes * 24;
	Vertex sv[totalCubeVertices];
	int cnt = 0;
	float s[16];
	ds::matScale(s, 0.5f, 2.0f, 0.5f);
	for(int j = 0; j < numCubes; ++j) {
		ds::matTranslate(m, CP[j]);
		ds::matMultiply(m, s, m);
		geometry::buildCube(m, positions, uvs);
		for (int i = 0; i < 24; ++i) {
			sv[cnt++] = { positions[i].x,positions[i].y,positions[i].z, uvs[i].x,uvs[i].y };
		}
	}
	
	CubeConstantBuffer constantBuffer;
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
	v3 gridPositions[] = {
		v3(-4.0f, -1.0f, -3.5f),
		v3(-4.0f, -1.0f,  4.5f),
		v3(4.0f, -1.0f,  4.5f),
		v3(4.0f, -1.0f, -3.5f)
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
	float vp[3] = { 0.0f, 2.0f, -6.0f };
	ds::setViewPosition(vp);

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
	
	float t = 0.0f;

	float vpm[16];
	ds::getViewProjectionMatrix(vpm);
	ds::matTranspose(constantBuffer.viewProjectionMatrix, vpm);

	float world[16];
	ds::matIdentity(world);

	while (ds::isRunning()) {
		ds::begin();

		grid.render();

		ds::matIdentity(world);
		// spinning cube
		ds::matTranspose(constantBuffer.worldMatrix, world);
		ds::submit(staticItem);

		t += static_cast<float>(ds::getElapsedSeconds());
		ds::matSRT(world, 1.0f, 1.0f, 1.0f, t, 2.0f * t, 0.0f, 0.0f, 0.0f, 0.0f);
		ds::matTranspose(constantBuffer.worldMatrix, world);
		ds::submit(cubeItem);
		
		ds::end();
	}
	ds::shutdown();
}