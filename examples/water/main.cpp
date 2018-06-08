#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "Water_VS_Main.h"
#include "Water_PS_Main.h"

// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(ds::TextureDesc()
		.Width(x)
		.Height(y)
		.Channels(n)
		.Data(data)
		.Format(ds::TextureFormat::R8G8B8A8_UNORM)
		.BindFlags(ds::BindFlag::BF_SHADER_RESOURCE));
	stbi_image_free(data);
	return textureID;
}

// ---------------------------------------------------------------
// the cube constant buffer
// ---------------------------------------------------------------
struct WaterConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
};

struct WaterPixelConstantBuffer {
	ds::vec4 data;
};

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {
	ds::vec3 p;
	ds::Color color;
	ds::vec2 uv;
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
	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 4.0f, -4.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
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
		.Data(Water_VS_Main)
		.DataSize(sizeof(Water_VS_Main))
		.ShaderType(ds::ShaderType::ST_VERTEX_SHADER)
	);

	RID pixelShader = ds::createShader(ds::ShaderDesc()
		.Data(Water_PS_Main)
		.DataSize(sizeof(Water_PS_Main))
		.ShaderType(ds::ShaderType::ST_PIXEL_SHADER)
	);

	// create buffer input layout
	ds::InputLayoutDefinition decl[] = {
		{ "POSITION", 0, ds::BufferAttributeType::FLOAT3 },
		{ "COLOR", 0, ds::BufferAttributeType::FLOAT4 },
		{ "TEXCOORD", 0, ds::BufferAttributeType::FLOAT2 }
	};

	float tiling = 4.0f;
	float size = 2.0f;
	Vertex v[] = {
		{ ds::vec3(-size, 0.0f, -size), ds::Color(255,0,0,255), ds::vec2(0.0f,tiling) },
		{ ds::vec3(-size, 0.0f,  size), ds::Color(255,0,0,255), ds::vec2(0.0f,0.0f) },
		{ ds::vec3( size, 0.0f,  size), ds::Color(255,0,0,255), ds::vec2(tiling,0.0f) },
		{ ds::vec3( size, 0.0f,  size), ds::Color(255,0,0,255), ds::vec2(tiling,0.0f) },
		{ ds::vec3( size, 0.0f, -size), ds::Color(255,0,0,255), ds::vec2(tiling,tiling) },
		{ ds::vec3(-size, 0.0f, -size), ds::Color(255,0,0,255), ds::vec2(0.0f,tiling) }
	};
	
	RID rid = ds::createInputLayout(ds::InputLayoutDesc()
		.Declarations(decl)
		.NumDeclarations(3)
		.VertexShader(vertexShader)
	);

	RID vbid = ds::createVertexBuffer(ds::VertexBufferDesc()
		.BufferType(ds::BufferType::STATIC)
		.Data(v)
		.NumVertices(6)
		.VertexSize(sizeof(Vertex))
	);

	RID texture = loadImage("cube_map.png");
	RID flowTexture = loadImage("flowmap.png");

	RID bs_id = ds::createBlendState(ds::BlendStateDesc()
		.SrcBlend(ds::BlendStates::SRC_ALPHA)
		.SrcAlphaBlend(ds::BlendStates::SRC_ALPHA)
		.DestBlend(ds::BlendStates::INV_SRC_ALPHA)
		.DestAlphaBlend(ds::BlendStates::INV_SRC_ALPHA)
		.AlphaEnabled(true)
	);

	RID ssid = ds::createSamplerState(ds::SamplerStateDesc()
		.AddressMode(ds::TextureAddressModes::WRAP)
		.Filter(ds::TextureFilters::LINEAR)
	);

	WaterConstantBuffer constantBuffer;
	RID cbID = ds::createConstantBuffer(sizeof(WaterConstantBuffer), &constantBuffer);

	WaterPixelConstantBuffer pixelBuffer;
	pixelBuffer.data = ds::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	RID pbID = ds::createConstantBuffer(sizeof(WaterPixelConstantBuffer), &pixelBuffer);
	//
	// create state group
	//
	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.blendState(bs_id)
		.samplerState(ssid,pixelShader)
		.constantBuffer(cbID,vertexShader)
		.constantBuffer(pbID, pixelShader, 1)
		.vertexBuffer(vbid)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.texture(texture,pixelShader,0)
		.texture(flowTexture, pixelShader, 1)
		.build();
	//
	// the draw command
	//
	ds::DrawCommand drawCmd = { 6, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };
	//
	// and finally we can create the draw item
	//
	RID drawItem = ds::compile(drawCmd, stateGroup);

	float timer = 0.0f;

	while (ds::isRunning()) {

		ds::begin();

		pixelBuffer.data.x = timer * 0.25f;// abs(sinf(timer));

		timer += ds::getElapsedSeconds();

		constantBuffer.viewprojectionMatrix = ds::matTranspose(camera.viewProjectionMatrix);
		constantBuffer.worldMatrix = ds::matTranspose(ds::matIdentity());

		ds::submit(basicPass, drawItem);

		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());

		ds::end();
	}
	ds::shutdown();
	return 0;
}