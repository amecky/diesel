# Creating resources

The following will list resource creation methods.

## Buffer Input Layout / Vertex Declaration

You need to define the input buffer layout for any vertex buffer. But you also need a loaded vertex shader to
create a vertex declaration.

```c
RID createVertexDeclaration(VertexDeclaration* decl, uint8_t num, RID shaderId);
```
### Example

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
## Constant buffer

```c
RID createConstantBuffer(int byteWidth);
```
### Example

```c
struct ConstantBuffer {
    matrix world;
    matrix viewProjection;
};

RID cbID = ds::createConstantBuffer(sizeof(ConstantBuffer));
```

## Index buffer

```c
RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type);
```
```c
RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type, void* data);
```
```c
RID createQuadIndexBuffer(int numQuads);
```

## Vertex buffer

```c
RID createVertexBuffer(BufferType type, int numVertices, uint32_t vertexSize, void* data = 0);
```

## Ssampler state

```c
RID createSamplerState(TextureAddressModes addressMode, TextureFilters filter);
```

## Blendstate

```c
RID createBlendState(BlendStates srcBlend, BlendStates srcAlphaBlend, BlendStates destBlend, BlendStates destAlphaBlend, bool alphaEnabled);
```	

```c
RID createAlphaBlendState(BlendStates srcBlend, BlendStates destBlend);
```

## Shader

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

## Texture

```c
RID createTexture(int width, int height, uint8_t channels, void* data, TextureFormat format);
```

## RenderTarget

```c
RID createRenderTarget(uint16_t width, uint16_t height, const ds::Color& clearColor);
```

## Rasterizer state

```c
RID createRasterizerState(CullMode cullMode, FillMode fillMode, bool multiSample, bool scissor, float depthBias, float slopeDepthBias);
```
