#define DS_IMPLEMENTATION
#include "..\..\diesel.h"

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {
	ds::vec3 p;
	ds::Color color;

};

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	//
	// prepare application
	//
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Triangle demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.supportDebug = true;
	ds::init(rs);
	//
	// create render with view and projection matrix
	//
	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 0.0f, -4.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	ds::Camera camera = {
		viewMatrix,
		projectionMatrix,
		viewMatrix * projectionMatrix,
		ds::vec3(0,0,-4),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	ds::RenderPassInfo rpInfo = { &camera, ds::DepthBufferState::ENABLED, 0, 0 };
	RID basicPass = ds::createRenderPass(rpInfo);
	//
	// create resources
	//
	RID vertexShader = ds::loadVertexShader("Triangle_vs.cso");
	RID pixelShader = ds::loadPixelShader("Triangle_ps.cso");
	// create buffer input layout
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};
	Vertex v[] = {
		{ ds::vec3(-1,-1,1),ds::Color(255,0,0,255) },
		{ ds::vec3(0,1,1),ds::Color(255,0,0,255) },
		{ ds::vec3(1,-1,1),ds::Color(255,0,0,255) }
	};
	RID rid = ds::createVertexDeclaration(decl, 2, vertexShader);
	ds::VertexBufferInfo vbInfo = { ds::BufferType::STATIC, 3, sizeof(Vertex), v };
	RID vbid = ds::createVertexBuffer(vbInfo);
	//
	// create state group
	//
	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.basicConstantBuffer(vertexShader)
		.vertexBuffer(vbid)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.build();
	//
	// the draw command
	//
	ds::DrawCommand drawCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };
	//
	// and finally we can create the draw item
	//
	RID drawItem = ds::compile(drawCmd, stateGroup);

	while (ds::isRunning()) {

		ds::begin();

		ds::submit(basicPass, drawItem);

		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());

		ds::end();
	}
	ds::shutdown();
}