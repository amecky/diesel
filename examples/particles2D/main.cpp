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
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);
	return textureID;
}

struct ExplosionSettings {
	int count;
	ds::vec2 ttl;
	float angleVariance;
	float radiusVariance;
	ds::vec2 velocityVariance;
	ds::vec2 sizeVariance;
};

void emittExplosion(Particlesystem* system, ParticleDescriptor& particleDescriptor, const ExplosionSettings& settings, float px, float py, float radius) {
	for (int i = 0; i < settings.count; ++i) {
		float angle = ds::TWO_PI * static_cast<float>(i) / static_cast<float>(settings.count);
		float x = px + cos(angle) * (radius + ds::random(-settings.radiusVariance, settings.radiusVariance));
		float y = py + sin(angle) * (radius + ds::random(-settings.radiusVariance, settings.radiusVariance));
		float da = angle * settings.angleVariance;
		angle += ds::random(-da, da);
		ds::vec2 s = ds::vec2(0.2f, 0.15f);
		float ds = ds::random(settings.sizeVariance.x,settings.sizeVariance.y);
		s *= ds;
		particleDescriptor.scale = s;
		particleDescriptor.ttl = ds::random(settings.ttl.x, settings.ttl.y);
		particleDescriptor.rotation = angle;
		particleDescriptor.rotationSpeed = 0.0f;
		particleDescriptor.velocity = ds::random(settings.velocityVariance.x, settings.velocityVariance.y) * ds::vec2(cos(angle), sin(angle));
		system->add(ds::vec2(x, y), particleDescriptor);
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

void emittSparks(Particlesystem* system, ParticleDescriptor& particleDescriptor, const SparksSettings& settings, float px, float py, float radius) {
	for (int i = 0; i < settings.count; ++i) {
		float angle = ds::TWO_PI * static_cast<float>(i) / static_cast<float>(settings.count);
		float x = px + cos(angle) * (radius + ds::random(-settings.radiusVariance, settings.radiusVariance));
		float y = py + sin(angle) * (radius + ds::random(-settings.radiusVariance, settings.radiusVariance));
		float da = angle * settings.angleVariance;
		angle += ds::random(-da, da);
		particleDescriptor.ttl = ds::random(settings.ttl.x, settings.ttl.y);
		particleDescriptor.rotation = angle;
		particleDescriptor.rotationSpeed = 0.0f;
		particleDescriptor.growth = settings.growth;
		particleDescriptor.velocity = ds::random(settings.velocityVariance.x, settings.velocityVariance.y) * ds::vec2(cos(angle), sin(angle));
		system->add(ds::vec2(x, y), particleDescriptor);
	}
}
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

	ParticlesystemDescriptor descriptor;
	descriptor.maxParticles = 1024;
	descriptor.particleDimension = ds::vec2(64, 64);
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("particles.png", &x, &y, &n, 4);
	descriptor.textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);
	
	// prepare IMGUI

	RID guiTextureID = loadImage("..\\common\\imgui.png");
	SpriteBuffer spriteBuffer(512, guiTextureID);
	gui::init(&spriteBuffer, guiTextureID);

	Particlesystem system(descriptor);
	Particlesystem sparks(descriptor);

	ParticleManager particles(2048, descriptor.textureID);
	particles.add(&system);
	particles.add(&sparks);

	ParticleDescriptor explosionDescriptor;
	explosionDescriptor.ttl = 2.0f;
	explosionDescriptor.color = ds::Color(255,250, 179, 255);
	explosionDescriptor.velocity = ds::vec2(20.0f, 0.0f);
	explosionDescriptor.friction = 0.1f;
	explosionDescriptor.scale = ds::vec2(0.2f,0.2f);
	explosionDescriptor.growth = ds::vec2(0.0f, 0.0f);
	explosionDescriptor.alphaFading = true;

	ParticleDescriptor sparksDescriptor;
	sparksDescriptor.ttl = 2.0f;
	sparksDescriptor.color = ds::Color(248, 213, 131, 255);
	sparksDescriptor.velocity = ds::vec2(20.0f, 0.0f);
	sparksDescriptor.friction = 0.0f;
	sparksDescriptor.scale = ds::vec2(0.2f, 0.1f);
	sparksDescriptor.growth = ds::vec2(0.8f, 0.0f);
	sparksDescriptor.alphaFading = true;

	ExplosionSettings explosionSettings;
	explosionSettings.count = 128;
	explosionSettings.angleVariance = 0.1f;
	explosionSettings.radiusVariance = 10.0f;
	explosionSettings.ttl = ds::vec2(0.6f, 0.9f);
	explosionSettings.velocityVariance = ds::vec2(40.0f, 120.0f);
	explosionSettings.sizeVariance = ds::vec2(0.5f, 2.0f);

	SparksSettings sparksSettings;
	sparksSettings.count = 128;
	sparksSettings.angleVariance = 0.2f;
	sparksSettings.radiusVariance = 10.0f;
	sparksSettings.ttl = ds::vec2(0.6f, 0.9f);
	sparksSettings.velocityVariance = ds::vec2(120.0f, 200.0f);
	sparksSettings.growth = ds::vec2(0.8f, 0.0f);
	
	int emitter = 128;
	float t = 1.1f;

	int explosionState = 1;
	int sparksState = 1;
	bool useExplosion = true;
	bool useSparks = true;

	while (ds::isRunning()) {

		t += static_cast<float>(ds::getElapsedSeconds());
		// every second emitt some particles at random position
		if (t >= 1.0f) {
			t -= 1.0f;
			float radius = ds::random(25.0f,100.0f);
			float px = ds::random(200.0f, 800.0f);
			float py = ds::random(200.0, 500.0f);
			if (useExplosion) {
				emittExplosion(&system, explosionDescriptor, explosionSettings, px, py, radius);
			}
			if (useSparks) {
				emittSparks(&sparks, sparksDescriptor, sparksSettings, px, py, radius);
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
			gui::Input("radiusVariance", &explosionSettings.radiusVariance);
			gui::Input("ttl", &explosionSettings.ttl);
			gui::Input("velocityVariance", &explosionSettings.velocityVariance);
			gui::Input("sizeVariance", &explosionSettings.sizeVariance);
		}
		gui::end();
		gui::begin("Sparks", &sparksState);
		if (sparksState == 1) {
			gui::Checkbox("Show sparks", &useSparks);
			gui::Input("Count",&sparksSettings.count);
			gui::Input("angleVariance", &sparksSettings.angleVariance);
			gui::Input("radiusVariance", &sparksSettings.radiusVariance);
			gui::Input("ttl", &sparksSettings.ttl);
			gui::Input("velocityVariance", &sparksSettings.velocityVariance);
			gui::Input("growth", &sparksSettings.growth);
		}
		gui::end();
		spriteBuffer.flush();

		ds::end();
	}
	ds::shutdown();
}