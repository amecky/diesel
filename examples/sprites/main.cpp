#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "Sprites_VS_Main.h"
#include "Sprites_PS_Main.h"

const int MAX_SPRITES = 1024;

// based on the presentation: https://de.slideshare.net/DevCentralAMD/vertex-shader-tricks-bill-bilodeau by Bill Bilodeau

struct SpriteBuffer {

	ds::vec2* positions;
	ds::vec2* scales;
	float* rotations;
	ds::vec4* textures;
	ds::Color* colors;
	ds::vec2* velocities;
	char* data;
	int total;

};

void allocateSpriteBuffer(SpriteBuffer* buffer) {
	int size = MAX_SPRITES * (sizeof(ds::vec2) + sizeof(ds::vec2) + sizeof(float) + sizeof(ds::vec4) + sizeof(ds::Color) + sizeof(ds::vec2));
	buffer->data = new char[size];
	buffer->positions = (ds::vec2*)(buffer->data);
	buffer->scales = (ds::vec2*)(buffer->positions + MAX_SPRITES);
	buffer->rotations = (float*)(buffer->scales + MAX_SPRITES);
	buffer->textures = (ds::vec4*)(buffer->rotations + MAX_SPRITES);
	buffer->colors = (ds::Color*)(buffer->textures + MAX_SPRITES);
	buffer->velocities = (ds::vec2*)(buffer->colors + MAX_SPRITES);
	buffer->total = size;
}


struct GPUSprite {
	ds::vec2 position; // x, y world position
	float rotation;
	float scaling;
	ds::vec4 texture;
};

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct SpriteConstantBuffer {
	ds::vec4 screenDimension;
	ds::vec4 screenCenter;
	ds::matrix wvp;
};

// ---------------------------------------------------------------
// get angle between two ds::vec2 vectors
// ---------------------------------------------------------------
float getAngle(const ds::vec2& u, const ds::vec2& v) {
	double x = v.x - u.x;
	double y = v.y - u.y;
	double ang = atan2(y, x);
	return (float)ang;
}

// ---------------------------------------------------------------
// create sprite
// ---------------------------------------------------------------
int add(const ds::vec2& p, const ds::vec4& r, SpriteBuffer* sprites, int index) {
	if ((index + 1) < MAX_SPRITES) {
		float angle = ds::random(0.0f, ds::PI * 2.0f);
		sprites->positions[index] = p;
		sprites->colors[index] = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
		sprites->scales[index] = ds::vec2(1.0f, 1.0f);
		sprites->rotations[index] = angle;
		sprites->textures[index] = r;
		float vel = ds::random(100.0f,250.0f);
		float vx = cos(angle) * vel;
		float vy = sin(angle) * vel;
		sprites->velocities[index] = ds::vec2(vx,vy);
		return index + 1;
	}
	return index;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	SpriteBuffer spriteBuffer;
	allocateSpriteBuffer(&spriteBuffer);

	//Sprite sprites[MAX_SPRITES];
	int numSprites = 0;
	// create some sprites
	for (int i = 0; i < MAX_SPRITES; ++i) {
		float x = ds::random(100.0f, 900.0f);
		float y = ds::random(100.0f, 700.0f);
		int r = ds::random(0.0, 1.9f);
		int c = ds::random(0.0, 1.9f);
		numSprites = add(ds::vec2(x, y), ds::vec4(r * 40, c * 40, 40, 40), &spriteBuffer, numSprites);
	}

	SpriteConstantBuffer constantBuffer;
	constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, 128.0f, 128.0f);
	constantBuffer.screenCenter = ds::vec4(512.0f, 384.0f, 0.0f, 0.0f);
	float t = 0.0f;

	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Sprites demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);

	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("cubes.png", &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(texInfo);
	stbi_image_free(data);


	ds::ShaderInfo vsInfo = { 0, Sprites_VS_Main, sizeof(Sprites_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID vertexShader = ds::createShader(vsInfo);
	ds::ShaderInfo psInfo = { 0, Sprites_PS_Main, sizeof(Sprites_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID pixelShader = ds::createShader(psInfo);
		
	RID cbid = ds::createConstantBuffer(sizeof(SpriteConstantBuffer), &constantBuffer);
	int indices[] = { 0,1,2,1,3,2 };
	RID idxBuffer = ds::createQuadIndexBuffer(MAX_SPRITES,indices);
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);

	ds::StructuredBufferInfo sbInfo;
	sbInfo.cpuWritable = true;
	sbInfo.data = 0;
	sbInfo.elementSize = sizeof(GPUSprite);
	sbInfo.numElements = 4096;
	sbInfo.gpuWritable = false;
	sbInfo.renderTarget = NO_RID;
	sbInfo.textureID = NO_RID;
	RID sbID = ds::createStructuredBuffer(sbInfo);

	GPUSprite* gpuSprites = new GPUSprite[4096];

	// create orthographic view
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(ds::getScreenWidth(), ds::getScreenHeight(), 0.0f, 1.0f);
	ds::Camera camera = {
		orthoView,
		orthoProjection,
		orthoView * orthoProjection,
		ds::vec3(0,0,0),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	ds::RenderPassInfo rpInfo = { &camera, ds::DepthBufferState::DISABLED, 0, 0 };
	RID orthoPass = ds::createRenderPass(rpInfo);
	constantBuffer.wvp = ds::matTranspose(camera.viewProjectionMatrix);

	RID stateGroup = ds::StateGroupBuilder()
		.constantBuffer(cbid, vertexShader)
		.structuredBuffer(sbID,vertexShader,1)
		.vertexBuffer(NO_RID)
		.vertexShader(vertexShader)
		.indexBuffer(idxBuffer)
		.pixelShader(pixelShader)
		.samplerState(ssid, pixelShader)
		.texture(textureID, pixelShader, 0)
		.build();

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST, 0 };

	RID drawItem = ds::compile(drawCmd, stateGroup);

	while (ds::isRunning()) {
		// move sprites
		ds::vec2* positions = spriteBuffer.positions;
		ds::vec2* velocities = spriteBuffer.velocities;
		float* rotations = spriteBuffer.rotations;
		ds::vec2* scales = spriteBuffer.scales;
		ds::vec4* tex = spriteBuffer.textures;
		GPUSprite* current = gpuSprites;
		for (int i = 0; i < numSprites; ++i) {
			*positions += *velocities * static_cast<float>(ds::getElapsedSeconds());
			if (positions->x < 10.0f || positions->x > 1000.0f) {
				velocities->x *= -1.0f;
				*positions += *velocities * static_cast<float>(ds::getElapsedSeconds());
				*rotations = getAngle(*velocities, ds::vec2(1, 0));
			}
			if (positions->y < 10.0f || positions->y > 760.0f) {
				velocities->y *= -1.0f;
				*positions += *velocities * static_cast<float>(ds::getElapsedSeconds());
				*rotations = getAngle(*velocities, ds::vec2(1, 0));
			}		
			current->position = *positions;
			current->rotation = *rotations;
			current->scaling = 1.0f;
			current->texture = *tex;
			++positions;
			++velocities;
			++rotations;
			++scales;
			++tex;
			++current;
		}
			
		ds::begin();
		ds::mapBufferData(sbID, gpuSprites, numSprites * sizeof(GPUSprite));
		drawCmd.size = numSprites;
		ds::submit(orthoPass, drawItem, numSprites * 6);
		ds::end();
	}
	delete[] gpuSprites;
	delete[] spriteBuffer.data;
	ds::shutdown();
	return 0;
}