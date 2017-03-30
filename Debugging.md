# Debugging resources

Every method to create a resource accepts an optional last parameter called name.
This name is mainly used for debugging. You can call:

```c
void saveResourcesToFile(const char* fileName = "resources.txt");
```

to get a list of all resources and the draw items. For every draw item you can see the number
of groups in the correct order how they will be applied.

Here is an example from the "Texture" example:

```
 index | resource type       | Name
--------------------------------------------------------------
   0  | BLENDSTATE           | DefaultBlendState
   1  | SAMPLER_STATE        | DefaultSamplerState
   2  | RASTERIZER_STATE     | DefaultRasterizerState
   3  | CONSTANT_BUFFER      | BasicConstantBuffer
   4  | STATE_GROUP          | DefaultGroup
   5  | SRV                  | DebugTextFont
   6  | VERTEX_SHADER        | DebugVS
   7  | PIXEL_SHADER         | DebugPS
   8  | GEOMETRY_SHADER      | DebugGS
   9  | VERTEX_DECLARATION   | PCC_Layout
  10  | CONSTANT_BUFFER      | DebugTextConstantBuffer
  11  | VERTEX_BUFFER        | DebugTextVertexBuffer
  12  | SAMPLER_STATE        | UNKNOWN
  13  | STATE_GROUP          | StateGroup
  14  | DRAW_ITEM            | DebugText
  15  | RENDER_PASS          | DebugTextOrthoPass
  16  | RENDER_PASS          | BasicPass
  17  | SRV                  | ..\common\cube_map.png
  18  | SRV                  | ..\common\grid.png
  19  | VERTEX_SHADER        | TextureVS
  20  | PIXEL_SHADER         | TexturePS
  21  | VERTEX_DECLARATION   | PT_Layout
  22  | CONSTANT_BUFFER      | GridConstantBuffer
  23  | INDEX_BUFFER         | GridIndexBuffer
  24  | VERTEX_BUFFER        | GridVertexBuffer
  25  | SAMPLER_STATE        | UNKNOWN
  26  | STATE_GROUP          | StateGroup
  27  | DRAW_ITEM            | Grid
  28  | VERTEX_DECLARATION   | PosTex_Layout
  29  | CONSTANT_BUFFER      | CubeConstantBuffer
  30  | INDEX_BUFFER         | IndexBuffer
  31  | VERTEX_BUFFER        | CubeBuffer
  32  | VERTEX_BUFFER        | StaticCubes
  33  | SAMPLER_STATE        | CLAMP_LINEAR_SAMPLER
  34  | STATE_GROUP          | BasicGroup
  35  | STATE_GROUP          | StaticCubeGroup
  36  | STATE_GROUP          | CubeGroup
  37  | DRAW_ITEM            | StaticItem
  38  | DRAW_ITEM            | CubeItem
  39  | SRV                  | ..\common\imgui.png
  40  | VERTEX_SHADER        | SpriteVS
  41  | PIXEL_SHADER         | SpritePS
  42  | GEOMETRY_SHADER      | SpriteGS
  43  | VERTEX_DECLARATION   | PCNC_Layout
  44  | CONSTANT_BUFFER      | SpriteBufferConstantBuffer
  45  | VERTEX_BUFFER        | UNKNOWN
  46  | SAMPLER_STATE        | UNKNOWN
  47  | STATE_GROUP          | StateGroup
  48  | DRAW_ITEM            | SpriteBuffer
  49  | RENDER_PASS          | SpriteOrthoPass


DrawItem 14 (DebugText) - groups: 2
Group: 13 (StateGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
VERTEX_DECLARATION   |     9 | IA       |  0   | PCC_Layout
INDEX_BUFFER         | 65534 | IA       |  0   | NO_RID
VERTEX_BUFFER        |    11 | IA       |  0   | DebugTextVertexBuffer
GEOMETRY_SHADER      |     8 | GS       |  0   | DebugGS
CONSTANT_BUFFER      |    10 | GS_RES   |  0   | DebugTextConstantBuffer
VERTEX_SHADER        |     6 | PS       |  0   | DebugVS
PIXEL_SHADER         |     7 | PS       |  0   | DebugPS
SAMPLER_STATE        |    12 | PS_RES   |  0   | UNKNOWN
SRV                  |     5 | PS_RES   |  0   | DebugTextFont
Group: 4 (DefaultGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
INDEX_BUFFER         | 65534 | IA       |  0   | NO_RID
VERTEX_BUFFER        | 65534 | IA       |  0   | NO_RID
SRV                  | 65534 | VS_RES   |  0   | NO_RID
GEOMETRY_SHADER      | 65534 | GS       |  0   | NO_RID
SRV                  | 65534 | GS_RES   |  0   | NO_RID
RASTERIZER_STATE     |     2 | RS       |  0   | DefaultRasterizerState
VERTEX_SHADER        | 65534 | PS       |  0   | NO_RID
PIXEL_SHADER         | 65534 | PS       |  0   | NO_RID
SRV                  | 65534 | PS_RES   |  0   | NO_RID
BLENDSTATE           |     0 | OM       |  0   | DefaultBlendState

DrawItem 27 (Grid) - groups: 2
Group: 26 (StateGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
VERTEX_DECLARATION   |    21 | IA       |  0   | PT_Layout
INDEX_BUFFER         |    23 | IA       |  0   | GridIndexBuffer
VERTEX_BUFFER        |    24 | IA       |  0   | GridVertexBuffer
CONSTANT_BUFFER      |    22 | VS_RES   |  0   | GridConstantBuffer
VERTEX_SHADER        |    19 | PS       |  0   | TextureVS
PIXEL_SHADER         |    20 | PS       |  0   | TexturePS
SAMPLER_STATE        |    25 | PS_RES   |  0   | UNKNOWN
SRV                  |    18 | PS_RES   |  0   | ..\common\grid.png
Group: 4 (DefaultGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
INDEX_BUFFER         | 65534 | IA       |  0   | NO_RID
VERTEX_BUFFER        | 65534 | IA       |  0   | NO_RID
SRV                  | 65534 | VS_RES   |  0   | NO_RID
GEOMETRY_SHADER      | 65534 | GS       |  0   | NO_RID
SRV                  | 65534 | GS_RES   |  0   | NO_RID
RASTERIZER_STATE     |     2 | RS       |  0   | DefaultRasterizerState
VERTEX_SHADER        | 65534 | PS       |  0   | NO_RID
PIXEL_SHADER         | 65534 | PS       |  0   | NO_RID
SRV                  | 65534 | PS_RES   |  0   | NO_RID
BLENDSTATE           |     0 | OM       |  0   | DefaultBlendState

DrawItem 37 (StaticItem) - groups: 3
Group: 34 (BasicGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
VERTEX_DECLARATION   |    28 | IA       |  0   | PosTex_Layout
INDEX_BUFFER         |    30 | IA       |  0   | IndexBuffer
CONSTANT_BUFFER      |    29 | VS_RES   |  0   | CubeConstantBuffer
VERTEX_SHADER        |    19 | PS       |  0   | TextureVS
PIXEL_SHADER         |    20 | PS       |  0   | TexturePS
SAMPLER_STATE        |    33 | PS_RES   |  0   | CLAMP_LINEAR_SAMPLER
SRV                  |    17 | PS_RES   |  0   | ..\common\cube_map.png
BLENDSTATE           |     0 | OM       |  0   | DefaultBlendState
Group: 35 (StaticCubeGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
VERTEX_BUFFER        |    32 | IA       |  0   | StaticCubes
Group: 4 (DefaultGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
INDEX_BUFFER         | 65534 | IA       |  0   | NO_RID
VERTEX_BUFFER        | 65534 | IA       |  0   | NO_RID
SRV                  | 65534 | VS_RES   |  0   | NO_RID
GEOMETRY_SHADER      | 65534 | GS       |  0   | NO_RID
SRV                  | 65534 | GS_RES   |  0   | NO_RID
RASTERIZER_STATE     |     2 | RS       |  0   | DefaultRasterizerState
VERTEX_SHADER        | 65534 | PS       |  0   | NO_RID
PIXEL_SHADER         | 65534 | PS       |  0   | NO_RID
SRV                  | 65534 | PS_RES   |  0   | NO_RID
BLENDSTATE           |     0 | OM       |  0   | DefaultBlendState

DrawItem 38 (CubeItem) - groups: 3
Group: 34 (BasicGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
VERTEX_DECLARATION   |    28 | IA       |  0   | PosTex_Layout
INDEX_BUFFER         |    30 | IA       |  0   | IndexBuffer
CONSTANT_BUFFER      |    29 | VS_RES   |  0   | CubeConstantBuffer
VERTEX_SHADER        |    19 | PS       |  0   | TextureVS
PIXEL_SHADER         |    20 | PS       |  0   | TexturePS
SAMPLER_STATE        |    33 | PS_RES   |  0   | CLAMP_LINEAR_SAMPLER
SRV                  |    17 | PS_RES   |  0   | ..\common\cube_map.png
BLENDSTATE           |     0 | OM       |  0   | DefaultBlendState
Group: 36 (CubeGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
VERTEX_BUFFER        |    31 | IA       |  0   | CubeBuffer
Group: 4 (DefaultGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
INDEX_BUFFER         | 65534 | IA       |  0   | NO_RID
VERTEX_BUFFER        | 65534 | IA       |  0   | NO_RID
SRV                  | 65534 | VS_RES   |  0   | NO_RID
GEOMETRY_SHADER      | 65534 | GS       |  0   | NO_RID
SRV                  | 65534 | GS_RES   |  0   | NO_RID
RASTERIZER_STATE     |     2 | RS       |  0   | DefaultRasterizerState
VERTEX_SHADER        | 65534 | PS       |  0   | NO_RID
PIXEL_SHADER         | 65534 | PS       |  0   | NO_RID
SRV                  | 65534 | PS_RES   |  0   | NO_RID
BLENDSTATE           |     0 | OM       |  0   | DefaultBlendState

DrawItem 48 (SpriteBuffer) - groups: 2
Group: 47 (StateGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
VERTEX_DECLARATION   |    43 | IA       |  0   | PCNC_Layout
INDEX_BUFFER         | 65534 | IA       |  0   | NO_RID
VERTEX_BUFFER        |    45 | IA       |  0   | UNKNOWN
GEOMETRY_SHADER      |    42 | GS       |  0   | SpriteGS
CONSTANT_BUFFER      |    44 | GS_RES   |  0   | SpriteBufferConstantBuffer
VERTEX_SHADER        |    40 | PS       |  0   | SpriteVS
PIXEL_SHADER         |    41 | PS       |  0   | SpritePS
SAMPLER_STATE        |    46 | PS_RES   |  0   | UNKNOWN
SRV                  |    39 | PS_RES   |  0   | ..\common\imgui.png
Group: 4 (DefaultGroup)
resource type        | id    | stage    | slot | Name
------------------------------------------------------------------------------------------
INDEX_BUFFER         | 65534 | IA       |  0   | NO_RID
VERTEX_BUFFER        | 65534 | IA       |  0   | NO_RID
SRV                  | 65534 | VS_RES   |  0   | NO_RID
GEOMETRY_SHADER      | 65534 | GS       |  0   | NO_RID
SRV                  | 65534 | GS_RES   |  0   | NO_RID
RASTERIZER_STATE     |     2 | RS       |  0   | DefaultRasterizerState
VERTEX_SHADER        | 65534 | PS       |  0   | NO_RID
PIXEL_SHADER         | 65534 | PS       |  0   | NO_RID
SRV                  | 65534 | PS_RES   |  0   | NO_RID
BLENDSTATE           |     0 | OM       |  0   | DefaultBlendState
```