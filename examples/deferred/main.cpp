#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\Grid.h"
#include "..\common\Geometry.h"
#include "..\common\worldMatrix.h"

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	ds::vec3 p;
	ds::vec2 uv;
	ds::vec3 normal;
	ds::vec3 tangent;
	ds::vec3 binormal;

	Vertex() : p(0.0f), uv(0.0f) , normal(0.0f) , tangent(0.0f) , binormal(0.0f) {}
	Vertex(const ds::vec3& pv, const ds::vec2& uvv, const ds::vec3& n,const ds::vec3& t,const ds::vec3& b) : p(pv), uv(uvv) , normal(n) , tangent(t) , binormal(b) {}
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
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name, ds::TextureFormat format) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, format);
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

void buildRoom(Vertex* vertices, int max) {
	


}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	float sx = 2.0f;
	float sy = 2.0f;
	float sz = 2.0f;

	float uvMax = 1.0f;

	Vertex vertices[] = {
		// pos / uv / normal / tangent (u) / binormal (v)
		// floor
		{ ds::vec3(-sx, -sy, -sz), ds::vec2(0.0f,uvMax),  ds::vec3(0.0f,1.0f,0.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,-1.0f) },
		{ ds::vec3(-sx, -sy,  sz), ds::vec2(0.0f,0.0f),   ds::vec3(0.0f,1.0f,0.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,-1.0f) },
		{ ds::vec3( sx, -sy,  sz), ds::vec2(uvMax,0.0f),  ds::vec3(0.0f,1.0f,0.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,-1.0f) },
		{ ds::vec3( sx, -sy, -sz), ds::vec2(uvMax,uvMax), ds::vec3(0.0f,1.0f,0.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,-1.0f) },
		// left wall
		{ ds::vec3(-sx, -sy, -sz), ds::vec2(0.0f,uvMax),  ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,1.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		{ ds::vec3(-sx,  sy, -sz), ds::vec2(0.0f,0.0f),   ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,1.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		{ ds::vec3(-sx,  sy,  sz), ds::vec2(uvMax,0.0f),  ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,1.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		{ ds::vec3(-sx, -sy,  sz), ds::vec2(uvMax,uvMax), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,1.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		// back wall
		{ ds::vec3(-sx, -sy,  sz), ds::vec2(0.0f,uvMax),  ds::vec3(0.0f,0.0f,-1.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		{ ds::vec3(-sx,  sy,  sz), ds::vec2(0.0f,0.0f),   ds::vec3(0.0f,0.0f,-1.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		{ ds::vec3( sx,  sy,  sz), ds::vec2(uvMax,0.0f),  ds::vec3(0.0f,0.0f,-1.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		{ ds::vec3( sx, -sy,  sz), ds::vec2(uvMax,uvMax), ds::vec3(0.0f,0.0f,-1.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		// right wall
		{ ds::vec3(sx, -sy,  sz), ds::vec2(0.0f,uvMax),  ds::vec3(-1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,-1.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		{ ds::vec3(sx,  sy,  sz), ds::vec2(0.0f,0.0f),   ds::vec3(-1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,-1.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		{ ds::vec3(sx,  sy, -sz), ds::vec2(uvMax,0.0f),  ds::vec3(-1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,-1.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		{ ds::vec3(sx, -sy, -sz), ds::vec2(uvMax,uvMax), ds::vec3(-1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,-1.0f), ds::vec3(0.0f,-1.0f,0.0f) },
		// ceiling
		{ ds::vec3(-sx, sy,  sz), ds::vec2(0.0f,uvMax),  ds::vec3(0.0f,-1.0f,0.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,1.0f) },
		{ ds::vec3(-sx, sy, -sz), ds::vec2(0.0f,0.0f),   ds::vec3(0.0f,-1.0f,0.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,1.0f) },
		{ ds::vec3( sx, sy, -sz), ds::vec2(uvMax,0.0f),  ds::vec3(0.0f,-1.0f,0.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,1.0f) },
		{ ds::vec3( sx, sy,  sz), ds::vec2(uvMax,uvMax), ds::vec3(0.0f,-1.0f,0.0f), ds::vec3(1.0f,0.0f,0.0f), ds::vec3(0.0f,0.0f,1.0f) }
	};

	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 1;
	ds::init(rs);

	RID cubeTextureID = loadImage("..\\assets\\Hex.png", ds::TextureFormat::R8G8B8A8_UNORM_SRGB);
	RID cubeNormalID = loadImage("..\\assets\\Hex_Normal.png", ds::TextureFormat::R8G8B8A8_UNORM_SRGB);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);
	
	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "FillBuffer_vs.cso" },
		{ ds::ShaderType::PIXEL, "FillBuffer_ps.cso" }
	};

	RID fillShader = ds::createShader(desc, 2);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT, 3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT, 2 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT, 3 },
		{ ds::BufferAttribute::TANGENT,ds::BufferAttributeType::FLOAT, 3 },
		{ ds::BufferAttribute::BINORMAL,ds::BufferAttributeType::FLOAT, 3 }

	};

	RID rid = ds::createVertexDeclaration(decl, 5, fillShader);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBuffer = ds::createQuadIndexBuffer(256);
	RID roomBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), vertices);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::WRAP, ds::TextureFilters::LINEAR);

	ds::vec3 vp = ds::vec3(0.0f, 0.0f, -6.0f);
	ds::setViewPosition(vp);

	worldMatrix wm;

	ds::StateGroup* basicGroup = ds::createStateGroup();
	basicGroup->bindLayout(rid);
	basicGroup->bindShader(fillShader);
	basicGroup->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	basicGroup->bindBlendState(bs_id);
	basicGroup->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	basicGroup->bindTexture(cubeTextureID, ds::ShaderType::PIXEL, 0);
	basicGroup->bindTexture(cubeNormalID, ds::ShaderType::PIXEL, 1);
	basicGroup->bindShader(fillShader);
	basicGroup->bindIndexBuffer(indexBuffer);
	basicGroup->bindVertexBuffer(roomBuffer);

	ds::DrawCommand drawCmd = { 30, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	ds::DrawItem* roomItem = ds::compile(drawCmd, basicGroup);

	while (ds::isRunning()) {
		ds::begin();

		
		constantBuffer.viewprojectionMatrix = ds::matTranspose(ds::getViewProjectionMatrix());

		// spinning cube
		ds::matrix world = ds::matIdentity();
		constantBuffer.worldMatrix = ds::matTranspose(world);
		ds::submit(roomItem);

		ds::end();
	}
	ds::shutdown();
}