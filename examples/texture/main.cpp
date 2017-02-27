#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "..\common\Grid.h"
#include "..\common\Geometry.h"
#include "..\common\WorldMatrix.h"

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	v3 p;
	v2 uv;

	Vertex() : p(0.0f), uv(0.0f) {}
	Vertex(const v3& pv, float u, float v) : p(pv) , uv(u,v) {}
	Vertex(const v3& pv, const v2& uvv) : p(pv), uv(uvv) {}
};

// ---------------------------------------------------------------
// the cube constant buffer
// ---------------------------------------------------------------
struct CubeConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
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
	std::vector<v3> vertCache;
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
	std::vector<v2> uvCache;
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
			const v3& p = vertCache[i];
			fprintf(fp, "v %g %g %g\n", p.x, p.y, p.z);
		}
		for (int i = 0; i < uvCache.size(); ++i) {
			const v2& p = uvCache[i];
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

	v3 positions[24];
	v2 uvs[24];
	matrix m = mat_identity();
	geometry::buildCube(m, positions, uvs);

	Vertex v[24];
	for (int i = 0; i < 24; ++i) {
		v[i] = Vertex(positions[i], uvs[i]);
	}

	v3 CP[] = { v3(-2,0,-1),v3(-2,0,3),v3(2,0,-1),v3(2,0,3) };
	const int numCubes = 4;
	const int totalCubeVertices = numCubes * 24;
	Vertex sv[totalCubeVertices];
	int cnt = 0;
	for(int j = 0; j < numCubes; ++j) {
		matrix m = mat_Translate(CP[j]);
		matrix s = mat_Scale(v3(1,2,1));
		matrix w = s * m;
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
	
	RID gridShader = ds::createShader();
	ds::loadVertexShader(gridShader, "..\\common\\Textured_vs.cso");
	ds::loadPixelShader(gridShader, "..\\common\\Textured_ps.cso");
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
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, rid, v,sizeof(Vertex));
	RID staticCubes = ds::createVertexBuffer(ds::BufferType::STATIC, totalCubeVertices, rid, sv, sizeof(Vertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	v3 vp = v3(0.0f, 2.0f, -6.0f);
	ds::setViewPosition(vp);

	WorldMatrix wm;
		
	while (ds::isRunning()) {
		ds::begin();

		grid.render();
			
		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;

		ds::setVertexDeclaration(rid);
		ds::setIndexBuffer(indexBuffer);
		ds::setBlendState(bs_id);
		ds::setShader(gridShader);
		ds::setSamplerState(ssid);
		constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());

		// spinning cube
		matrix world = mat_identity();
		constantBuffer.worldMatrix = mat_Transpose(world);
		ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
		ds::setConstantBuffer(cbid,ds::ShaderType::VERTEX);
		ds::setVertexBuffer(staticCubes, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
		ds::setTexture(cubeTextureID, ds::ShaderType::PIXEL);
		ds::drawIndexed(totalCubeVertices/4*6);

		wm.rotateBy(v3(static_cast<float>(ds::getElapsedSeconds()), 2.0f  * static_cast<float>(ds::getElapsedSeconds()), 0.0f));
		constantBuffer.worldMatrix = wm.getTransposedMatrix();
		ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
		ds::setConstantBuffer(cbid, ds::ShaderType::VERTEX);
		ds::setTexture(cubeTextureID, ds::ShaderType::PIXEL);
		ds::setVertexBuffer(cubeBuffer, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
		ds::drawIndexed(36);
		
		ds::end();
	}
	ds::shutdown();
}