#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "..\common\Grid.h"
#include "..\common\Geometry.h"
#include "..\common\WorldMatrix.h"
#include "..\common\Camera.h"

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	v3 p;
	v2 uv;
	v3 n;
	v3 t;

	Vertex() : p(0.0f), uv(0.0f) , n(0.0f) , t(0.0f) {}
	Vertex(const v3& pv, const v2& uvv, const v3& nv,const v3& tv) : p(pv), uv(uvv) , n(nv), t(tv) {}
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
RID loadImage(const char* name,ds::TextureFormat format) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, format);
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

void calculateTangents(v3* positions, v2* uvs, v3* tangents, int num) {
	int steps = num / 4;
	for (int i = 0; i < steps; ++i) {
		// Shortcuts for vertices
		v3& vp0 = positions[i * 4 + 0];
		v3& vp1 = positions[i * 4 + 1];
		v3& vp2 = positions[i * 4 + 2];

		// Shortcuts for UVs
		v2 uv0 = uvs[i * 4 + 0];
		v2 uv1 = uvs[i * 4 + 1];
		v2 uv2 = uvs[i * 4 + 2];

		// Edges of the triangle : postion delta
		v3 deltaPos1 = vp1 - vp0;
		v3 deltaPos2 = vp2 - vp0;

		// UV delta
		v2 deltaUV1 = uv1 - uv0;
		v2 deltaUV2 = uv2 - uv0;
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		v3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		tangents[i * 4] = tangent;
		tangents[i * 4 + 1] = tangent;
		tangents[i * 4 + 2] = tangent;
		tangents[i * 4 + 3] = tangent;
	}
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	v3 positions[24];
	v3 normals[24];
	v3 tangents[24];
	v2 uvs[24];
	matrix m = mat_identity();
	geometry::buildCube(m, positions, uvs, normals);
	calculateTangents(positions, uvs, tangents, 24);
	Vertex v[24];
	for (int i = 0; i < 24; ++i) {
		v[i] = Vertex(positions[i], uvs[i], normals[i],tangents[i]);
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

	RID textureID = loadImage("..\\common\\cube_map.png", ds::TextureFormat::R8G8B8A8_UNORM);
	RID cubeTextureID = loadImage("fieldstone-rgba.tga", ds::TextureFormat::R8G8B8A8_UNORM);
	RID cubeNormalID = loadImage("fieldstone-n.tga", ds::TextureFormat::R8G8B8A8_UNORM);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);
	
	RID textureShader = ds::createShader();
	ds::loadVertexShader(textureShader, "..\\common\\Textured_vs.cso");
	ds::loadPixelShader(textureShader, "..\\common\\Textured_ps.cso");

	RID gridShader = ds::createShader();
	ds::loadVertexShader(gridShader, "Bump_vs.cso");
	ds::loadPixelShader(gridShader, "Bump_ps.cso");
	Grid grid;
	v3 gridPositions[] = {
		v3(-4.0f, -1.0f, -3.5f),
		v3(-4.0f, -1.0f,  4.5f),
		v3(4.0f, -1.0f,  4.5f),
		v3(4.0f, -1.0f, -3.5f)
	};
	grid.create(gridPositions, 2, textureShader, textureID);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TANGENT,ds::BufferAttributeType::FLOAT,3 }
	};

	RID rid = ds::createVertexDeclaration(decl, 4, gridShader);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBuffer = ds::createQuadIndexBuffer(256);
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, rid, v,sizeof(Vertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	v3 vp = v3(0.0f, 2.0f, -6.0f);
	ds::setViewPosition(vp);

	WorldMatrix wm;

	FPSCamera camera(1024, 768);
	camera.setPosition(v3(0, 2, -12));
		
	while (ds::isRunning()) {
		ds::begin();

		camera.update(static_cast<float>(ds::getElapsedSeconds()));
		matrix vpm = camera.getViewProjectionMatrix();

		grid.render(&vpm);
			
		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;

		ds::setVertexDeclaration(rid);
		ds::setIndexBuffer(indexBuffer);
		ds::setBlendState(bs_id);
		ds::setShader(gridShader);
		ds::setSamplerState(ssid);
		constantBuffer.viewProjectionMatrix = mat_Transpose(vpm);

		//wm.rotateBy(v3(static_cast<float>(ds::getElapsedSeconds()), 2.0f  * static_cast<float>(ds::getElapsedSeconds()), 0.0f));
		constantBuffer.worldMatrix = wm.getTransposedMatrix();
		ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
		ds::setConstantBuffer(cbid, ds::ShaderType::VERTEX);
		ds::setTexture(cubeTextureID, ds::ShaderType::PIXEL, 0);
		ds::setTexture(cubeNormalID, ds::ShaderType::PIXEL, 1);
		ds::setVertexBuffer(cubeBuffer, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
		ds::drawIndexed(36);
		
		ds::end();
	}
	ds::shutdown();
}