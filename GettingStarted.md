# Getting started

We will run through the very basic steps of rendering something on screen. This example
can be found in the examples/triangle directory. First of all here is the complete listing.
The steps will be explained further down.

## Listing

```c
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
	// prepare application (1)
	//
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Triangle demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);
	//
	// create render with view and projection matrix (2)
	//
	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 0.0f, -4.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	RID basicPass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::ENABLED);

	//
	// create resources (3)
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
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, 3, sizeof(Vertex), v);
	//
	// create state group (4)
	//
	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.basicConstantBuffer(vertexShader)
		.vertexBuffer(vbid)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.build();
	//
	// the draw command (5)
	//
	ds::DrawCommand drawCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };
	//
	// and fianlly we can create the draw item (6)
	//
	RID drawItem = ds::compile(drawCmd, stateGroup);

	while (ds::isRunning()) {
		ds::begin();

		ds::submit(basicPass, drawItem); // (7)

		ds::end();
	}
	ds::shutdown();
}
```

## Preparation (1)

The first step is to initialize the rendering system. Most settings are pretty obvious. The gpuProfiling is a special case in will 
not be covered here. 

## RenderPass (2)

Next step is to define a render pass. We are using a regular view and projection matrix. The viewpoint is set to (0,0,-4). Also we are 
enabling the depth buffer write. This is a pretty regular setup for 3D rendering.

## Resources (3)

Now we also need to create some resources. First we load the two shaders. Then we create an input layout. Since we need the
vertex shader here it is necessary to load the shaders first. Also we will create a static vertex buffer and pass over the
three vertices for our triangle.

## StateGroup (4)

The StateGroup defines the states of the rendering pipeline. First of all we only need to bind the resources that we really need.
All other states are handled by the default state group which is applied after this state group. There is no need to for example
set the geometry shader to none.
One special note here is that we are using the "BasicConstantBuffer". This is a special buffer which will contain the view-matrix,
projection-matrix, world-matrix and view-projection-matrix defined by the render pass. It will be automatically set.
Therefore we do not need to define a constant buffer and set the values althogh you can see it in the vertex shader that
we are using one.

## Draw Command (5)

The draw command is pretty straightforward. We will draw 3 vertices as TriangleList.

## DrawItem (6)

The last step of preparation is the DrawItem which combines the DrawCommand and the StateGroup.

## Rendering (7)

Everything is ready and we can now finally submit the DrawItem along with the render pass.