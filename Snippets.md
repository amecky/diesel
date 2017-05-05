# Snippets

A collection of code snippets

## BlendState

```c
ds::BlendStateInfo blendInfo = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
RID bs_id = ds::createBlendState(blendInfo);
```

## VertexBuffer

```c
ds::VertexBufferInfo vbInfo = { ds::BufferType::STATIC, num, sizeof(ObjVertex), vertices };
RID vbid = ds::createVertexBuffer(vbInfo);
```

## Shader from cso

```c
ds::ShaderInfo vsInfo = { "Grid_vs.cso", 0, 0, ds::ShaderType::ST_VERTEX_SHADER };
RID vertexShader = ds::createShader(vsInfo);
ds::ShaderInfo psInfo = { "Grid_ps.cso", 0, 0, ds::ShaderType::ST_PIXEL_SHADER };
RID pixelShader = ds::createShader(psInfo);
```

## InputLayout

```c
ds::InputLayoutInfo layoutInfo = { decl, 3, vertexShader };
RID rid = ds::createInputLayout(layoutInfo);
```

## SamplerState

```c
ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
RID ssid = ds::createSamplerState(samplerInfo);
```

## Texture

```c
ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
RID textureID = ds::createTexture(texInfo);
```

## Camera and RenderPass

```c
ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 3.0f, -6.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
ds::Camera camera = {
    viewMatrix,
    projectionMatrix,
    viewMatrix * projectionMatrix,
    ds::vec3(0,3,-6),
    ds::vec3(0,0,1),
    ds::vec3(0,1,0),
    ds::vec3(1,0,0),
    0.0f,
    0.0f,
    0.0f
};
ds::RenderPassInfo rpInfo = { &camera, ds::DepthBufferState::ENABLED, 0, 0 };
RID basicPass = ds::createRenderPass(rpInfo);
```