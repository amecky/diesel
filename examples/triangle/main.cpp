#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#include "Triangle_VS_Main.h"
#include "Triangle_PS_Main.h"

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

	RID vp = ds::createViewport(ds::ViewportDesc()
		.Top(0)
		.Left(0)
		.Width(1024)
		.Height(768)
		.MinDepth(0.0f)
		.MaxDepth(1.0f)
	);

	
	RID basicPass = ds::createRenderPass(ds::RenderPassDesc()
		.Camera(&camera)
		.Viewport(vp)
		.DepthBufferState(ds::DepthBufferState::ENABLED)
		.RenderTargets(0)
		.NumRenderTargets(0));
	//
	// create resources
	//
	
	RID vertexShader = ds::createShader(ds::ShaderDesc()
		.Data(Triangle_VS_Main)
		.DataSize(sizeof(Triangle_VS_Main))
		.ShaderType(ds::ShaderType::ST_VERTEX_SHADER)
	);

	RID pixelShader = ds::createShader(ds::ShaderDesc()
		.Data(Triangle_PS_Main)
		.DataSize(sizeof(Triangle_PS_Main))
		.ShaderType(ds::ShaderType::ST_PIXEL_SHADER)
	);

	// create buffer input layout
	ds::InputLayoutDefinition decl[] = {
		{ "POSITION", 0, ds::BufferAttributeType::FLOAT3 },
		{ "COLOR", 0, ds::BufferAttributeType::FLOAT4 }
	};
	Vertex v[] = {
		{ ds::vec3(-1,-1,1),ds::Color(255,0,0,255) },
		{ ds::vec3(0,1,1),ds::Color(255,0,0,255) },
		{ ds::vec3(1,-1,1),ds::Color(255,0,0,255) }
	};
	
	RID rid = ds::createInputLayout(ds::InputLayoutDesc()
	.	Declarations(decl)
		.NumDeclarations(2)
		.VertexShader(vertexShader)
	);

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
	return 0;
}