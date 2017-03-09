#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "..\common\math.h"

struct Vertex {
	v3 p;
	v2 t;
	v3 n;

	Vertex() : p(0.0f), n(0.0f), t(0.0f) {}
	Vertex(const v3& pv, const v3& nv, const v2& tv) :p(pv), n(nv), t(tv) {}
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

void addPlane(int index, int side, Vertex* vertices, const float* world) {
	int idx = index * 4;
	float pos[3];
	float u[4] = { 0.0f,0.0f,1.0f,1.0f };
	float v[4] = { 1.0f,0.0f,0.0f,1.0f };
	for (int i = 0; i < 4; ++i) {
		int p = idx + i;
		ds::matVec3Multiply(pos, world, CUBE_VERTICES[CUBE_PLANES[side][i]]());
		vertices[p] = Vertex(v3(pos[0],pos[1],pos[2]), v3(0.0f), v2(u[i],v[i]));
	}	
	v3 d0 = vertices[idx + 1].p - vertices[idx].p;
	v3 d1 = vertices[idx + 2].p - vertices[idx].p;
	v3 c = cross(d0, d1);
	for (int i = 0; i < 4; ++i) {
		vertices[idx + i].n = c;
	}
}

struct CubeConstantBuffer {
	float viewProjectionMatrix[16];
	float worldMatrix[16];
};

struct LightBuffer {
	float ambientColor[4];
	float diffuseColor[4];
	float lightDirection[3];
	float padding;
};

RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);
	return textureID;
}

int main(const char** args) {
	float w[16];
	ds::matIdentity(w);
	Vertex v[24];
	addPlane(0, 0, v, w);
	addPlane(1, 1, v, w);
	addPlane(2, 2, v, w);
	addPlane(3, 3, v, w);
	addPlane(4, 4, v, w);
	addPlane(5, 5, v, w);

	float s[16];
	ds::matScale(s,0.2f,0.2f,0.2f);
	float lw[16];
	ds::matMultiply(lw, s, w);
	Vertex lv[24];
	addPlane(0, 0, lv, lw);
	addPlane(1, 1, lv, lw);
	addPlane(2, 2, lv, lw);
	addPlane(3, 3, lv, lw);
	addPlane(4, 4, lv, lw);
	addPlane(5, 5, lv, lw);

	Vertex floorVertices[4];
	floorVertices[0] = Vertex(v3(-3.0f, -1.0f, -2.5f), v3(0.0f,1.0f,0.0f),v2(0.0f, 1.0f));
	floorVertices[1] = Vertex(v3(-3.0f, -1.0f, 3.5f), v3(0.0f, 1.0f, 0.0f), v2(0.0f, 0.0f));
	floorVertices[2] = Vertex(v3(3.0f, -1.0f, 3.5f), v3(0.0f, 1.0f, 0.0f), v2(1.0f, 0.0f));
	floorVertices[3] = Vertex(v3(3.0f, -1.0f, -2.5f), v3(0.0f, 1.0f, 0.0f), v2(1.0f, 1.0f));

	CubeConstantBuffer constantBuffer;
	LightBuffer lightBuffer;
	ds::vec4(lightBuffer.ambientColor,0.15f, 0.15f, 0.15f,1.0f);
	ds::vec4(lightBuffer.diffuseColor,1.0f, 1.0f, 1.0f, 1.0f);
	lightBuffer.padding = 0.0f;
	float lightPos[3] = { 1.0f, -0.5f, 1.0f };
	ds::vec3(lightBuffer.lightDirection,lightPos);

	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	RID textureID = loadImage("directx-11-logo.png");
	RID floorTexture = loadImage("..\\common\\cube_map.png");


	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "AmbientLightning_vs.cso" },
		{ ds::ShaderType::PIXEL, "AmbientLightning_ps.cso" }
	};

	RID shaderID = ds::createShader(desc, 2);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 }			
	};

	RID rid = ds::createVertexDeclaration(decl, 3, shaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer));
	RID indexBuffer = ds::createQuadIndexBuffer(36);
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), v);
	RID floorBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, sizeof(Vertex), floorVertices);
	RID bulbID = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), lv);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	float vp[3] = { 0.0f, 2.0f, -6.0f };
	ds::setViewPosition(vp);
	float scale[3] = { 1.0f, 1.0f, 1.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float pos[3] = { 0.0f, 0.0f, 0.0f };

	ds::StateGroup* basicGroup = ds::createStateGroup();
	basicGroup->bindLayout(rid);
	basicGroup->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	basicGroup->bindConstantBuffer(lightBufferID, ds::ShaderType::PIXEL, &lightBuffer);
	basicGroup->bindBlendState(bs_id);
	basicGroup->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	basicGroup->bindShader(shaderID);
	basicGroup->bindIndexBuffer(indexBuffer);

	ds::StateGroup* floorGroup = ds::createStateGroup();
	floorGroup->bindTexture(floorTexture, ds::ShaderType::PIXEL, 0);
	floorGroup->bindVertexBuffer(floorBuffer);

	ds::StateGroup* ambientGroup = ds::createStateGroup();	
	ambientGroup->bindTexture(textureID, ds::ShaderType::PIXEL, 0);
	ambientGroup->bindVertexBuffer(cubeBuffer);

	ds::StateGroup* bulbGroup = ds::createStateGroup();
	bulbGroup->bindTexture(floorTexture, ds::ShaderType::PIXEL, 0);
	bulbGroup->bindVertexBuffer(bulbID);

	ds::DrawCommand floorCmd = { 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
		
	ds::StateGroup* floorStack[] = { basicGroup, floorGroup };
	ds::DrawItem* floorItem = ds::compile(floorCmd, floorStack , 2);

	ds::StateGroup* cubeStack[] = { basicGroup, ambientGroup };
	ds::DrawItem* cubeItem = ds::compile(drawCmd, cubeStack, 2);

	ds::StateGroup* bulbStack[] = { basicGroup, bulbGroup };
	ds::DrawItem* bulbItem = ds::compile(drawCmd, bulbStack, 2);

	float world[16];
	float vpm[16];
	while (ds::isRunning()) {
		ds::begin();

		t += static_cast<float>(ds::getElapsedSeconds());

		ds::getViewProjectionMatrix(vpm);
		ds::matTranspose(constantBuffer.viewProjectionMatrix, vpm);
			
		// floor
		ds::matIdentity(world);
		ds::matTranspose(constantBuffer.worldMatrix, world);
		ds::submit(floorItem);
		
		// draw the light as small cube
		lightPos[0] = cos(t);
		lightPos[2] = sin(t);
		ds::vec3(lightBuffer.lightDirection, lightPos);
		float lp[3] = { -lightPos[0], -lightPos[1], -lightPos[2] };
		float by[16];
		ds::matRotationY(by,t);
		ds::matTranslate(world, lp);
		ds::matMultiply(world, by, world);
		ds::matTranspose(constantBuffer.worldMatrix, world);
		ds::submit(bulbItem);
		
		// spinning cube
		rotation[1] += 2.0f  * static_cast<float>(ds::getElapsedSeconds());
		rotation[0] += 1.0f  * static_cast<float>(ds::getElapsedSeconds());
		ds::matSRT(world, scale[0], scale[1], scale[2], rotation[0], rotation[1], rotation[2], pos[0], pos[1], pos[2]);
		
		ds::matTranspose(constantBuffer.worldMatrix, world);
		ds::submit(cubeItem);

		ds::end();
	}
	ds::shutdown();
}