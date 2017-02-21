//#include "..\..\diesel.h"
#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
	ds::init(rs);

	ParticlesystemDescriptor descriptor;
	descriptor.maxParticles = 1024;
	descriptor.particleDimension = v2(32, 32);
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("particles.png", &x, &y, &n, 4);
	descriptor.texture = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);

	
	descriptor.blendState = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	// create shader and load compiled shaders
	descriptor.shader = ds::createShader();
	ds::loadVertexShader(descriptor.shader, "Particles_2D_vs.cso");
	ds::loadPixelShader(descriptor.shader, "Particles_2D_ps.cso");
	ds::loadGeometryShader(descriptor.shader, "Particles_2D_gs.cso");

	// very special buffer layout 
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	descriptor.vertexDeclaration = ds::createVertexDeclaration(decl, 4, descriptor.shader);
		
	descriptor.constantBuffer = ds::createConstantBuffer(sizeof(ParticleConstantBuffer));
	descriptor.vertexBuffer = ds::createVertexBuffer(ds::BufferType::DYNAMIC, descriptor.maxParticles, descriptor.vertexDeclaration);
	descriptor.samplerState = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	Particlesystem system(descriptor);

	ParticleDescriptor particleDescriptor;
	particleDescriptor.ttl = 2.0f;
	particleDescriptor.color = ds::Color(244,165,41,255);
	particleDescriptor.velocity = v2(20.0f, 0.0f);
	particleDescriptor.friction = 0.5f;
	particleDescriptor.scale = v2(1, 1);
	particleDescriptor.growth = v2(-0.3f, -0.1f);
	particleDescriptor.alphaFading = true;
	
	int emitter = 64;
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
				particleDescriptor.rotationSpeed = ds::random(0.5f,2.0f) * ds::PI * 2.0f;
				particleDescriptor.scale = v2(1.0f, 0.4f);
				particleDescriptor.velocity = ds::random(80.0f,300.0f) * v2(cos(angle), sin(angle));				
				system.add(v2(x, y), particleDescriptor);
			}
		}
			
		ds::begin();
		// disbale depth buffer
		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
		system.tick(static_cast<float>(ds::getElapsedSeconds()));
		system.render();
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		ds::end();
	}
	ds::shutdown();
}