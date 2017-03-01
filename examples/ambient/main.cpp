#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void addPlane(int index, int side, Vertex* vertices, const matrix& world) {
	int idx = index * 4;
	float u[4] = { 0.0f,0.0f,1.0f,1.0f };
	float v[4] = { 1.0f,0.0f,0.0f,1.0f };
	for (int i = 0; i < 4; ++i) {
		int p = idx + i;
		v3 pos = world * CUBE_VERTICES[CUBE_PLANES[side][i]];
		vertices[p] = Vertex(pos, v3(0.0f), v2(u[i],v[i]));
	}	
	v3 d0 = vertices[idx + 1].p - vertices[idx].p;
	v3 d1 = vertices[idx + 2].p - vertices[idx].p;
	v3 c = cross(d0, d1);
	for (int i = 0; i < 4; ++i) {
		vertices[idx + i].n = c;
	}
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

int main(const char** args) {
	matrix w = mat_identity();
	Vertex v[24];
	addPlane(0, 0, v, w);
	addPlane(1, 1, v, w);
	addPlane(2, 2, v, w);
	addPlane(3, 3, v, w);
	addPlane(4, 4, v, w);
	addPlane(5, 5, v, w);

	matrix s = mat_Scale(v3(0.2f,0.2f,0.2f));
	matrix lw = s * w;
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
	lightBuffer.ambientColor = ds::Color(0.15f, 0.15f, 0.15f,1.0f);
	lightBuffer.diffuseColor = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
	lightBuffer.padding = 0.0f;
	v3 lightPos = v3(1.0f, -0.5f, 1.0f);
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
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, rid, v,sizeof(Vertex));
	RID floorBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, rid, floorVertices, sizeof(Vertex));
	RID bulbID = ds::createVertexBuffer(ds::BufferType::STATIC, 24, rid, lv, sizeof(Vertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	v3 vp = v3(0.0f, 2.0f, -6.0f);
	ds::setViewPosition(vp);
	v3 scale(1.0f, 1.0f, 1.0f);
	v3 rotation(0.0f, 0.0f, 0.0f);
	v3 pos(0.0f, 0.0f, 0.0f);

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

	while (ds::isRunning()) {
		ds::begin();

		t += static_cast<float>(ds::getElapsedSeconds());

		constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
			
		// floor
		matrix world = mat_identity();
		constantBuffer.worldMatrix = mat_Transpose(world);
		ds::submit(floorItem);
		
		// draw the light as small cube
		lightPos.x = cos(t);
		lightPos.z = sin(t);
		lightBuffer.lightDirection = lightPos;
		v3 lp = v3(-lightPos.x, -lightPos.y, -lightPos.z);
		matrix bY = mat_RotationY(t);
		w = bY * mat_Translate(lp);		
		constantBuffer.worldMatrix = mat_Transpose(w);
		ds::submit(bulbItem);
		
		// spinning cube
		world = mat_identity();
		rotation.y += 2.0f  * static_cast<float>(ds::getElapsedSeconds());
		rotation.x += 1.0f  * static_cast<float>(ds::getElapsedSeconds());
		matrix rotY = mat_RotationY(rotation.y);
		matrix rotX = mat_RotationX(rotation.x);
		matrix rotZ = mat_RotationZ(rotation.z);
		matrix s = mat_Scale(scale);
		matrix w = rotZ * rotY * rotX * s * world;
		constantBuffer.worldMatrix = mat_Transpose(w);
		ds::submit(cubeItem);

		ds::end();
	}
	ds::shutdown();
}