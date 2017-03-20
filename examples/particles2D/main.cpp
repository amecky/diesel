//#include "..\..\diesel.h"
#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#define DS_IMPLEMENTATION
#include "Particlesystem.h"

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
	descriptor.particleDimension = ds::vec2(64, 64);
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("particles.png", &x, &y, &n, 4);
	descriptor.textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);
	
	Particlesystem system(descriptor);

	ParticleDescriptor particleDescriptor;
	particleDescriptor.ttl = 2.0f;
	particleDescriptor.color = ds::Color(244,165,41,255);
	particleDescriptor.velocity = ds::vec2(20.0f, 0.0f);
	particleDescriptor.friction = 0.5f;
	particleDescriptor.scale = ds::vec2(0.4f,0.2f);
	particleDescriptor.growth = ds::vec2(1.2f, 0.0f);
	particleDescriptor.alphaFading = true;
	
	int emitter = 128;
	float radius = 10.0f;
	float t = 1.1f;

	while (ds::isRunning()) {

		t += static_cast<float>(ds::getElapsedSeconds());
		// every second emitt some particles at random position
		if (t >= 1.0f) {
			t -= 1.0f;
			float px = ds::random(200.0f, 800.0f);
			float py = ds::random(200.0, 500.0f);
			for (int i = 0; i < emitter; ++i) {
				float angle = ds::TWO_PI * static_cast<float>(i) / static_cast<float>(emitter);
				float x = px + cos(angle) * radius;
				float y = py + sin(angle) * radius;
				particleDescriptor.ttl = ds::random(1.0f,2.0f);
				particleDescriptor.rotation = angle;
				particleDescriptor.rotationSpeed = 0.0f;// ds::random(0.5f, 2.0f) * ds::PI * 2.0f;
				//particleDescriptor.scale = ds::vec2(1.0f, 0.4f);
				particleDescriptor.velocity = ds::random(80.0f,300.0f) * ds::vec2(cos(angle), sin(angle));				
				system.add(ds::vec2(x, y), particleDescriptor);
			}
		}
			
		ds::begin();
		system.tick(static_cast<float>(ds::getElapsedSeconds()));
		system.render();
		ds::end();
	}
	ds::shutdown();
}