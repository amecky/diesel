#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "..\common\WaveFrontReader.h"
#include "..\common\Camera.h"
#include "HexGrid.h"

const int HEIGHT = 10;
const int WIDTH = 14;
const int TOTAL = HEIGHT * WIDTH;

struct GridItem {
	ds::vec3 pos;
	float timer;
	ds::Color color;
};

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

struct InstanceData {
	ds::vec3 pos;
	ds::Color color;
};
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	ds::vec3 rotation(ds::PI * 0.5f, 0.0f, 0.0f);
	ds::matrix rotX = ds::matRotationX(rotation.x);
	ObjVertex vertices[512];
	WaveFrontReader reader;
	int num = reader.load("hex.obj", &rotX);
	for (size_t i = 0; i < reader.size(); ++i) {
		vertices[i] = reader.get(i);
	}
	if ( num == -1) {
		printf("Cannot load file\n");
	}
	num = reader.size();
	CubeConstantBuffer constantBuffer;
	
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Instancing example - 140 Hexagons";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	InstanceData instances[512];


	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "Mesh_vs.cso" },
		{ ds::ShaderType::PIXEL, "Mesh_ps.cso" }
	};

	RID shaderID = ds::createShader(desc,2);

	ds::InstanceLayoutDeclaration instDecl[] = {
		{ "NORMAL", 1, ds::BufferAttributeType::FLOAT, 3 },
		{ "COLOR", 1, ds::BufferAttributeType::FLOAT, 4 }
	};

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	LightBuffer lightBuffer;
	lightBuffer.ambientColor = ds::Color(0.15f, 0.15f, 0.15f, 1.0f);
	lightBuffer.diffuseColor = ds::Color(64,0,0,255);
	lightBuffer.padding = 0.0f;
	ds::vec3 lightPos = ds::vec3(0.0f, -0.5f, 1.0f);
	lightBuffer.lightDirection = normalize(lightPos);
	
	RID rid = ds::createInstanceDeclaration(decl, 4, instDecl, 2, shaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer));
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, num, sizeof(ObjVertex), vertices);
	RID idid = ds::createVertexBuffer(ds::BufferType::DYNAMIC, 512, sizeof(InstanceData));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	ds::vec3 vp = ds::vec3(0.0f, 3.0f, -6.0f);
	ds::setViewPosition(vp);

	ds::vec3 scale(1.0f, 1.0f, 1.0f);
	
	ds::vec3 pos(0.0f, 0.0f, 0.0f);

	FPSCamera camera(1024, 768);
	camera.setPosition(ds::vec3(0, 2, -12));

	ds::HexGrid<int> grid;
	grid.resize(20, 20);

	float t = 0.0f;
	Layout l(layout_pointy,ds::vec2(0.5f),ds::vec2(-6.0f,-1.0f));

	GridItem items[WIDTH * HEIGHT];

	for (int r = 0; r < HEIGHT; r++) {
		int q_offset = r >> 1;
		int w = 0;
		for (int q = -q_offset; q < WIDTH - q_offset; q++) {
			Hex h = Hex(q, r);
			ds::vec2 p = hex_math::hex_to_pixel(l, h);
			GridItem& item = items[w + r * WIDTH];
			item.pos = p;
			item.timer = ds::random(0.0f, ds::PI);
			//item.color = ds::Color(ds::random(0.6f, 1.0f), ds::random(0.6f, 1.0f), ds::random(0.6f, 1.0f), 1.0f);
			item.color = ds::Color(255, 255, 255, 255);
			++w;
		}
	}

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(rid);
	sg->bindShader(shaderID);
	sg->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	sg->bindConstantBuffer(lightBufferID, ds::ShaderType::PIXEL, &lightBuffer);
	sg->bindBlendState(bs_id);
	sg->bindSamplerState(ssid,ds::ShaderType::PIXEL);
	sg->bindInstancedVertexBuffer(vbid,idid);
	
	ds::DrawCommand drawCmd = { num, ds::DrawType::DT_INSTANCED, ds::PrimitiveTypes::TRIANGLE_LIST, TOTAL };

	ds::DrawItem* drawItem = ds::compile(drawCmd, sg);

	while (ds::isRunning()) {
		ds::begin();
		for (int y = 0; y < TOTAL; ++y) {
			GridItem& item = items[y];
			item.timer += ds::getElapsedSeconds();
			instances[y] = { ds::vec3(item.pos.x, item.pos.y, sin(item.timer) * 0.4f), item.color };
		}
		ds::mapBufferData(idid, instances, sizeof(InstanceData) * TOTAL);
		camera.update(static_cast<float>(ds::getElapsedSeconds()));
		ds::matrix vpm = camera.getViewprojectionMatrix();
		constantBuffer.viewprojectionMatrix = ds::matTranspose(vpm);
		ds::matrix world = ds::matIdentity();
		constantBuffer.worldMatrix = ds::matTranspose(world);
		ds::submit(drawItem);
		ds::end();
	}
	ds::shutdown();
}