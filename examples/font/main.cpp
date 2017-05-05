#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
struct SpriteVertex {

	ds::vec3 position;
	ds::vec4 texture;
	ds::vec3 size;
	ds::Color color;

};

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct SpriteConstantBuffer {
	ds::vec4 screenDimension;
	ds::vec4 screenCenter;
	ds::matrix wvp;
};

SpriteVertex vertices[1024];
int numVertices;

// ---------------------------------------------------------------
// add text
// ---------------------------------------------------------------
void addText(const ds::vec2& pos, const char* text) {
	float xpos = pos.x;
	float ypos = pos.y;
	const char* current = text;
	for (int i = 0; i < strlen(text); ++i) {
		int c = current[i] - 32;
		int t = c / 16;
		int y = t * 20;
		int x = (c - t * 16) * 16;
		float dimX = 32;
		float dimY = 32;
		float th = 32;
		if ((numVertices + 1) < 1024) {
			vertices[numVertices++] = { ds::vec3(xpos,ypos,0.0f),ds::vec4(x, y, 14.0f, 20.0f),ds::vec3(1.0f,1.0f,0.0f),ds::Color(255,255,255,255) };
		}
		xpos += 12;
	}
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	numVertices = 0;

	addText(ds::vec2(300.0f, 500.0f), "More text is here");
	addText(ds::vec2(100.0f, 100.0f), "Here is also some text");
	addText(ds::vec2(50.0f, 300.0f), "The magic number 42 is less than 123099");

	SpriteConstantBuffer constantBuffer;
	constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, 256.0f, 256.0f);
	constantBuffer.screenCenter = ds::vec4(512.0f, 384.0f, 0.0f, 0.0f);
	float t = 0.0f;

	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("nk_57_monospace.png", &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(texInfo);
	stbi_image_free(data);


	ds::BlendStateInfo blendInfo = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
	RID bs_id = ds::createBlendState(blendInfo);

	ds::ShaderInfo vsInfo = { "Font_vs.cso", 0, 0, ds::ShaderType::ST_VERTEX_SHADER };
	RID vertexShader = ds::createShader(vsInfo);
	ds::ShaderInfo psInfo = { "Font_ps.cso", 0, 0, ds::ShaderType::ST_PIXEL_SHADER };
	RID pixelShader = ds::createShader(psInfo);
	ds::ShaderInfo gsInfo = { "Font_gs.cso", 0, 0, ds::ShaderType::ST_GEOMETRY_SHADER};
	RID geoShader = ds::createShader(gsInfo);

	// very special buffer layout 
	ds::InputLayoutDefinition decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	ds::InputLayoutInfo layoutInfo = {decl, 4, vertexShader};
	RID vertexDeclId = ds::createInputLayout(layoutInfo);
		
	RID cbid = ds::createConstantBuffer(sizeof(SpriteConstantBuffer), &constantBuffer);
	ds::VertexBufferInfo vbInfo = { ds::BufferType::DYNAMIC, 1024, sizeof(SpriteVertex) };
	RID vertexBufferID = ds::createVertexBuffer(vbInfo);
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);

	// create orthographic view
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	ds::Camera camera = {
		orthoView,
		orthoProjection,
		orthoView * orthoProjection,
		ds::vec3(0,3,-6),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	ds::RenderPassInfo rpInfo = { &camera, ds::DepthBufferState::DISABLED, 0, 0 };
	RID orthoPass = ds::createRenderPass(rpInfo);

	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(vertexDeclId)
		.constantBuffer(cbid, geoShader, 0)
		.blendState(bs_id)
		.samplerState(ssid, pixelShader)
		.vertexBuffer(vertexBufferID)
		.vertexShader(vertexShader)
		.geometryShader(geoShader)
		.pixelShader(pixelShader)
		.texture(textureID, pixelShader, 0)
		.build();

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST };

	RID drawItem = ds::compile(drawCmd, stateGroup);

	while (ds::isRunning()) {
		ds::begin();
		ds::matrix w = ds::matIdentity();
		constantBuffer.wvp = ds::matTranspose(camera.viewProjectionMatrix);
		ds::mapBufferData(vertexBufferID, vertices, numVertices * sizeof(SpriteVertex));
		ds::submit(orthoPass, drawItem, numVertices);
		ds::end();
	}
	ds::shutdown();
}