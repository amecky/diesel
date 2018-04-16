#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#include "..\common\WaveFrontReader.h"
#include "..\common\Camera.h"
#include "HexGrid.h"
#include "Instancing_VS_Main.h"
#include "Instancing_PS_Main.h"

const int HEIGHT = 10;
const int WIDTH = 14;
const int TOTAL = HEIGHT * WIDTH;

struct GridItem {
	ds::vec3 pos;
	float timer;
	ds::Color color;
	int type;
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
	ds::matrix worldMatrix;
	ds::Color color;
};
/*
void log(const ds::LogLevel&, const char* message) {
	OutputDebugString(message);
	OutputDebugString("\n");
}
*/
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	ds::vec3 rotation(-ds::PI * 0.5f, 0.0f, 0.0f);
	ds::matrix rotX = ds::matRotationX(rotation.x);
	ObjVertex vertices[512];
	WaveFrontReader reader;
	int num = reader.load("hex.obj");// , &rotX);
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
	//rs.logHandler = &log;
	ds::init(rs);

	InstanceData instances[TOTAL];

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 3.0f, -8.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	ds::Camera camera = {
		viewMatrix,
		projectionMatrix,
		viewMatrix * projectionMatrix,
		ds::vec3(0,3,-8),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	ds::ViewportInfo vpInfo = { 1024,768,0.0f,1.0f };
	RID vp = ds::createViewport(vpInfo);
	ds::RenderPassInfo rpInfo = { &camera, vp, ds::DepthBufferState::ENABLED, 0, 0 };
	RID basicPass = ds::createRenderPass(rpInfo);

	ds::BlendStateInfo blendInfo = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
	RID bs_id = ds::createBlendState(blendInfo);

	ds::ShaderInfo vsInfo = { 0, Instancing_VS_Main, sizeof(Instancing_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID vertexShader = ds::createShader(vsInfo);
	ds::ShaderInfo psInfo = { 0, Instancing_PS_Main, sizeof(Instancing_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID pixelShader = ds::createShader(psInfo);
	//
	// the instance data contains a world matrix for each instance and a color
	//
	ds::InstancedInputLayoutDefinition instDecl[] = {
		{ "WORLD", 0, ds::BufferAttributeType::FLOAT4 },
		{ "WORLD", 1, ds::BufferAttributeType::FLOAT4 },
		{ "WORLD", 2, ds::BufferAttributeType::FLOAT4 },
		{ "WORLD", 3, ds::BufferAttributeType::FLOAT4 },
		{ "COLOR", 1, ds::BufferAttributeType::FLOAT4 }
	};
	//
	// the input layout of our vertex data
	//
	ds::InputLayoutDefinition decl[] = {
		{ "POSITION", 0, ds::BufferAttributeType::FLOAT3 },
		{ "TEXCOORD", 0, ds::BufferAttributeType::FLOAT2 },
		{ "NORMAL"  , 0, ds::BufferAttributeType::FLOAT3 },
		{ "COLOR"   , 0, ds::BufferAttributeType::FLOAT4 }
	};

	LightBuffer lightBuffer;
	lightBuffer.ambientColor = ds::Color(0.25f, 0.25f, 0.25f, 1.0f);
	lightBuffer.diffuseColor = ds::Color(255,255,255,255);
	lightBuffer.padding = 0.0f;
	ds::vec3 lightPos = ds::vec3(0.1f, 0.0f, 1.0f);
	lightBuffer.lightDirection = normalize(lightPos);
	ds::InstancedInputLayoutInfo iilInfo = { decl, 4, instDecl, 5, vertexShader };
	RID rid = ds::createInstancedInputLayout(iilInfo);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);
	RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer), &lightBuffer);
	ds::VertexBufferInfo vbInfo = { ds::BufferType::STATIC, num, sizeof(ObjVertex), vertices };
	RID vbid = ds::createVertexBuffer(vbInfo);
	ds::VertexBufferInfo ibInfo = { ds::BufferType::DYNAMIC, TOTAL, sizeof(InstanceData) };
	RID idid = ds::createVertexBuffer(ibInfo);
	RID instanceBuffer = ds::createInstancedBuffer(vbid, idid);
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);

	ds::vec3 scale(1.0f, 1.0f, 1.0f);
	
	ds::vec3 pos(0.0f, 0.0f, 0.0f);

	FPSCamera fpsCamera(&camera);
	fpsCamera.setPosition(ds::vec3(1, 3, -14));

	// we use a 14 x 10 hexagon grid 
	ds::HexGrid<int> grid;
	grid.resize(WIDTH, HEIGHT);

	float t = 0.0f;
	Layout l(layout_pointy,ds::vec2(0.58f),ds::vec2(-6.0f,-1.0f));

	GridItem items[TOTAL];

	for (int r = 0; r < HEIGHT; r++) {
		int q_offset = r >> 1;
		int w = 0;
		for (int q = -q_offset; q < WIDTH - q_offset; q++) {
			Hex h = Hex(q, r);
			ds::vec2 p = hex_math::hex_to_pixel(l, h);
			GridItem& item = items[w + r * WIDTH];
			item.pos = p;
			item.timer = ds::random(0.0f, ds::PI);
			item.color = ds::Color(192, 0, 0, 255);
			item.type = 0;
			if (r == 0 || r == HEIGHT - 1) {
				item.color = ds::Color(0, 0, 192, 255);
				item.type = 1;
			}
			if (q == -q_offset || q == WIDTH - q_offset  - 1) {
				item.color = ds::Color(0, 192, 0, 255);
				item.type = 1;
			}
			++w;
		}
	}

	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.constantBuffer(cbid, vertexShader, 0)
		.constantBuffer(lightBufferID, pixelShader, 0)
		.instancedVertexBuffer(instanceBuffer)
		.build();
	
	ds::DrawCommand drawCmd = { num, ds::DrawType::DT_INSTANCED, ds::PrimitiveTypes::TRIANGLE_LIST, TOTAL };

	RID drawItem = ds::compile(drawCmd, stateGroup);

	while (ds::isRunning()) {

		ds::begin();

		// rotate, scale and move every instance
		for (int y = 0; y < TOTAL; ++y) {
			GridItem& item = items[y];
			if (item.type == 0) {
				item.timer += ds::getElapsedSeconds();
				ds::matrix pm = ds::matTranslate(ds::vec3(item.pos.x, item.pos.y, sin(item.timer) * 0.4f));
				ds::matrix rx = ds::matRotationX(item.timer);
				float scale = 0.5f + sin(item.timer) * 0.2f;
				ds::matrix sm = ds::matScale(ds::vec3(scale, scale, scale));
				ds::matrix world = rx * sm * pm;
				instances[y] = { ds::matTranspose(world), item.color };
			}
			else {
				ds::matrix rx = ds::matRotationX(-ds::PI * 0.5f);
				ds::matrix pm = ds::matTranslate(ds::vec3(item.pos.x, item.pos.y, sin(item.timer) * 0.4f));
				ds::matrix world = rx * pm;
				instances[y] = { ds::matTranspose(world), item.color };
			}
			
		}
		// map the instance data
		ds::mapBufferData(idid, instances, sizeof(InstanceData) * TOTAL);
		// update camera
		fpsCamera.update(static_cast<float>(ds::getElapsedSeconds()));
		ds::matrix vpm = camera.viewProjectionMatrix;
		constantBuffer.viewprojectionMatrix = ds::matTranspose(vpm);
		ds::matrix world = ds::matIdentity();
		constantBuffer.worldMatrix = ds::matTranspose(world);
		// submit draw item
		ds::submit(basicPass, drawItem);

		ds::end();

	}
	ds::shutdown();
	return 0;
}