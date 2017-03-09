#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "..\common\WaveFrontReader.h"
#include "..\common\Camera.h"
#include "HexGrid.h"

const int HEIGHT = 10;
const int WIDTH = 14;
const int TOTAL = HEIGHT * WIDTH;

struct GridItem {
	v3 pos;
	float timer;
	ds::Color color;
};

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

struct InstanceData {
	float pos[3];
	float color[4];
};
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	v3 rotation(ds::PI * 0.5f, 0.0f, 0.0f);
	float rotX[16];
	ds::matRotationX(rotX, ds::PI);
	ObjVertex vertices[512];
	WaveFrontReader reader;
	int num = reader.load("hex.obj", rotX);
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
	ds::vec4(lightBuffer.ambientColor,0.15f, 0.15f, 0.15f, 1.0f);
	ds::vec4(lightBuffer.diffuseColor,0.25f,0.0f,0.0f,1.0f);
	lightBuffer.padding = 0.0f;
	float lightPos[3] = { 0.0f, -0.5f, 1.0f };
	ds::vec3(lightBuffer.lightDirection,lightPos);
	
	RID rid = ds::createInstanceDeclaration(decl, 4, instDecl, 2, shaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer));
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, num, sizeof(ObjVertex), vertices);
	RID idid = ds::createVertexBuffer(ds::BufferType::DYNAMIC, 512, sizeof(InstanceData));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	float vp[3] = { 0.0f, 3.0f, -6.0f };
	ds::setViewPosition(vp);

	//FPSCamera camera(1024, 768);
	//camera.setPosition(v3(0, 2, -12));

	ds::HexGrid<int> grid;
	grid.resize(20, 20);

	float t = 0.0f;
	Layout l(layout_pointy,v2(0.5f),v2(-6.0f,-1.0f));

	GridItem items[WIDTH * HEIGHT];

	for (int r = 0; r < HEIGHT; r++) {
		int q_offset = r >> 1;
		int w = 0;
		for (int q = -q_offset; q < WIDTH - q_offset; q++) {
			Hex h = Hex(q, r);
			v2 p = hex_math::hex_to_pixel(l, h);
			GridItem& item = items[w + r * WIDTH];
			item.pos = p;
			item.timer = ds::random(0.0f, ds::PI);
			//item.color = ds::Color(ds::random(0.6f, 1.0f), ds::random(0.6f, 1.0f), ds::random(0.6f, 1.0f), 1.0f);
			item.color = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
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

	float vpm[16];
	ds::getViewProjectionMatrix(vpm);
	ds::matTranspose(constantBuffer.viewProjectionMatrix, vpm);
	float world[16];
	ds::matIdentity(world);
	ds::matTranspose(constantBuffer.worldMatrix, world);

	while (ds::isRunning()) {
		ds::begin();
		for (int y = 0; y < TOTAL; ++y) {
			GridItem& item = items[y];
			item.timer += ds::getElapsedSeconds();
			ds::vec3(instances[y].pos, item.pos.x, item.pos.y, sin(item.timer) * 0.4f);
			ds::vec4(instances[y].color, item.color);
		}
		ds::mapBufferData(idid, instances, sizeof(InstanceData) * TOTAL);
		//camera.update(static_cast<float>(ds::getElapsedSeconds()));
		//ds::getViewProjectionMatrix(vpm);
		//ds::matTranspose(constantBuffer.viewProjectionMatrix, vpm);
		ds::submit(drawItem);
		ds::end();
	}
	ds::shutdown();
}