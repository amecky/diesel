#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"

struct Vertex {
	ds::vec3 p;
	ds::vec2 t;
	ds::vec3 n;

	Vertex() : p(0.0f), n(0.0f), t(0.0f) {}
	Vertex(const ds::vec3& pv, const ds::vec3& nv, const ds::vec2& tv) :p(pv), n(nv), t(tv) {}
};

const ds::vec3 CUBE_VERTICES[8] = {
	ds::vec3(-0.5f,-0.5f,-0.5f),
	ds::vec3(-0.5f, 0.5f,-0.5f),
	ds::vec3( 0.5f, 0.5f,-0.5f),
	ds::vec3( 0.5f,-0.5f,-0.5f),
	ds::vec3(-0.5f,-0.5f, 0.5f),
	ds::vec3(-0.5f, 0.5f, 0.5f),
	ds::vec3( 0.5f, 0.5f, 0.5f),
	ds::vec3( 0.5f,-0.5f, 0.5f)
};

const int CUBE_PLANES[6][4] = {
	{ 0, 1, 2, 3 }, // front
	{ 3, 2, 6, 7 }, // left
	{ 1, 5, 6, 2 }, // top
	{ 4, 5, 1, 0 }, // right
	{ 4, 0, 3, 7 }, // bottom
	{ 7, 6, 5, 4 }, // back
};

void addPlane(int index, int side, Vertex* vertices, const ds::matrix& world) {
	int idx = index * 4;
	float u[4] = { 0.0f,0.0f,1.0f,1.0f };
	float v[4] = { 1.0f,0.0f,0.0f,1.0f };
	for (int i = 0; i < 4; ++i) {
		int p = idx + i;
		ds::vec3 pos = world * CUBE_VERTICES[CUBE_PLANES[side][i]];
		vertices[p] = Vertex(pos, ds::vec3(0.0f), ds::vec2(u[i],v[i]));
	}	
	ds::vec3 d0 = vertices[idx + 1].p - vertices[idx].p;
	ds::vec3 d1 = vertices[idx + 2].p - vertices[idx].p;
	ds::vec3 c = cross(d0, d1);
	for (int i = 0; i < 4; ++i) {
		vertices[idx + i].n = c;
	}
}

struct CubeConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
};

struct LightBuffer {
	ds::Color ambientColor;
	ds::Color diffuseColor;
	ds::vec3 lightDirection;
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
	ds::matrix w = ds::matIdentity();
	Vertex v[24];
	addPlane(0, 0, v, w);
	addPlane(1, 1, v, w);
	addPlane(2, 2, v, w);
	addPlane(3, 3, v, w);
	addPlane(4, 4, v, w);
	addPlane(5, 5, v, w);

	ds::matrix s = ds::matScale(ds::vec3(0.2f,0.2f,0.2f));
	ds::matrix lw = s * w;
	Vertex lv[24];
	addPlane(0, 0, lv, lw);
	addPlane(1, 1, lv, lw);
	addPlane(2, 2, lv, lw);
	addPlane(3, 3, lv, lw);
	addPlane(4, 4, lv, lw);
	addPlane(5, 5, lv, lw);

	Vertex floorVertices[4];
	floorVertices[0] = Vertex(ds::vec3(-3.0f, -1.0f, -2.5f), ds::vec3(0.0f,1.0f,0.0f),ds::vec2(0.0f, 1.0f));
	floorVertices[1] = Vertex(ds::vec3(-3.0f, -1.0f, 3.5f), ds::vec3(0.0f, 1.0f, 0.0f), ds::vec2(0.0f, 0.0f));
	floorVertices[2] = Vertex(ds::vec3(3.0f, -1.0f, 3.5f), ds::vec3(0.0f, 1.0f, 0.0f), ds::vec2(1.0f, 0.0f));
	floorVertices[3] = Vertex(ds::vec3(3.0f, -1.0f, -2.5f), ds::vec3(0.0f, 1.0f, 0.0f), ds::vec2(1.0f, 1.0f));

	CubeConstantBuffer constantBuffer;
	LightBuffer lightBuffer;
	lightBuffer.ambientColor = ds::Color(0.15f, 0.15f, 0.15f,1.0f);
	lightBuffer.diffuseColor = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
	lightBuffer.padding = 0.0f;
	ds::vec3 lightPos = ds::vec3(1.0f, -0.5f, 1.0f);
	lightBuffer.lightDirection = normalize(lightPos);

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

	RID vertexShader = ds::loadVertexShader("AmbientLightning_vs.cso");
	RID pixelShader = ds::loadPixelShader("AmbientLightning_ps.cso");

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 2.0f, -6.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	RID basicPass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::ENABLED);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 }			
	};

	RID rid = ds::createVertexDeclaration(decl, 3, vertexShader);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);
	RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer), &lightBuffer);
	RID indexBuffer = ds::createQuadIndexBuffer(36);
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), v);
	RID floorBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, sizeof(Vertex), floorVertices);
	RID bulbID = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), lv);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	ds::vec3 scale(1.0f, 1.0f, 1.0f);
	ds::vec3 rotation(0.0f, 0.0f, 0.0f);
	ds::vec3 pos(0.0f, 0.0f, 0.0f);

	RID basicGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.constantBuffer(cbid, vertexShader, 0)
		.constantBuffer(lightBufferID, pixelShader, 0)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.indexBuffer(indexBuffer)
		.build();

	RID floorGroup = ds::StateGroupBuilder()
		.texture(floorTexture, pixelShader, 0)
		.vertexBuffer(floorBuffer)
		.build();

	RID ambientGroup = ds::StateGroupBuilder()
		.texture(textureID, pixelShader, 0)
		.vertexBuffer(cubeBuffer)
		.build();

	RID bulbGroup = ds::StateGroupBuilder()
		.texture(floorTexture, pixelShader, 0)
		.vertexBuffer(bulbID)
		.build();

	ds::DrawCommand floorCmd = { 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
		
	RID floorStack[] = { basicGroup, floorGroup };
	RID floorItem = ds::compile(floorCmd, floorStack , 2);

	RID cubeStack[] = { basicGroup, ambientGroup };
	RID cubeItem = ds::compile(drawCmd, cubeStack, 2);

	RID bulbStack[] = { basicGroup, bulbGroup };
	RID bulbItem = ds::compile(drawCmd, bulbStack, 2);

	while (ds::isRunning()) {
		ds::begin();

		t += static_cast<float>(ds::getElapsedSeconds());

		constantBuffer.viewprojectionMatrix = ds::matTranspose(ds::getViewProjectionMatrix(basicPass));
			
		// floor
		ds::matrix world = ds::matIdentity();
		constantBuffer.worldMatrix = ds::matTranspose(world);
		ds::submit(basicPass, floorItem);
		
		// draw the light as small cube
		lightPos.x = cos(t);
		lightPos.z = sin(t);
		lightBuffer.lightDirection = lightPos;
		ds::vec3 lp = ds::vec3(-lightPos.x, -lightPos.y, -lightPos.z);
		ds::matrix bY = ds::matRotationY(t);
		w = bY * ds::matTranslate(lp);		
		constantBuffer.worldMatrix = ds::matTranspose(w);
		ds::submit(basicPass, bulbItem);
		
		// spinning cube
		world = ds::matIdentity();
		rotation.y += 2.0f  * static_cast<float>(ds::getElapsedSeconds());
		rotation.x += 1.0f  * static_cast<float>(ds::getElapsedSeconds());
		ds::matrix rotY = ds::matRotationY(rotation.y);
		ds::matrix rotX = ds::matRotationX(rotation.x);
		ds::matrix rotZ = ds::matRotationZ(rotation.z);
		ds::matrix s = ds::matScale(scale);
		ds::matrix w = rotZ * rotY * rotX * s * world;
		constantBuffer.worldMatrix = ds::matTranspose(w);
		ds::submit(basicPass, cubeItem);

		ds::end();
	}
	ds::shutdown();
}