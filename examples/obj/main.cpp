#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "WaveFrontReader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Vertex {
	float x;
	float y;
	float z;
	float u;
	float v;

	Vertex() : x(0.0f), y(0.0f), z(0.0f), u(0.0f) , v(0.0f) {}
	Vertex(float xp, float yp, float zp, float uv,float vv) : x(xp), y(yp), z(zp), u(uv), v(vv) {}
	Vertex(const v3& p, float uv, float vv) : x(p.x), y(p.y), z(p.z), u(uv), v(vv) {}
};

const v3 CUBE_VERTICES[8] = {
	v3(-0.5f,-0.5f,-0.5f),
	v3(-0.5f, 0.5f,-0.5f),
	v3( 0.5f, 0.5f,-0.5f),
	v3( 0.5f,-0.5f,-0.5f),
	v3(-0.5f,-0.5f, 0.5f),
	v3(-0.5f, 0.5f, 0.5f),
	v3( 0.5f, 0.5f, 0.5f),
	v3( 0.5f,-0.5f, 0.5f)
};

const int CUBE_PLANES[6][4] = {
	{ 0, 1, 2, 3 }, // front
	{ 3, 2, 6, 7 }, // left
	{ 1, 5, 6, 2 }, // top
	{ 4, 5, 1, 0 }, // right
	{ 4, 0, 3, 7 }, // bottom
	{ 7, 6, 5, 4 }, // back
};

void addPlane(int side, Vertex* vertices, uint32_t* indices) {
	int idx = side * 4;
	float u[4] = { 0.0f,0.0f,1.0f,1.0f };
	float v[4] = { 1.0f,0.0f,0.0f,1.0f };
	for (int i = 0; i < 4; ++i) {
		int p = idx + i;
		vertices[p] = Vertex(CUBE_VERTICES[CUBE_PLANES[side][i]], u[i],v[i]);
	}
	int offset = side * 4;
	int ar[6] = { 0, 1, 2, 2, 3, 0 };
	for (int i = 0; i < 6; ++i) {
		indices[side * 6 + i] = offset + ar[i];
	}
}

struct CubeConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
};

int main(const char** args) {

	ObjVertex vertices[256];
	WaveFrontReader reader;
	int num = reader.load("cube.obj", vertices, 256);
	if ( num == -1) {
		printf("Cannot load file\n");
	}
	int q = num / 4;
	uint32_t* p_indices = new uint32_t[q*6];
	int ind[6] = { 0,1,2,2,3,0 };
	for (int i = 0; i < q; ++i) {
		for (int j = 0; j < 6; ++j) {
			p_indices[i * 6 + j] = i * 4 + ind[j];
		}
	}
	//Vertex v[24];
	//addPlane(0, v, p_indices);
	//addPlane(1, v, p_indices);
	//addPlane(2, v, p_indices);
	//addPlane(3, v, p_indices);
	//addPlane(4, v, p_indices);
	//addPlane(5, v, p_indices);

	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	if (ds::init(rs)) {

		int x, y, n;
		unsigned char *data = stbi_load("directx-11-logo.png", &x, &y, &n, 4);
		RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
		stbi_image_free(data);


		RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

		RID shaderID = ds::createShader();
		ds::loadVertexShader(shaderID, "Coloured_vs.cso");
		ds::loadPixelShader(shaderID, "Coloured_ps.cso");

		ds::VertexDeclaration decl[] = {
			{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
			{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 }
		};

		RID rid = ds::createVertexDeclaration(decl, 3, shaderID);
		RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
		RID iid = ds::createIndexBuffer(ds::BufferType::STATIC, p_indices,q*6);
		RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, num, 0, vertices,sizeof(ObjVertex));
		RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
		v3 vp = v3(0.0f, 0.0f, -6.0f);
		ds::setViewPosition(vp);
		v3 scale(1.0f, 1.0f, 1.0f);
		v3 rotation(0.0f, 0.0f, 0.0f);
		v3 pos(0.0f, 0.0f, 0.0f);
		
		while (ds::isRunning()) {
			ds::begin();
			//t += 0.001f;
			rotation.y += 0.0001f;
			rotation.x += 0.0001f;
			matrix world = mat_identity();
			matrix rotY = mat_RotationY(rotation.y);
			matrix rotX = mat_RotationX(rotation.x);
			matrix rotZ = mat_RotationZ(rotation.z);
			matrix s = mat_Scale(scale);
			matrix w = rotZ * rotY * rotX * s * world;
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;

			ds::setVertexBuffer(vbid, &stride, &offset, ds::PrimitiveTypes::LINE_LIST);
			ds::setVertexDeclaration(rid);
			//ds::setIndexBuffer(iid);
			ds::setBlendState(bs_id);
			ds::setShader(shaderID);
			ds::setSamplerState(ssid);
			ds::setTexture(textureID);
			constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
			constantBuffer.worldMatrix = mat_Transpose(w);
			ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
			ds::setVertexConstantBuffer(cbid);
			//ds::drawIndexed(q*6);
			ds::draw(num);
			ds::end();
		}
		ds::shutdown();
	}
}