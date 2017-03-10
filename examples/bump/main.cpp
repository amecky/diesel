#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\Grid.h"
#include "..\common\Geometry.h"
#include "..\common\worldMatrix.h"
#include "..\common\Camera.h"

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	ds::vec3 p;
	ds::vec2 uv;
	ds::vec3 n;
	ds::vec3 t;

	Vertex() : p(0.0f), uv(0.0f) , n(0.0f) , t(0.0f) {}
	Vertex(const ds::vec3& pv, const ds::vec2& uvv, const ds::vec3& nv,const ds::vec3& tv) : p(pv), uv(uvv) , n(nv), t(tv) {}
};

// ---------------------------------------------------------------
// the cube constant buffer
// ---------------------------------------------------------------
struct CubeConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
	ds::vec3 eyePos;
	float padding;
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

void calculateTangents(ds::vec3* positions, ds::vec2* uvs, ds::vec3* tangents, int num) {
	int steps = num / 4;
	for (int i = 0; i < steps; ++i) {
		// Shortcuts for vertices
		ds::vec3& vp0 = positions[i * 4 + 0];
		ds::vec3& vp1 = positions[i * 4 + 1];
		ds::vec3& vp2 = positions[i * 4 + 2];

		// Shortcuts for UVs
		ds::vec2 uv0 = uvs[i * 4 + 0];
		ds::vec2 uv1 = uvs[i * 4 + 1];
		ds::vec2 uv2 = uvs[i * 4 + 2];

		// Edges of the triangle : postion delta
		ds::vec3 deltaPos1 = vp1 - vp0;
		ds::vec3 deltaPos2 = vp2 - vp0;

		// UV delta
		ds::vec2 deltaUV1 = uv1 - uv0;
		ds::vec2 deltaUV2 = uv2 - uv0;
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		ds::vec3 tangent = normalize((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r);
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

	ds::vec3 positions[24];
	ds::vec3 normals[24];
	ds::vec3 tangents[24];
	ds::vec2 uvs[24];
	ds::matrix m = ds::matIdentity();
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
	RID cubeTextureID = loadImage("Hex.png", ds::TextureFormat::R8G8B8A8_UNORM_SRGB);
	RID cubeNormalID = loadImage("Hex_Normal.png", ds::TextureFormat::R8G8B8A8_UNORM_SRGB);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	ds::ShaderDescriptor texDesc[] = {
		{ ds::ShaderType::VERTEX, "..\\common\\Textured_vs.cso" },
		{ ds::ShaderType::PIXEL, "..\\common\\Textured_ps.cso" }
	};

	RID textureShader = ds::createShader(texDesc, 2);

	ds::ShaderDescriptor gridDesc[] = {
		{ ds::ShaderType::VERTEX, "Bump_vs.cso" },
		{ ds::ShaderType::PIXEL, "Bump_ps.cso" }
	};

	RID gridShader = ds::createShader(gridDesc, 2);

	Grid grid;
	ds::vec3 gridPositions[] = {
		ds::vec3(-4.0f, -1.0f, -3.5f),
		ds::vec3(-4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f, -3.5f)
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
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), v);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	ds::vec3 vp = ds::vec3(0.0f, 2.0f, -6.0f);
	ds::setViewPosition(vp);

	worldMatrix wm;

	FPSCamera camera(1024, 768);
	camera.setPosition(ds::vec3(0, 2, -12));

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(rid);
	sg->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	sg->bindConstantBuffer(cbid, ds::ShaderType::PIXEL, &constantBuffer);
	sg->bindBlendState(bs_id);
	sg->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	sg->bindVertexBuffer(cubeBuffer);
	sg->bindShader(gridShader);
	sg->bindIndexBuffer(indexBuffer);
	sg->bindTexture(cubeTextureID, ds::ShaderType::PIXEL, 0);
	sg->bindTexture(cubeNormalID, ds::ShaderType::PIXEL, 1);
	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
		
	while (ds::isRunning()) {
		ds::begin();

		camera.update(static_cast<float>(ds::getElapsedSeconds()));
		ds::matrix vpm = camera.getViewprojectionMatrix();

		grid.render();
			
		constantBuffer.viewprojectionMatrix = ds::matTranspose(vpm);

		//wm.rotateBy(ds::vec3(static_cast<float>(ds::getElapsedSeconds()), 2.0f  * static_cast<float>(ds::getElapsedSeconds()), 0.0f));
		wm.rotateBy(ds::vec3(0.0f, 1.0f  * static_cast<float>(ds::getElapsedSeconds()), 0.0f));
		constantBuffer.worldMatrix = wm.getTransposedMatrix();
		constantBuffer.eyePos = camera.getPosition();
		constantBuffer.padding = 0.0f;
		
		ds::submit(drawCmd, sg);

		ds::end();
	}
	ds::shutdown();
}