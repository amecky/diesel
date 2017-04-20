# Goals

The main goal of this project is to provide a single header DirectX 11 stateless renderer.
The native DirectX 11 API is a statefull API. If you bind for example a vertex buffer then
it will used by any draw call unless you overwrite it or reset it. Therefore it is difficult
to know the current state of your rendering pipeline. There might be some resources still
binded for the next draw call without even knowing it. It is difficult to make sure youhave all the
desired resources bind and of course all others reset. diesel makes sure that
for every draw call you only get the assigned resources.

The main principle is that you submit a combination of a RenderPass and DrawItem to renderer something.
When submitting this information diesel will ckeck the previous state of the rendering pipeline
and revert any left over states or overwrite them with defaults.

The following will describe the terms.
 
All the ideas here are based on the wonderfull presentation [Designing a modern GPU interface](https://www.dropbox.com/sh/4uvmgy14je7eaxv/AABboa6UE5Pzfg3d9updwUexa?dl=0&preview=Designing+a+Modern+GPU+Interface.pdf)
by Brooke Hodgman. This is basically an implementation of what is describe in the presentation.
 
## RenderPass

A render pass defines first of all the view and projection matrix. Also you can set the
depth buffer state.

### Example of creating a render pass
```c
ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 2.0f, -8.0f), ds::vec3(0,0,0), ds::vec3(0,1,0));
ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
RID basicPass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::ENABLED, "BasicPass");
```	

## Resource

A resource is anything like a vertex buffer or texture or shader. The engine takes
care of the lifecycle of every resource. Every resource is identified by a RID (unique 
resource identifier). You cannot access any resource directly. Also the binding
of resources is defined by these RIDs. 

### Example of creating resources

This is just a short example showing how to create resources.

```c
ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	RID rid = ds::createVertexDeclaration(decl, 2, vertexShader, "PosTex_Layout");
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer, "CubeConstantBuffer");
	RID indexBuffer = ds::createQuadIndexBuffer(256, "IndexBuffer");
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), v, "CubeBuffer");
	RID staticCubes = ds::createVertexBuffer(ds::BufferType::STATIC, totalCubeVertices, sizeof(Vertex), sv, "StaticCubes");
```

## StateGroup

A StateGroup will bind the defined resources to the rendering pipeline.
There is a helper class called StateGroupBuilder that you can use to
actually bind the resources. First there is an example to demonstrate
how to build a StateGroup. 

*Example of building a StateGroup*

```c
RID basicGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.blendState(blendStateID)
		.constantBuffer(cbid, vertexShader, 0)
		.texture(textureID, pixelShader, 0)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.samplerState(ssid,pixelShader)
		.indexBuffer(indexBuffer)
		.build("BasicGroup"); 
```

The engine will take care to bind the resources in the correct order. 

### Inheritance

You do not need to define all kind of resources when building a StateGroup. You can
also use inheritance and to combine groups. This means you can define a group
containing all common rendering states and then build a group that will only
contain the special resources needed to render something. 
The engine will process the groups in the defined order and will only bind resources
if they have not been binded before.

## DrawCommand

The DrawCommand defines what you are going to render.
```c
ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
```

## DrawItem

A DrawItem combines all native API states and resources together.

### Example using one DrawItem
```c
RID cubeItem = ds::compile(drawCmd, cubeGroup, "CubeItem");
```

## Example of using several state groups
```c
RID cubeStack[] = { basicGroup, cubeGroup };
RID cubeItem = ds::compile(drawCmd, cubeStack, 2, "CubeItem");
```


## Rendering

Rendering an object just means to submit the render pass and DrawItem

```c
ds::submit(basicPass, cubeItem);
```

