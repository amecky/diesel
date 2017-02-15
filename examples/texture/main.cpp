#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Rect {
	float top;
	float left;
	float width;
	float height;

	Rect() : top(0.0f), left(0.0f), width(0.0f), height(0.0f) {}
	Rect(float t, float l, float w, float h) : top(t), left(l), width(w), height(h) {}

};

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

void addPlane(int index, int side, Vertex* vertices, const v3& offset = v3(0.0f), const v4& uvs = v4(0.0f,0.0f,1.0f,1.0f)) {
	int idx = index * 4;
	float u[4] = { uvs.x,uvs.x,uvs.z,uvs.z };
	float v[4] = { uvs.w,uvs.y,uvs.y,uvs.w };
	for (int i = 0; i < 4; ++i) {
		int p = idx + i;
		vertices[p] = Vertex(CUBE_VERTICES[CUBE_PLANES[side][i]] + offset, u[i],v[i]);
	}	
}

struct CubeConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
};

RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);
	return textureID;
}

v4 convertTextureRect(const Rect& r, float size) {
	v4 ret;
	ret.x = r.left / size; // u0
	ret.y = r.top / size; // v0
	ret.z = (r.left + r.width) / size; // u1
	ret.w = (r.top + r.height) / size; // v1
	return ret;
}

//int main(const char** args) {
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	Vertex v[24];
	addPlane(0, 0, v, v3(0.0f), convertTextureRect(Rect(0, 0, 64, 64),512.0f));
	addPlane(1, 1, v, v3(0.0f), convertTextureRect(Rect(64, 0, 64, 64), 512.0f));
	addPlane(2, 2, v, v3(0.0f), convertTextureRect(Rect(128, 0, 64, 64), 512.0f));
	addPlane(3, 3, v, v3(0.0f), convertTextureRect(Rect(192, 0, 64, 64), 512.0f));
	addPlane(4, 4, v, v3(0.0f), convertTextureRect(Rect(0, 64, 64, 64), 512.0f));
	addPlane(5, 5, v, v3(0.0f), convertTextureRect(Rect(64, 64, 64, 64), 512.0f));

	Vertex floorVertices[4];
	floorVertices[0] = Vertex(v3(-3.0f, -1.0f, -3.0f), 0.0f, 1.0f);
	floorVertices[1] = Vertex(v3(-3.0f, -1.0f,  3.0f), 0.0f, 0.0f);
	floorVertices[2] = Vertex(v3( 3.0f, -1.0f,  3.0f), 1.0f, 0.0f);
	floorVertices[3] = Vertex(v3( 3.0f, -1.0f, -3.0f), 1.0f, 1.0f);
	
	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 1;
	if (ds::init(rs)) {

		RID textureID = loadImage("cube_map.png");
		RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);
		RID shaderID = ds::createShader();
		ds::loadVertexShader(shaderID, "Coloured_vs.cso");
		ds::loadPixelShader(shaderID, "Coloured_ps.cso");

		ds::VertexDeclaration decl[] = {
			{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
		};

		RID rid = ds::createVertexDeclaration(decl, 2, shaderID);
		RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
		RID indexBuffer = ds::createQuadIndexBuffer(36);
		RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, 0, v,sizeof(Vertex));
		RID floorBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, 0, floorVertices, sizeof(Vertex));
		RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
		v3 vp = v3(0.0f, 2.0f, -6.0f);
		ds::setViewPosition(vp);
		v3 scale(1.0f, 1.0f, 1.0f);
		v3 rotation(0.0f, 0.0f, 0.0f);
		v3 pos(0.0f, 0.0f, 0.0f);
		
		while (ds::isRunning()) {
			ds::begin();
			
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;

			ds::setVertexDeclaration(rid);
			ds::setIndexBuffer(indexBuffer);
			ds::setBlendState(bs_id);
			ds::setShader(shaderID);
			ds::setSamplerState(ssid);
			constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
			
			// floor
			matrix world = mat_identity();
			constantBuffer.worldMatrix = mat_Transpose(world);
			ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
			ds::setTexture(textureID, ds::ShaderType::PIXEL);
			ds::setVertexBuffer(floorBuffer, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
			ds::drawIndexed(6);

			world = mat_identity();
			//t += 0.001f;
			rotation.y += 0.001f;
			rotation.x += 0.001f;
			matrix rotY = mat_RotationY(rotation.y);
			matrix rotX = mat_RotationX(rotation.x);
			matrix rotZ = mat_RotationZ(rotation.z);
			matrix s = mat_Scale(scale);
			matrix w = rotZ * rotY * rotX * s * world;
			constantBuffer.worldMatrix = mat_Transpose(w);
			ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
			ds::setVertexConstantBuffer(cbid);
			ds::setTexture(textureID, ds::ShaderType::PIXEL);
			ds::setVertexBuffer(cubeBuffer, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
			ds::drawIndexed(36);
			ds::end();
		}
		ds::shutdown();
	}
}