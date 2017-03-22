#include "Particlesystem.h"

// ------------------------------------------------------ -
// create new particlesystem
// -------------------------------------------------------
ParticleManager::ParticleManager(int maxParticles, RID textureID) {
	_vertices = new ParticleVertex[maxParticles];
	_constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, 128.0f, 128.0f);
	_constantBuffer.screenCenter = ds::vec4(512.0f, 384.0f, 0.0f, 0.0f);
	ds::matrix viewMatrix = ds::matIdentity();
	ds::matrix projectionMatrix = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	_viewprojectionMatrix = viewMatrix * projectionMatrix;

	RID vertexShader = ds::loadVertexShader("Particles_2D_vs.cso");
	RID pixelShader = ds::loadPixelShader("Particles_2D_Tex_ps.cso");
	RID geoShader = ds::loadGeometryShader("Particles_2D_gs.cso");

	// very special buffer layout 
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::ONE, ds::BlendStates::ONE, ds::BlendStates::ONE, true);

	RID vertexDeclaration = ds::createVertexDeclaration(decl, 4, vertexShader);

	RID constantBuffer = ds::createConstantBuffer(sizeof(ParticleConstantBuffer), &_constantBuffer);
	_vertexBuffer = ds::createVertexBuffer(ds::BufferType::DYNAMIC, maxParticles, sizeof(ParticleVertex));

	RID basicGroup = ds::StateGroupBuilder()
		.inputLayout(vertexDeclaration)
		.vertexShader(vertexShader)
		//.blendState(bs_id)
		.geometryShader(geoShader)
		.pixelShader(pixelShader)
		.constantBuffer(constantBuffer, geoShader, 0)
		.texture(textureID, pixelShader, 0)
		.vertexBuffer(_vertexBuffer)
		.build();

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST };

	_drawItem = ds::compile(drawCmd, basicGroup);

	// create orthographic view
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(ds::getScreenWidth(), ds::getScreenHeight(), 0.1f, 1.0f);
	_orthoPass = ds::createRenderPass(orthoView, orthoProjection, ds::DepthBufferState::DISABLED);
	//constantBuffer.wvp = ds::matTranspose(orthoView * orthoProjection);

}

void ParticleManager::add(Particlesystem* system) {
	_systems.push_back(system);
}

void ParticleManager::tick(float dt) {
	for (size_t p = 0; p < _systems.size(); ++p) {
		_systems[p]->tick(dt);
	}
}

void ParticleManager::render() {
	//ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
	ds::matrix w = ds::matIdentity();
	_constantBuffer.wvp = ds::matTranspose(_viewprojectionMatrix);
	for (size_t p = 0; p < _systems.size(); ++p) {
		const ParticleArray* array = _systems[p]->getArray();
		const ParticlesystemDescriptor& desc = _systems[p]->getDescriptor();
		for (int i = 0; i < array->countAlive; ++i) {
			ds::vec4 t = ds::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			_vertices[i] = ParticleVertex(array->positions[i], desc.particleDimension, ds::vec3(array->scales[i].x, array->scales[i].y, array->rotations[i]), array->colors[i]);
		}
		ds::mapBufferData(_vertexBuffer, _vertices, array->countAlive * sizeof(ParticleVertex));
		ds::submit(_orthoPass, _drawItem, array->countAlive);
	}
	//ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
}

// -------------------------------------------------------
// create new particlesystem
// -------------------------------------------------------
Particlesystem::Particlesystem(ParticlesystemDescriptor descriptor) : _descriptor(descriptor) {
	_array.initialize(descriptor.maxParticles);
}

// -------------------------------------------------------
// add particle based on ParticleDescriptor
// -------------------------------------------------------
void Particlesystem::add(const ds::vec2& pos, ParticleDescriptor descriptor) {
	int start = _array.countAlive;
	if ((start + 1) < _array.count) {	
		_array.colors[start] = descriptor.color;
		_array.scales[start] = descriptor.scale;
		_array.grow[start] = descriptor.growth;
		_array.rotations[start] = descriptor.rotation;
		_array.rotationSpeeds[start] = descriptor.rotationSpeed;
		_array.timers[start] = ds::vec3(0.0f, descriptor.ttl, 1);
		_array.velocities[start] = ds::vec3(descriptor.velocity);
		_array.frictions[start] = descriptor.friction;
		_array.positions[start] = ds::vec3(pos);
		if (descriptor.alphaFading) {
			_array.decays[start] = 1.0f;
		}
		else {
			_array.decays[start] = 0.0f;
		}
		_array.wake(start);
	}
}

// -------------------------------------------------------
// tick - updates all particles and manages lifetime
// -------------------------------------------------------
void Particlesystem::tick(float dt) {
	for (int i = 0; i < _array.countAlive; ++i) {
		ds::vec3 t = _array.timers[i];
		t.x += dt;
		t.z = t.x / t.y;
		_array.timers[i] = t;
		if (t.z >= 1.0f) {
			_array.kill(i);
		}
	}
	for (int i = 0; i < _array.countAlive; ++i) {
		// move
		if (_array.frictions[i] > 0.0f) {
			_array.velocities[i] *= (1.0f - _array.frictions[i] * dt);
		}
		_array.positions[i] += _array.velocities[i] * dt;
		// scale
		_array.scales[i] += _array.grow[i] * dt;
		// rotate
		if (_array.rotationSpeeds[i] > 0.0f) {
			_array.rotations[i] += _array.rotationSpeeds[i] * dt;
		}
		// fade color
		if (_array.decays[i] > 0.0f) {
			_array.colors[i].a = 1.0f - _array.timers[i].z;
		}
	}
}
