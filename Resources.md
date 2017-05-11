# Resources 

## Table of contents

- [Camera](#camera)
- [RenderPass](#renderpass)
- [Blendstate](#blendstate)

- [x] INPUT_LAYOUT
- [ ] CONSTANT_BUFFER
- [x] INDEX_BUFFER
- [x] VERTEX_BUFFER
- [ ] SAMPLER_STATE
- [ ] BLENDSTATE
- [ ] SRV
- [x] RASTERIZER_STATE		
- [ ] INSTANCED_VERTEX_BUFFER
- [ ] VERTEX_SHADER
- [ ] GEOMETRY_SHADER
- [ ] PIXEL_SHADER
- [ ] RENDER_TARGET
- [x] RENDER_PASS
- [ ] DRAW_ITEM
- [ ] STATE_GROUP
- [ ] STRUCTURED_BUFFER
        
## Camera <a name="camera"></a>

A camera is a logical unit containing the view and projection matrix. You need to create a camera to 
create a RenderPass.
The following structs shows all attributes of a camera.

```c
struct Camera {
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix viewProjectionMatrix;
    ds::vec3 position;
    ds::vec3 lookAt;
    ds::vec3 target;
    ds::vec3 up;
    ds::vec3 right;
    float roll;
	float pitch;
	float yaw;		
};
```

If you change for example the position you should call:

```c
ds::rebuildCamera(ds::Camera* camera);
```

This method will rebuild the view and the view projection matrix.

### Example

The following example creates a camera located at {0,1,-6} with target {0,0,1}.
You can take a look at the camera example to see how to build a first person camera.

```c
ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 1.0f, -6.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
ds::Camera camera = {
    viewMatrix,
    projectionMatrix,
    viewMatrix * projectionMatrix,
    ds::vec3(0,1,-6),
    ds::vec3(0,0,1),
    ds::vec3(0,1,0),
    ds::vec3(1,0,0),
    0.0f,
    0.0f,
    0.0f
};
```

You can also create a camera using an orthographic view:
```c
ds::matrix orthoView = ds::matIdentity();
ds::matrix orthoProjection = ds::matOrthoLH(ds::getScreenWidth(), ds::getScreenHeight(), 0.1f, 1.0f);
ds::Camera orthoCamera = {
    orthoView,
    orthoProjection,
    orthoView * orthoProjection,
    ds::vec3(0,0,0),
    ds::vec3(0,0,0),
    ds::vec3(0,1,0),
    ds::vec3(1,0,0),
    0.0f,
    0.0f,
    0.0f
};
```

## RenderPass <a name="renderpass"></a>

You can create create a RenderPass by building a RenderPassInfo:

```c
struct RenderPassInfo {
    Camera* camera;
    DepthBufferState depthBufferState;
    RID* renderTargets;
    int numRenderTargets;
};
```

| Attribute        | Description              | Optional  |
| ---------------- |--------------------------|:---------:|
| camera           | the camera to us         | no        |
| depthBufferState | depth buffer state       | no        |
| renderTargets    | array of render targets  | no        |
| numRenderTargets | number of render targets | no        |

The DepthBufferState:

- ds::DepthBufferState::ENABLED
- ds::DepthBufferState::DISABLED


```c
RID createRenderPass(const RenderPassInfo& info, const char* name = "RenderPass");
```

### Example
The following example will create a RenderPass using a camera and the depth buffer enabled and also no RenderTargets.

```c
ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 1.0f, -6.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
ds::Camera camera = {
    viewMatrix,
    projectionMatrix,
    viewMatrix * projectionMatrix,
    ds::vec3(0,1,-6),
    ds::vec3(0,0,1),
    ds::vec3(0,1,0),
    ds::vec3(1,0,0),
    0.0f,
    0.0f,
    0.0f
};

ds::RenderPassInfo rpInfo = { &camera, ds::DepthBufferState::ENABLED, 0, 0 };
RID ppPass = ds::createRenderPass(rpInfo);
```

## BlendState <a name="blendstate"></a>

In order to create a BlendState you need to fill out a BlendStateInfo:
```c
struct BlendStateInfo {
    BlendStates srcBlend;
    BlendStates srcAlphaBlend;
    BlendStates destBlend;
    BlendStates destAlphaBlend;
    bool alphaEnabled;
};
```

Then you can create a BlendState:    
```c
RID createBlendState(const BlendStateInfo& info, const char* name = "BlendState");
```

### Example

```c
ds::BlendStateInfo myBlendState = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
RID bs_id = ds::createBlendState( myBlendState, "MyBlendState");
```

## SamplerState

```c
struct SamplerStateInfo {
	TextureAddressModes addressMode;
	TextureFilters filter;
};
```

| Attribute   | Description          | Optional  |
| ------------|----------------------|:---------:|
| addressMode | texture address mode | no        |
| filter      | texture filter mode  | no        |

TextureAddressModes:

- ds::TextureAddressModes::WRAP
- ds::TextureAddressModes::MIRROR
- ds::TextureAddressModes::CLAMP
- ds::TextureAddressModes::BORDER
- ds::TextureAddressModes::MIRROR_ONCE

TextureFilters:

- ds::TextureFilters::POINT
- ds::TextureFilters::LINEAR
- ds::TextureFilters::ANISOTROPIC
    
```c    
RID createSamplerState(const SamplerStateInfo& info, const char* name = "SamplerState");
```
    
    
## Texture

In order to create a Texture you need to fill out the TextureInfo

```c
struct TextureInfo {
    uint16_t width;
    uint16_t height;
    uint8_t channels;
    void* data;
    TextureFormat format;
    uint16_t bindFlags;
};
```

| Attribute | Description             | Optional  |
| --------- |-------------------------|:---------:|
| width     | STATIC or DYNAMIC       | no        |
| height    | number of vertices      | no        |
| channels  | byte size of one vertex | no        |
| data      | initial data            | yes       |
| format    | initial data            | yes       |
| bindFlags | initial data            | yes       |


### Example

```c
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int x, y, n;
unsigned char *data = stbi_load(name, &x, &y, &n, 4);
ds::TextureInfo info = { x,y,n,data,ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
RID textureID = ds::createTexture(info);
stbi_image_free(data);
return textureID;
```

## VertexBuffer

You can create a vertex buffer by using the VertexBufferInfo:

```c
struct VertexBufferInfo {
    BufferType type;
    uint32_t numVertices;
    uint32_t vertexSize;
    void* data;
};
```

| Attribute   | Description             | Optional  |
| ----------- |-------------------------|:---------:|
| type        | STATIC or DYNAMIC       | no        |
| numVertices | number of vertices      | no        |
| vertexSize  | byte size of one vertex | no        |
| data        | initial data            | yes       |

You can use this one to provide initial data for the vertex buffer.

The BufferType is defined as:

- ds::BufferType::STATIC
- ds::BufferType::DYNAMIC

```c
RID createVertexBuffer(const VertexBufferInfo& info, const char* name = "VertexBuffer");
```

### Example

The following shows how to create a static vertex buffer using initial data:

```c
Vertex v[] = {
    { ds::vec3(-1,-1,1),ds::Color(255,0,0,255) },
    { ds::vec3(0,1,1),ds::Color(255,0,0,255) },
    { ds::vec3(1,-1,1),ds::Color(255,0,0,255) }
};
ds::VertexBufferInfo vbInfo = { ds::BufferType::STATIC, 3, sizeof(Vertex), v };
RID vbid = ds::createVertexBuffer(vbInfo);
```

## InputLayoutDefinition

In order to create an InputLayout you need to provide an array of InputLayoutDefinitions:
```c
struct InputLayoutDefinition {
    BufferAttribute attribute;
    BufferAttributeType type;
    uint8_t size;
};
```

| Attribute | Description               | Optional  |
| --------- |---------------------------|:---------:|
| attribute | the buffer attribute      | no        |
| type      | the buffer attribute type | no        |
| size      | the size of the entry     | no        |

The following list shows all available ds::BufferAttribute:

- ds::BufferAttribute::POSITION
- ds::BufferAttribute::COLOR
- ds::BufferAttribute::TEXCOORD
- ds::BufferAttribute::NORMAL
- ds::BufferAttribute::TANGENT
- ds::BufferAttribute::BINORMAL

Currently only the following BufferAttributeTypes are supported:

- ds::BufferAttributeType::FLOAT
- ds::BufferAttributeType::UINT_8

## InputLayout

Once you have defined an array of InputLayoutDefitions you can create an InputLayout using the InputLayoutInfo:

```c
struct InputLayoutInfo {
    InputLayoutDefinition* declarations;
    uint8_t numDeclarations;
    RID vertexShaderId;
};
```

| Attribute       | Description                     | Optional  |
| --------------- |---------------------------------|:---------:|
| declarations    | array of InputLayoutDefinitions | no        |
| numDeclarations | number of entries in array      | no        |
| vertexShaderId  | RID of the vertex shader        | no        |

```c
RID createInputLayout(const InputLayoutInfo& info, const char* name = "InputLayout");
```

### Example

```c
ds::InputLayoutDefinition decl[] = {
    { ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
    { ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
};
ds::InputLayoutInfo layoutInfo = { decl, 2, vertexShader };
RID rid = ds::createInputLayout(layoutInfo, "PosTex_Layout");
```

## IndexBuffer

You need to create an IndexBufferInfo to create an index buffer. Here is the IndexBufferInfo struct:
```c
struct IndexBufferInfo {
	uint32_t numIndices;
	IndexType indexType;
	BufferType type;
	void* data;
};
```

| Attribute   | Description                | Optional  |
| ----------- |----------------------------|:---------:|
| numIndices  | number of indices          | no        |
| indexType   | the index buffer byte type | no        |
| type        | STATIC or DYNAMIC          | no        |
| data        | initial data               | yes       |

The IndexType can be:
- ds::IndexType::UINT_16
- ds::IndexType::UINT_32

The BufferType can be:
- ds::BufferType::STATIC
- ds::BufferType::DYNAMIC


Then you can create an index buffer:
```c
RID createIndexBuffer(const IndexBufferInfo& info, const char* name = "IndexBuffer");
```

### Example
This example shows how to create a dynamic index buffer containing 32 elements and the type is uint16_t: 
```c
ds::IndexBufferInfo info = {32, ds::IndexType::UINT_16,ds::BufferType::DYNAMIC,0};
RID indexBuffer = ds::createIndexBuffer(info, "MyIndexBuffer");
```

## Rasterizer state

In order to create a rasterizer state you need to fill out a RasterizerStateInfo:

```c
struct RasterizerStateInfo {
    CullMode cullMode;
    FillMode fillMode;
    bool multiSample;
    bool scissor;
    float depthBias;
    float slopeDepthBias;
};
```

| Attribute      | Description   | Optional  |
| -------------- |---------------|:---------:|
| cullMode       | the cull mode | no        |
| fillMode       | the fill mode | no        |
| multiSample    |               | no        |
| scissor        |               | no        |
| depthBias      |               | no        |
| slopeDepthBias |               | no        |

The CullMode can be:
 
- ds::CullMode::NONE
- ds::CullMode::FRONT
- ds::CullMode::BACK

The FillMode can be:
    
- ds::FillMode::WIREFRAME
- ds::FillMode::SOLID

Then you can create a rasterizer state:

```c    
RID createRasterizerState(const RasterizerStateInfo& info, const char* name = "RasterizerState");
```    

## Shader

```c    
struct ShaderInfo {
    const char* csoName;
    const void* data;
    int dataSize;
    ShaderType type;
};
```

| Attribute | Description                         | Optional  |
| ----------|-------------------------------------|:---------:|
| csoName   | the filename of the compiled shader | yes       |
| data      | the shader data                     | yes       |
| dataSize  | The size of the data                | yes       |
| type      | The shader type                     | no        |


- ds::ShaderType::ST_VERTEX_SHADER
- ds::ShaderType::ST_PIXEL_SHADER
- ds::ShaderType::ST_GEOMETRY_SHADER
- ds::ShaderType::ST_COMPUTE_SHADER

```c
RID createShader(const ShaderInfo& info, const char* name = "Shader");
```

### Example

```c    
ds::ShaderInfo vsInfo = { "Textured_vs.cso" , 0, 0, ds::ShaderType::ST_VERTEX_SHADER };
RID vertexShader = ds::createShader(vsInfo);
ds::ShaderInfo psInfo = { "Textured_ps.cso" , 0, 0, ds::ShaderType::ST_PIXEL_SHADER };
RID pixelShader = ds::createShader(psInfo);
```