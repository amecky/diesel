//#include "..\..\diesel.h"
#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#define DS_IMPLEMENTATION
#include "Particlesystem.h"

void emittExplosion(Particlesystem* system, ParticleDescriptor& particleDescriptor, float px, float py, float radius) {
	int emitter = 128;
	for (int i = 0; i < emitter; ++i) {
		float angle = ds::TWO_PI * static_cast<float>(i) / static_cast<float>(emitter);		
		float x = px + cos(angle) * (radius + ds::random(-10.0f, 10.0f));
		float y = py + sin(angle) * (radius + ds::random(-10.0f, 10.0f));
		float da = angle * 0.1f;
		angle += ds::random(-da, da);
		ds::vec2 s = ds::vec2(0.2f, 0.15f);
		float ds = ds::random(0.5f,1.5f);
		s *= ds;
		particleDescriptor.scale = s;
		particleDescriptor.ttl = ds::random(0.6f, 0.9f);
		particleDescriptor.rotation = angle;
		particleDescriptor.rotationSpeed = 0.0f;
		particleDescriptor.velocity = ds::random(40.0f, 120.0f) * ds::vec2(cos(angle), sin(angle));
		system->add(ds::vec2(x, y), particleDescriptor);
	}
}

void emittSparks(Particlesystem* system, ParticleDescriptor& particleDescriptor, float px, float py, float radius) {
	int emitter = 128;
	for (int i = 0; i < emitter; ++i) {
		float angle = ds::TWO_PI * static_cast<float>(i) / static_cast<float>(emitter);		
		float x = px + cos(angle) * (radius + ds::random(-10.0f, 10.0f));
		float y = py + sin(angle) * (radius + ds::random(-10.0f, 10.0f));
		float da = angle * 0.2f;
		angle += ds::random(-da, da);
		particleDescriptor.ttl = ds::random(0.6f, 0.9f);
		particleDescriptor.rotation = angle;
		particleDescriptor.rotationSpeed = 0.0f;
		particleDescriptor.velocity = ds::random(120.0f, 200.0f) * ds::vec2(cos(angle), sin(angle));
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
	
	Particlesystem system(descriptor);
	Particlesystem sparks(descriptor);

	ParticleManager particles(2048, descriptor.textureID);
	particles.add(&system);
	particles.add(&sparks);

	ParticleDescriptor explosionDescriptor;
	explosionDescriptor.ttl = 2.0f;
	explosionDescriptor.color = ds::Color(255,250, 179, 255);
	explosionDescriptor.velocity = ds::vec2(20.0f, 0.0f);
	explosionDescriptor.friction = 0.9f;
	explosionDescriptor.scale = ds::vec2(0.2f,0.2f);
	explosionDescriptor.growth = ds::vec2(0.0f, 0.0f);
	explosionDescriptor.alphaFading = true;

	ParticleDescriptor sparksDescriptor;
	sparksDescriptor.ttl = 2.0f;
	sparksDescriptor.color = ds::Color(248, 213, 131, 255);
	sparksDescriptor.velocity = ds::vec2(20.0f, 0.0f);
	sparksDescriptor.friction = 0.9f;
	sparksDescriptor.scale = ds::vec2(0.2f, 0.1f);
	sparksDescriptor.growth = ds::vec2(0.8f, 0.0f);
	sparksDescriptor.alphaFading = true;
	
	int emitter = 128;
	float t = 1.1f;

	while (ds::isRunning()) {

		t += static_cast<float>(ds::getElapsedSeconds());
		// every second emitt some particles at random position
		if (t >= 1.0f) {
			t -= 1.0f;
			float radius = ds::random(25.0f,100.0f);
			float px = ds::random(200.0f, 800.0f);
			float py = ds::random(200.0, 500.0f);
			emittExplosion(&system, explosionDescriptor, px, py, radius);
			emittSparks(&sparks, sparksDescriptor, px , py, radius);
		}			
		ds::begin();
		particles.tick(static_cast<float>(ds::getElapsedSeconds()));
		particles.render();
		ds::end();
	}
	ds::shutdown();
}