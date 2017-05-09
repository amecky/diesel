//#include "..\..\diesel.h"
#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#define DS_IMPLEMENTATION
#include "Particlesystem.h"
#include "..\common\SpriteBuffer.h"
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
/*
struct ExplosionSettings {
	int count;
	ds::vec2 ttl;
	float angleVariance;
	float radiusVariance;
	ds::vec2 velocityVariance;
	ds::vec2 sizeVariance;
};

void emittExplosion(ParticleManager* particles,uint32_t id, const ExplosionSettings& settings, float px, float py, float radius) {
	for (int i = 0; i < settings.count; ++i) {
		float angle = ds::TWO_PI * static_cast<float>(i) / static_cast<float>(settings.count);
		float x = px + cos(angle) * (radius + ds::random(-settings.radiusVariance, settings.radiusVariance));
		float y = py + sin(angle) * (radius + ds::random(-settings.radiusVariance, settings.radiusVariance));
		float da = angle * settings.angleVariance;
		angle += ds::random(-da, da);
		ds::vec2 s = ds::vec2(0.2f, 0.15f);
		float ds = ds::random(settings.sizeVariance.x,settings.sizeVariance.y);
		s *= ds;
		float ttl = ds::random(settings.ttl.x, settings.ttl.y);
		float rotation = angle;
		ds::vec2 velocity = ds::random(settings.velocityVariance.x, settings.velocityVariance.y) * ds::vec2(cos(angle), sin(angle));
		//ds::vec2 acc = ds::vec2(0.0f, -400.0f);
		ds::vec2 acc = velocity * -0.8f;
		particles->add(id, ds::vec2(x, y), velocity, acc, ttl, rotation);
	}
}

struct SparksSettings {
	int count;
	ds::vec2 ttl;
	float angleVariance;
	float radiusVariance;
	ds::vec2 velocityVariance;
	ds::vec2 growth;
};

void emittSparks(ParticleManager* particles, uint32_t id, const SparksSettings& settings, float px, float py, float radius) {
	for (int i = 0; i < settings.count; ++i) {
		float angle = ds::TWO_PI * static_cast<float>(i) / static_cast<float>(settings.count);
		float x = px + cos(angle) * (radius + ds::random(-settings.radiusVariance, settings.radiusVariance));
		float y = py + sin(angle) * (radius + ds::random(-settings.radiusVariance, settings.radiusVariance));
		float da = angle * settings.angleVariance;
		angle += ds::random(-da, da);
		float ttl = ds::random(settings.ttl.x, settings.ttl.y);
		float rotation = angle;
		ds::vec2 velocity = ds::random(settings.velocityVariance.x, settings.velocityVariance.y) * ds::vec2(cos(angle), sin(angle));
		particles->add(id, ds::vec2(x, y), velocity, ds::vec2(0.0f,0.0f), ttl, rotation);
	}
}
*/
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "2D particle system demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);

	ParticlesystemDescriptor explosionDescriptor;
	explosionDescriptor.maxParticles = 4096;
	explosionDescriptor.particleDimension = ds::vec2(64, 64);
	explosionDescriptor.scale = ds::vec2(0.2f, 0.2f);
	explosionDescriptor.growth = ds::vec2(-0.5f, -0.5f);
	//explosionDescriptor.growth = ds::vec2(1.5f, 1.5f);
	explosionDescriptor.startColor = ds::Color(250,250,250, 255);
	explosionDescriptor.endColor = ds::Color(128, 0, 0, 0);
	explosionDescriptor.textureRect = ds::vec4(0, 0, 64, 64) / 64.0f;

	ParticlesystemDescriptor sparksDescriptor;
	sparksDescriptor.maxParticles = 1024;
	sparksDescriptor.particleDimension = ds::vec2(64, 64);
	sparksDescriptor.scale = ds::vec2(1.8f, 0.1f);
	sparksDescriptor.growth = ds::vec2(-0.9f, 0.0f);
	sparksDescriptor.startColor = ds::Color(248, 213, 131, 255);
	sparksDescriptor.endColor = ds::Color(248, 213, 131, 0);
	sparksDescriptor.textureRect = ds::vec4(0, 0, 64, 64) / 64.0f;
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("particles.png", &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	explosionDescriptor.textureID = ds::createTexture(texInfo);	
	sparksDescriptor.textureID = explosionDescriptor.textureID;
	stbi_image_free(data);
	// prepare IMGUI

	RID guiTextureID = loadImage("..\\common\\imgui.png");
	SpriteBuffer spriteBuffer(512, guiTextureID);
	gui::init(&spriteBuffer, guiTextureID);

	ParticleManager particles(4096, explosionDescriptor.textureID);

	EmitterSettings explosionSettings;
	explosionSettings.count = 512;
	explosionSettings.angleVariance = 0.1f;
	explosionSettings.radius = 50.0f;
	explosionSettings.radiusVariance = 10.0f;
	explosionSettings.ttl = ds::vec2(0.6f, 0.9f);
	explosionSettings.velocity = ds::vec2(0.0f);
	explosionSettings.velocityVariance = ds::vec2(200.0f, 300.0f);
	explosionSettings.size = ds::vec2(0.2f, 0.2f);
	explosionSettings.sizeVariance = ds::vec2(0.2f,0.2f);
	explosionSettings.acceleration = explosionSettings.velocity * -0.9f;
	explosionSettings.growth = ds::vec2(-0.5f, -0.5f);

	EmitterSettings sparksSettings;
	sparksSettings.count = 128;
	sparksSettings.angleVariance = 0.2f;
	sparksSettings.radius = 50.0f;
	sparksSettings.radiusVariance = 10.0f;
	sparksSettings.ttl = ds::vec2(0.6f, 0.9f);
	sparksSettings.velocity = ds::vec2(0.0f);
	sparksSettings.velocityVariance = ds::vec2(120.0f, 200.0f);
	sparksSettings.size = ds::vec2(0.6f, 0.1f);
	sparksSettings.sizeVariance = ds::vec2(1.2f, 0.0f);
	sparksSettings.acceleration = ds::vec2(0.0f);
	sparksSettings.growth = ds::vec2(1.0f, 0.0f);

	int explosionID = particles.add(&explosionDescriptor);
	int sparksID = particles.add(&sparksDescriptor);
	
	int emitter = 128;
	float t = 1.1f;

	int explosionState = 1;
	int sparksState = 1;
	bool useExplosion = true;
	bool useSparks = true;

	while (ds::isRunning()) {

		t += static_cast<float>(ds::getElapsedSeconds());
		// every second emit some particles at random position
		if (t >= 1.0f) {
			t -= 1.0f;
			float radius = ds::random(25.0f,100.0f);
			float px = ds::random(200.0f, 800.0f);
			float py = ds::random(200.0, 500.0f);
			if (useExplosion) {
				particles.emitt(explosionID, ds::vec2(px, py), explosionSettings);
			}
			if (useSparks) {
				particles.emitt(sparksID, ds::vec2(px, py), sparksSettings);
			}
		}			
		ds::begin();
		particles.tick(static_cast<float>(ds::getElapsedSeconds()));
		particles.render();

		// GUI
		spriteBuffer.begin();
		gui::start(ds::vec2(0, 750));
		gui::begin("Explosion", &explosionState);
		if (explosionState == 1) {
			gui::Checkbox("Show explosion", &useExplosion);
			gui::Input("Count", &explosionSettings.count);
			gui::Input("angleVariance", &explosionSettings.angleVariance);
			gui::Input("radius", &explosionSettings.radius);
			gui::Input("radiusVariance", &explosionSettings.radiusVariance);
			gui::Input("ttl", &explosionSettings.ttl);
			gui::Input("velocity", &explosionSettings.velocity);
			gui::Input("velocityVariance", &explosionSettings.velocityVariance);
			gui::Input("size", &explosionSettings.size);
			gui::Input("sizeVariance", &explosionSettings.sizeVariance);
			gui::Input("growth", &explosionSettings.growth);
			gui::Value("Alive", particles.numAlive(explosionID));
		}
		gui::end();
		gui::begin("Sparks", &sparksState);
		if (sparksState == 1) {
			gui::Checkbox("Show sparks", &useSparks);
			gui::Input("Count", &sparksSettings.count);
			gui::Input("angleVariance", &sparksSettings.angleVariance);
			gui::Input("radius", &sparksSettings.radius);
			gui::Input("radiusVariance", &sparksSettings.radiusVariance);
			gui::Input("ttl", &sparksSettings.ttl);
			gui::Input("velocity", &sparksSettings.velocity);
			gui::Input("velocityVariance", &sparksSettings.velocityVariance);
			gui::Input("size", &sparksSettings.size);
			gui::Input("sizeVariance", &sparksSettings.sizeVariance);
			gui::Input("growth", &sparksSettings.growth);
			gui::Value("Alive", particles.numAlive(sparksID));
		}
		gui::end();
		spriteBuffer.flush();

		ds::end();
	}
	ds::shutdown();
}