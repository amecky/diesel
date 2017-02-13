#define DS_IMPLEMENTATION
#include "..\..\diesel.h"

struct Vertex {
	float x;
	float y;
	float z;
	ds::Color color;

	Vertex() : x(0.0f), y(0.0f), z(0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	Vertex(float xp, float yp, float zp, const ds::Color& c) : x(xp), y(yp), z(zp), color(c) {}
	Vertex(const v3& p, const ds::Color& c) : x(p.x), y(p.y), z(p.z), color(c) {}
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

void addPlane(int side, const ds::Color& color, Vertex* vertices, uint32_t* indices) {
	int idx = side * 4;
	for (int i = 0; i < 4; ++i) {
		int p = idx + i;
		vertices[p] = Vertex(CUBE_VERTICES[CUBE_PLANES[side][i]], color);
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

	uint32_t p_indices[36];
	Vertex v[24];
	addPlane(0, ds::Color(1.0f, 0.0f, 0.0f, 1.0f), v, p_indices);
	addPlane(1, ds::Color(0.0f, 1.0f, 0.0f, 1.0f), v, p_indices);
	addPlane(2, ds::Color(0.0f, 0.0f, 1.0f, 1.0f), v, p_indices);
	addPlane(3, ds::Color(1.0f, 0.0f, 1.0f, 1.0f), v, p_indices);
	addPlane(4, ds::Color(1.0f, 1.0f, 0.0f, 1.0f), v, p_indices);
	addPlane(5, ds::Color(0.0f, 1.0f, 1.0f, 1.0f), v, p_indices);

	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	if (ds::init(rs)) {

		RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

		RID shaderID = ds::createShader();
		ds::loadVertexShader(shaderID, "Coloured_vs.cso");
		ds::loadPixelShader(shaderID, "Coloured_ps.cso");

		ds::VertexDeclaration decl[] = {
			{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
		};

		RID rid = ds::createVertexDeclaration(decl, 2, shaderID);
		RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
		RID iid = ds::createIndexBuffer(36, ds::IndexType::UINT_32, ds::BufferType::STATIC, p_indices);
		RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, 24, 0, v,sizeof(Vertex));
		RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
		v3 vp = v3(2.0f, 2.0f, -6.0f);
		ds::setViewPosition(vp);
		//ds::look_at(v3(2.0f, 0.0f, 0.0f));
		v3 scale(1.0f, 1.0f, 1.0f);
		v3 rotation(0.0f, 0.0f, 0.0f);
		v3 pos(0.0f, 0.0f, 0.0f);
		
		while (ds::isRunning()) {
			ds::begin();
			t += 0.001f;
			//rotation.y += 0.0001f;
			//rotation.x += 0.0001f;
			vp.y = 2.0f * sin(t);
			ds::setViewPosition(vp);
			pos.z = sin(t);
			matrix world = mat_identity();
			matrix w = mat_identity();
			//world = mat_Translate(pos);
			matrix rotY = mat_RotationY(rotation.y);
			matrix rotX = mat_RotationX(rotation.x);
			matrix rotZ = mat_RotationZ(rotation.z);
			matrix s = mat_Scale(scale);
			w = rotZ * rotY * rotX * s * world;
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;

			ds::setVertexBuffer(vbid, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
			ds::setVertexDeclaration(rid);
			ds::setIndexBuffer(iid);
			ds::setBlendState(bs_id);
			ds::setShader(shaderID);
			constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
			constantBuffer.worldMatrix = mat_Transpose(w);
			ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
			ds::setVertexConstantBuffer(cbid);
			ds::drawIndexed(36);
			ds::end();
		}
		ds::shutdown();
	}
}