#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "WaveFrontReader.h"
#include "..\common\Camera.h"
#include "HexGrid.h"

const int HEIGHT = 10;
const int WIDTH = 14;
const int TOTAL = HEIGHT * WIDTH;

struct GridItem {
	v3 pos;
	float timer;
};

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
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	ObjVertex vertices[512];
	WaveFrontReader reader;
	int num = reader.load("hex.obj", vertices, 512);
	if ( num == -1) {
		printf("Cannot load file\n");
	}
	
	CubeConstantBuffer constantBuffer;
	
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	


	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	RID shaderID = ds::createShader();
	ds::loadVertexShader(shaderID, "Mesh_vs.cso");
	ds::loadPixelShader(shaderID, "Mesh_ps.cso");

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
	v3 lightPos = v3(0.0f, -0.5f, 1.0f);
	lightBuffer.lightDirection = normalize(lightPos);


	RID rid = ds::createVertexDeclaration(decl, 4, shaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer));
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, num, rid, vertices,sizeof(ObjVertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	v3 vp = v3(0.0f, 3.0f, -6.0f);
	ds::setViewPosition(vp);

	v3 scale(1.0f, 1.0f, 1.0f);
	v3 rotation(ds::PI * 0.5f, 0.0f, 0.0f);
	v3 pos(0.0f, 0.0f, 0.0f);

	FPSCamera camera(1024, 768);
	camera.setPosition(v3(0, 2, -12));

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
			++w;
		}
	}


	while (ds::isRunning()) {
		ds::begin();

		camera.update(static_cast<float>(ds::getElapsedSeconds()));
		matrix vpm = camera.getViewProjectionMatrix();

		unsigned int stride = sizeof(ObjVertex);
		unsigned int offset = 0;

		// move cube
		//rotation.y += 2.0f  * static_cast<float>(ds::getElapsedSeconds());
		//rotation.x += 1.0f  * static_cast<float>(ds::getElapsedSeconds());
		//t += static_cast<float>(ds::getElapsedSeconds());
		//v3 scale = v3(0.9f + sin(t * 4.0f) * 0.2f);
		matrix world = mat_identity();
		matrix rotY = mat_RotationY(rotation.y);
		matrix rotX = mat_RotationX(rotation.x);
		matrix rotZ = mat_RotationZ(rotation.z);
		matrix s = mat_Scale(scale);
		matrix w = rotZ * rotY * rotX * s * world;

		ds::setVertexDeclaration(rid);
		ds::setBlendState(bs_id);
		ds::setShader(shaderID);
		ds::setSamplerState(ssid);
		
		// switch vertex buffer
		ds::setVertexBuffer(vbid, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
		// update constant buffer
		constantBuffer.viewProjectionMatrix = mat_Transpose(vpm);
		constantBuffer.worldMatrix = mat_Transpose(w);
		ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
		ds::updateConstantBuffer(lightBufferID, &lightBuffer, sizeof(LightBuffer));
		ds::setConstantBuffer(cbid, ds::ShaderType::VERTEX);
		ds::setConstantBuffer(lightBufferID, ds::ShaderType::PIXEL);
		// draw cube
		for (int y = 0; y < TOTAL; ++y) {
			GridItem& item = items[y];
			item.timer += ds::getElapsedSeconds();
			w = mat_Translate(v3(item.pos.x, item.pos.y, sin(item.timer) * 0.4f));
			w = rotX * w;
			constantBuffer.worldMatrix = mat_Transpose(w);
			ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
			ds::draw(num);
		}
		
		ds::end();
	}
	ds::shutdown();
}