//#include "..\..\diesel.h"
#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#define DS_IMPLEMENTATION
#include "Particlesystem.h"
#define SPRITE_IMPLEMENTATION
#include "..\common\SpriteBatchBuffer.h"
#include "..\common\imgui.h"

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
	rs.width = 1280;
	rs.height = 720;
	rs.title = "2D particle system demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.supportDebug = true;
	ds::init(rs);

	ParticlesystemDescriptor explosionDescriptor;
	explosionDescriptor.maxParticles = 4096;
	explosionDescriptor.particleDimension = ds::vec2(64, 64);
	explosionDescriptor.scale = ds::vec2(0.2f, 0.2f);
	//explosionDescriptor.growth = ds::vec2(-0.5f, -0.5f);
	explosionDescriptor.startColor = ds::Color(222, 222, 255, 255);
	explosionDescriptor.endColor = ds::Color(0, 222, 255, 64);
	explosionDescriptor.textureRect = ds::vec4(0, 0, 64, 64) / 64.0f;

	ParticlesystemDescriptor sparksDescriptor;
	sparksDescriptor.maxParticles = 1024;
	sparksDescriptor.particleDimension = ds::vec2(64, 64);
	sparksDescriptor.scale = ds::vec2(1.8f, 0.1f);
	//sparksDescriptor.growth = ds::vec2(-0.9f, 0.0f);
	sparksDescriptor.startColor = ds::Color(222, 222, 255, 255);
	sparksDescriptor.endColor = ds::Color(0, 222, 255, 64);
	sparksDescriptor.textureRect = ds::vec4(0, 0, 64, 64) / 64.0f;
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("particles.png", &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	explosionDescriptor.textureID = ds::createTexture(texInfo);	
	sparksDescriptor.textureID = explosionDescriptor.textureID;
	stbi_image_free(data);
	// prepare IMGUI

	ParticleManager particles(4096, explosionDescriptor.textureID);

	EmitterSettings explosionSettings;
	explosionSettings.count = 256;
	explosionSettings.angleVariance = 0.5f;
	explosionSettings.radius = 0.0f;
	explosionSettings.radiusVariance = 0.0f;
	explosionSettings.ttl = ds::vec2(0.6f, 0.9f);
	explosionSettings.velocity = ds::vec2(0.0f);
	explosionSettings.velocityVariance = ds::vec2(30.0f, 180.0f);
	explosionSettings.size = ds::vec2(0.6f, 0.6f);
	explosionSettings.sizeVariance = ds::vec2(0.05f,0.05f);
	explosionSettings.acceleration = ds::vec2(-0.1f, -0.1f);
	explosionSettings.growth = ds::vec2(-0.5f, -0.5f);

	EmitterSettings innerExplosionSettings;
	innerExplosionSettings.count = 256;
	innerExplosionSettings.angleVariance = 0.8f;
	innerExplosionSettings.radius = 0.0f;
	innerExplosionSettings.radiusVariance = 0.0f;
	innerExplosionSettings.ttl = ds::vec2(0.5f, 1.1f);
	innerExplosionSettings.velocity = ds::vec2(0.0f);
	innerExplosionSettings.velocityVariance = ds::vec2(600.0f, 800.0f);
	innerExplosionSettings.size = ds::vec2(0.5f, 0.5f);
	innerExplosionSettings.sizeVariance = ds::vec2(0.05f, 0.05f);
	innerExplosionSettings.acceleration = ds::vec2(-0.6f, -0.6f);
	innerExplosionSettings.growth = ds::vec2(-0.5f, -0.5f);

	EmitterSettings outerExplosionSettings;
	outerExplosionSettings.count = 128;
	outerExplosionSettings.angleVariance = 0.8f;
	outerExplosionSettings.radius = 120.0f;
	outerExplosionSettings.radiusVariance = 10.0f;
	outerExplosionSettings.ttl = ds::vec2(0.5f, 1.1f);
	outerExplosionSettings.velocity = ds::vec2(0.0f);
	outerExplosionSettings.velocityVariance = ds::vec2(50.0f, 80.0f);
	outerExplosionSettings.size = ds::vec2(0.5f, 0.5f);
	outerExplosionSettings.sizeVariance = ds::vec2(0.05f, 0.05f);
	outerExplosionSettings.acceleration = ds::vec2(0.f);
	outerExplosionSettings.growth = ds::vec2(-0.25f, -0.25f);

	EmitterSettings heavyStreaks;
	heavyStreaks.count = 128;
	heavyStreaks.angleVariance = 0.2f;
	heavyStreaks.radius = 50.0f;
	heavyStreaks.radiusVariance = 10.0f;
	heavyStreaks.ttl = ds::vec2(0.6f, 0.9f);
	heavyStreaks.velocity = ds::vec2(0.0f);
	heavyStreaks.velocityVariance = ds::vec2(120.0f, 360.0f);
	heavyStreaks.size = ds::vec2(0.6f, 0.1f);
	heavyStreaks.sizeVariance = ds::vec2(1.2f, 0.0f);
	heavyStreaks.acceleration = ds::vec2(0.0f);
	heavyStreaks.growth = ds::vec2(1.0f, 0.0f);

	EmitterSettings lightStreaks;
	lightStreaks.count = 128;
	lightStreaks.angleVariance = 0.3f;
	lightStreaks.radius = 120.0f;
	lightStreaks.radiusVariance = 10.0f;
	lightStreaks.ttl = ds::vec2(0.6f, 0.9f);
	lightStreaks.velocity = ds::vec2(0.0f);
	lightStreaks.velocityVariance = ds::vec2(60.0f, 240.0f);
	lightStreaks.size = ds::vec2(0.6f, 0.05f);
	lightStreaks.sizeVariance = ds::vec2(0.4f, 0.0f);
	lightStreaks.acceleration = ds::vec2(0.0f);
	lightStreaks.growth = ds::vec2(0.4f, 0.0f);

	int explosionID = particles.add(&explosionDescriptor);
	int sparksID = particles.add(&sparksDescriptor);
	
	float t = 1.1f;

	while (ds::isRunning()) {

		t += static_cast<float>(ds::getElapsedSeconds());
		// every second emit some particles at random position
		if (t >= 1.0f) {
			t -= 1.0f;
			float radius = ds::random(25.0f,100.0f);
			float px = ds::random(200.0f, 800.0f);
			float py = ds::random(200.0, 500.0f);
			particles.emitt(explosionID, ds::vec2(px, py), explosionSettings);
			particles.emitt(explosionID, ds::vec2(px, py), innerExplosionSettings);
			particles.emitt(explosionID, ds::vec2(px, py), outerExplosionSettings);
			particles.emitt(sparksID, ds::vec2(px, py), heavyStreaks);
			particles.emitt(sparksID, ds::vec2(px, py), lightStreaks);
		}			
		ds::begin();
		particles.tick(static_cast<float>(ds::getElapsedSeconds()));
		particles.render();

		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());

		ds::end();
	}
	ds::shutdown();
	return 0;
}