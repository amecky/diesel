//#include "..\..\diesel.h"
#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\common\stb_image.h"
#define DS_IMPLEMENTATION
#include "Particlesystem.h"
#include "..\common\Grid.h"
#include "..\common\Camera.h"
#include "..\common\Textured_VS_Main.h"
#include "..\common\Textured_PS_Main.h"
// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(texInfo);
	stbi_image_free(data);
	return textureID;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);

	ParticlesystemDescriptor descriptor;
	descriptor.maxParticles = 1024;
	descriptor.particleDimension = ds::vec2(32, 32);
	// load image using stb_image
	descriptor.texture = loadImage("particles.png");

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 0.0f, -4.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
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
	ds::RenderPassInfo basicInfo = { &camera, ds::DepthBufferState::ENABLED, 0, 0 };
	RID basicPass = ds::createRenderPass(basicInfo);
	ds::RenderPassInfo particlePassInfo = { &camera, ds::DepthBufferState::DISABLED, 0, 0 };
	RID particlePass = ds::createRenderPass(particlePassInfo);
	
	
	descriptor.startColor = ds::Color(192, 192, 0, 255);
	descriptor.endColor = ds::Color(192, 16, 0, 0);

	Particlesystem system(&camera, descriptor, particlePass);

	ParticleDescriptor particleDescriptor;
	particleDescriptor.ttl = 0.4f;
	particleDescriptor.velocity = ds::vec2(20.0f, 0.0f);
	particleDescriptor.friction = 0.5f;
	particleDescriptor.maxScale = ds::vec2(0.02f, 0.02f);
	particleDescriptor.minScale = ds::vec2(0.05f, 0.05f);
	particleDescriptor.acceleration = ds::vec3(0.0f, -1.75f, 0.0f);
	
	int emitter = 64;
	float radius = 0.05f;
	float t = 0.0f;

	RID textureID = loadImage("..\\common\\cube_map.png");

	ds::ShaderInfo vsInfo = { 0, Textured_VS_Main, sizeof(Textured_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID vertexShader = ds::createShader(vsInfo, "TexturedVS");
	ds::ShaderInfo psInfo = { 0, Textured_PS_Main, sizeof(Textured_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID pixelShader = ds::createShader(psInfo, "TexturedPS");

	Grid grid(&camera);
	ds::vec3 gridPositions[] = {
		ds::vec3(-4.0f, -1.0f, -3.5f),
		ds::vec3(-4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f, -3.5f)
	};
	grid.create(gridPositions, 2, vertexShader, pixelShader, textureID, basicPass);

	float dt = 1.0f / 60.0f;
	float frequency = 200.0f * dt;
	float accu = 0.0f;

	FPSCamera fpsCamera(&camera);
	fpsCamera.setPosition(ds::vec3(0, 1, -6));
	fpsCamera.buildView();

	while (ds::isRunning()) {

		fpsCamera.update(static_cast<float>(ds::getElapsedSeconds()));

		t += static_cast<float>(ds::getElapsedSeconds());
		if (t >= dt) {
			accu += frequency;
			while (accu >= 1.0f) {
				accu -= 1.0f;
				float angle = ds::random(0.0f, ds::TWO_PI);
				float x = cos(angle) * radius;
				float y = -1.0f;
				float z = sin(angle) * radius;
				particleDescriptor.ttl = ds::random(1.0f, 1.2f);
				particleDescriptor.velocity = ds::vec3(ds::random(-0.4f,0.4f), ds::random(1.2f, 2.6f), ds::random(-0.4f, 0.4f));
				system.add(ds::vec3(x, y, z), particleDescriptor);
			}
			t -= dt;
		}
		ds::begin();
		grid.render();
		system.tick(static_cast<float>(ds::getElapsedSeconds()));
		system.render();
		ds::end();
	}
	ds::shutdown();
	return 0;
}