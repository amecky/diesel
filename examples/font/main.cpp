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
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);


	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	RID vertexShader = ds::loadVertexShader("Font_vs.cso");
	RID pixelShader = ds::loadPixelShader("Font_ps.cso");
	RID geoShader = ds::loadGeometryShader("Font_gs.cso");

	// very special buffer layout 
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	RID vertexDeclId = ds::createVertexDeclaration(decl, 4, vertexShader);
		
	RID cbid = ds::createConstantBuffer(sizeof(SpriteConstantBuffer), &constantBuffer);
	RID vertexBufferID = ds::createVertexBuffer(ds::BufferType::DYNAMIC, 1024, sizeof(SpriteVertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::POINT);

	// create orthographic view
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	ds::matrix viewprojectionMatrix = orthoView * orthoProjection;
	RID orthoPass = ds::createRenderPass(orthoView, orthoProjection, ds::DepthBufferState::DISABLED);

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
		constantBuffer.wvp = ds::matTranspose(viewprojectionMatrix);
		ds::mapBufferData(vertexBufferID, vertices, numVertices * sizeof(SpriteVertex));
		ds::submit(orthoPass, drawItem, numVertices);
		ds::end();
	}
	ds::shutdown();
}