# Resources 

## Camera

A camera is a logical unit containing the view and projection matrix. You need to create a camera to create a RenderPass.
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

## RenderPass

```c
struct RenderPassInfo {
    Camera* camera;
    DepthBufferState depthBufferState;
    RID* renderTargets;
    int numRenderTargets;
};
```

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

## BlendState

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