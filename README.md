# diesel

Single Header DirectX11 renderer

## Basic concepts

## Creating resources

The following will list resource creation methods.

### Buffer Input Layout / Vertex Declaration

You need to define the input buffer layout for any vertex buffer. But you also need a loaded vertex shader to
create a vertex declaration.

```c
RID createVertexDeclaration(VertexDeclaration* decl, uint8_t num, RID shaderId);
```
#### Example

```c
ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

RID rid = ds::createVertexDeclaration(decl, 2, gridShader);
```

```c
RID createInstanceDeclaration(VertexDeclaration* decl, uint8_t num, InstanceLayoutDeclaration* instDecl, uint8_t instNum, RID shaderId);
```
### Constant buffer

```c
RID createConstantBuffer(int byteWidth);
```
#### Example

```c
struct ConstantBuffer {
    matrix world;
    matrix viewProjection;
};

RID cbID = ds::createConstantBuffer(sizeof(ConstantBuffer));
```

### Index buffer

```c
RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type);
```
```c
RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type, void* data);
```
```c
RID createQuadIndexBuffer(int numQuads);
```

### Vertex buffer

```c
RID createVertexBuffer(BufferType type, int numVertices, uint32_t vertexSize, void* data = 0);
```

### Ssampler state

```c
RID createSamplerState(TextureAddressModes addressMode, TextureFilters filter);
```

### Blendstate

```c
RID createBlendState(BlendStates srcBlend, BlendStates srcAlphaBlend, BlendStates destBlend, BlendStates destAlphaBlend, bool alphaEnabled);
```	

```c
RID createAlphaBlendState(BlendStates srcBlend, BlendStates destBlend);
```

### Shader

```c
RID createShader();
```

```c
RID createShader(ShaderDescriptor* descriptors,int num);
```

```c
RID createShader(ShaderDataDescriptor* descriptors, int num);
```

```c
void loadShader(RID shader, ShaderType type, const char* csoName);
```

### Texture

```c
RID createTexture(int width, int height, uint8_t channels, void* data, TextureFormat format);
```

### RenderTarget

```c
RID createRenderTarget(uint16_t width, uint16_t height, const ds::Color& clearColor);
```

### Rasterizer state

```c
RID createRasterizerState(CullMode cullMode, FillMode fillMode, bool multiSample, bool scissor, float depthBias, float slopeDepthBias);
```

## Building a StateGroup and binding resources

A StateGroup is a combination of different resources. You can combine several state groups when building a DrawItem.
It uses a kind of inheritance to avoid that already binded resources are overwritten. 

### Example

```c
// basic state group
ds::StateGroup* basicGroup = ds::createStateGroup();
basicGroup->bindLayout(bufferLayoutID);
basicGroup->bindConstantBuffer(constantBufferID, ds::ShaderType::VERTEX, &constantBuffer);
basicGroup->bindBlendState(blendStateID);
basicGroup->bindSamplerState(samplerStateID, ds::ShaderType::PIXEL);
basicGroup->bindTexture(textureID, ds::ShaderType::PIXEL, 0);
basicGroup->bindShader(shaderID);
basicGroup->bindIndexBuffer(indexBufferID);
// new group for some static cubes
ds::StateGroup* staticGroup = ds::createStateGroup();
staticGroup->bindVertexBuffer(staticCubes);
```

## Building a DrawItem

The last step before you can actually render something is to build a DrawItem. This is a combination
of a DrawCommand and a list of StateGroup. Of course you can also use only one StageGroup

### Example using one StateGroup
```c
ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
ds::DrawItem* staticItem = ds::compile(staticCmd, basicGroup);
```

### Example using several StateGroup
```c
ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
ds::StateGroup* staticStack[] = { basicGroup, staticGroup };
ds::DrawItem* staticItem = ds::compile(staticCmd, staticStack, 2);
```
 
## RenderPass 



## Math
A rendering engine uses vectors and matrices in some way. Therefore diesel comes with a 
template based Vector 2D/3D/4D and matrix implementation inside the header file. 
You can easily 

## GPU profiling

The renderer also includes a simple GPU profiler. 
It is heavily based on this article:
http://reedbeta.com/blog/gpu-profiling-101/

You can switch on the profiling using the RenderSettings:

```c
ds::RenderSettings settings;
settings.useGPUProfiler = true;
```
Now the renderer will take care of staring and ending the profiling. It will also measure the time for the entire
frame. You can plugin in your profiling steps by calling measure:

```c
// do something
ds::submit(staticItem);
ds::gpu::measure(1);
```

Please note that 0 is reserved for the entire frame measurement.
Before you can get the data from the profiling you need to call:

```c
ds::gpu::waitForData();
```

This will collect the timings.

### API

The API uses the namespace ds::gpu. 

In order to profile you need to call measure after you habe submitted a draw call:
```c
void measure(int index);
```

The profiler needs to collect the data once you are and therefore you need to call:

```c
void waitForData();
```

This returns the duration for a certain measurement:

```c
float dt(int index);
```

This is the average durarion for a certain measurement. The average is calculated every 0.5 seconds:

```c
float dtAvg(int index);
```

This is the total time for an entire frame:

```c
float totalTime();
```

### Example

The Texture examples uses GPU profiling as an example. Here is a short example using the imgui:
 
```c
gui::start(ds::vec2(700, 750));
gui::begin("Timer", &state);
if (state == 1) {
	gui::Value("DrawTime", 1000.0f * ds::gpu::totalTime(), "%0.4f");
	gui::Value("DT1", 1000.0f * ds::gpu::dtAvg(1), "%0.4f");
	gui::Value("DT2", 1000.0f * ds::gpu::dtAvg(2), "%0.4f");
	gui::Value("DT3", 1000.0f * ds::gpu::dtAvg(3), "%0.4f");
}
gui::end();
```		

