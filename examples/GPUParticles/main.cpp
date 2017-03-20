//#include "..\..\diesel.h"
#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#define DS_IMPLEMENTATION
#include "Particlesystem.h"
#include "..\common\Grid.h"

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
	
	
	descriptor.startColor = ds::Color(192, 192, 0, 255);
	descriptor.endColor = ds::Color(192, 16, 0, 255);

	Particlesystem system(descriptor);

	ParticleDescriptor particleDescriptor;
	particleDescriptor.ttl = 0.4f;
	particleDescriptor.velocity = ds::vec2(20.0f, 0.0f);
	particleDescriptor.friction = 0.5f;
	particleDescriptor.maxScale = ds::vec2(0.02f,0.02f);
	particleDescriptor.minScale = ds::vec2(0.05f, 0.05f);
	
	int emitter = 64;
	float radius = 0.05f;
	float t = 0.0f;

	RID textureID = loadImage("..\\common\\cube_map.png");

	RID vertexShader = ds::loadVertexShader("..\\common\\Textured_vs.cso");
	RID pixelShader = ds::loadPixelShader("..\\common\\Textured_ps.cso");

	Grid grid;
	ds::vec3 gridPositions[] = {
		ds::vec3(-4.0f, -1.0f, -3.5f),
		ds::vec3(-4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f, -3.5f)
	};
	grid.create(gridPositions, 2, vertexShader, pixelShader, textureID);

	ds::setViewPosition(ds::vec3(0, 0, -4));

	float dt = 1.0f / 60.0f;
	float frequency = 200.0f * dt;
	float accu = 0.0f;

	while (ds::isRunning()) {

		t += static_cast<float>(ds::getElapsedSeconds());
		if (t >= dt) {
			accu += frequency;
			while (accu >= 1.0f) {
				accu -= 1.0f;
				float angle = ds::random(0.0f, ds::TWO_PI);
				float x = cos(angle) * radius;
				float y = -1.0f;
				float z = sin(angle) * radius;
				particleDescriptor.ttl = ds::random(0.6f, 1.2f);
				particleDescriptor.velocity = ds::vec3(ds::random(-0.4f,0.4f), ds::random(1.2f, 2.6f), ds::random(-0.4f, 0.4f));
				system.add(ds::vec3(x, y, z), particleDescriptor);
			}
			t -= dt;
		}
		ds::begin();
		grid.render();
		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
		system.tick(static_cast<float>(ds::getElapsedSeconds()));
		system.render();
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		ds::end();
	}
	ds::shutdown();
}