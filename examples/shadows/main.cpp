#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Vertex {
	v3 p;	
	v3 n;
	ds::Color color;

	Vertex() : p(0.0f), n(0.0f), color(1.0f,1.0f,1.0f,1.0f) {}
	Vertex(const v3& pv, const v3& nv, const ds::Color& c) : p(pv), n(nv), color(c) {}
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

void addPlane(int index, int side, Vertex* vertices, const matrix& w, const ds::Color& color = ds::Color(0.7f, 0.0f, 0.0f, 1.0f)) {
	int idx = index * 4;
	float u[4] = { 0.0f,0.0f,1.0f,1.0f };
	float v[4] = { 1.0f,0.0f,0.0f,1.0f };
	for (int i = 0; i < 4; ++i) {
		int p = idx + i;
		v3 pos = w * CUBE_VERTICES[CUBE_PLANES[side][i]];
		vertices[p] = Vertex(pos, v3(0.0f), color);
	}	
	v3 d0 = vertices[idx + 1].p - vertices[idx].p;
	v3 d1 = vertices[idx + 2].p - vertices[idx].p;
	v3 c = cross(d0, d1);
	for (int i = 0; i < 4; ++i) {
		vertices[idx + i].n = c;
	}
}

void addCube(Vertex* vertices, const v3& pos,const v3& scale, const ds::Color& color = ds::Color(0.7f, 0.0f, 0.0f, 1.0f)) {
	matrix world = mat_identity();
	world = mat_Translate(pos);
	matrix rotY = mat_RotationY(0.0f);
	matrix rotX = mat_RotationX(0.0f);
	matrix rotZ = mat_RotationZ(0.0f);
	matrix s = mat_Scale(scale);
	matrix w = rotZ * rotY * rotX * s * world;
	addPlane(0, 0, vertices, w, color);
	addPlane(1, 1, vertices, w, color);
	addPlane(2, 2, vertices, w, color);
	addPlane(3, 3, vertices, w, color);
	addPlane(4, 4, vertices, w, color);
	addPlane(5, 5, vertices, w, color);
}

struct CubeConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
};

struct LightBuffer {
	ds::Color ambientColor;
	ds::Color diffuseColor;
	v3 lightDirection;
	float padding;
};

RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);
	return textureID;
}

//int main(const char** args) {
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	const int numCubes = 4;
	const int numVertices = 24 * numCubes;
	Vertex v[numVertices];
	Vertex* current = v;
	addCube(current, v3(0.0f, -0.2f, 0.0f), v3(5.0f, 0.1f, 5.0f),ds::Color(0.0f,0.7f,0.0f,1.0f));
	current += 24;
	for (int i = 1; i < numCubes; ++i) {
		addCube(current, v3(-1.0f + 1.0f* (i-1), 0.2f, -1.0f), v3(0.4f, 0.4f, 0.4f));
		current += 24;
	}

	CubeConstantBuffer constantBuffer;
	LightBuffer lightBuffer;
	lightBuffer.ambientColor = ds::Color(0.15f, 0.15f, 0.15f,1.0f);
	lightBuffer.diffuseColor = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
	lightBuffer.padding = 0.0f;
	lightBuffer.lightDirection = v3(-0.5f, -0.5f, 1.0f);

	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	if (ds::init(rs)) {

		//RID textureID = loadImage("directx-11-logo.png");
		//RID floorTexture = loadImage("Finishes.Flooring.Tile.Square.Grey.Dark.bump.jpg");


		RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

		RID shaderID = ds::createShader();
		ds::loadVertexShader(shaderID, "AmbientLightning_vs.cso");
		ds::loadPixelShader(shaderID, "AmbientLightning_ps.cso");

		ds::VertexDeclaration decl[] = {
			{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }		
		};
		RID rasterizerStateID = ds::createRasterizerState(ds::CullMode::BACK, ds::FillMode::SOLID, true, false, 0.0f, 0.0f);
		RID rid = ds::createVertexDeclaration(decl, 3, shaderID);
		RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
		RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer));
		RID indexBuffer = ds::createQuadIndexBuffer(36 * numCubes);
		RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, numVertices, 0, v,sizeof(Vertex));
		RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
		v3 vp = v3(0.0f, 2.0f, -6.0f);
		ds::setViewPosition(vp);
		//v3 scale(1.0f, 1.0f, 1.0f);
		//v3 rotation(0.0f, 0.0f, 0.0f);
		//v3 pos(0.0f, 0.0f, 0.0f);
		
		while (ds::isRunning()) {
			ds::begin();
			
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;

			ds::setVertexDeclaration(rid);
			ds::setIndexBuffer(indexBuffer);
			ds::setBlendState(bs_id);
			ds::setShader(shaderID);
			ds::setSamplerState(ssid);
			ds::setRasterizerState(rasterizerStateID);
			constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
			
			matrix world = mat_identity();
			//t += 0.001f;
			//rotation.y += 0.0001f;
			//rotation.x += 0.0001f;
			//matrix rotY = mat_RotationY(rotation.y);
			//matrix rotX = mat_RotationX(rotation.x);
			//matrix rotZ = mat_RotationZ(rotation.z);
			//matrix s = mat_Scale(scale);
			//matrix w = rotZ * rotY * rotX * s * world;
			constantBuffer.worldMatrix = mat_Transpose(world);
			ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
			ds::updateConstantBuffer(lightBufferID, &lightBuffer, sizeof(LightBuffer));
			ds::setConstantBuffer(cbid, ds::ShaderType::VERTEX);
			ds::setConstantBuffer(lightBufferID, ds::ShaderType::PIXEL);
			//ds::setTexture(textureID, ds::ShaderType::PIXEL);
			ds::setVertexBuffer(cubeBuffer, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
			ds::drawIndexed(36 * numCubes);
			ds::end();
		}
		ds::shutdown();
	}
}