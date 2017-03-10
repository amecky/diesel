#include "Particlesystem.h"

// -------------------------------------------------------
// create new particlesystem
// -------------------------------------------------------
Particlesystem::Particlesystem(ParticlesystemDescriptor descriptor) : _descriptor(descriptor) {
	_array.initialize(descriptor.maxParticles);
	_vertices = new ParticleVertex[descriptor.maxParticles];
	_constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, 128.0f, 128.0f);
	_constantBuffer.screenCenter = ds::vec4(512.0f, 384.0f, 0.0f, 0.0f);
	ds::matrix viewMatrix = ds::matIdentity();
	ds::matrix projectionMatrix = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	_viewprojectionMatrix = viewMatrix * projectionMatrix;
	

	RID blendState = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "Particles_2D_vs.cso" },
		{ ds::ShaderType::PIXEL, "Particles_2D_ps.cso" },
		{ ds::ShaderType::GEOMETRY, "Particles_2D_gs.cso" }
	};

	RID gridShader = ds::createShader(desc, 3);

	// very special buffer layout 
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	RID vertexDeclaration = ds::createVertexDeclaration(decl, 4, gridShader);

	RID constantBuffer = ds::createConstantBuffer(sizeof(ParticleConstantBuffer));
	_vertexBuffer = ds::createVertexBuffer(ds::BufferType::DYNAMIC, descriptor.maxParticles, vertexDeclaration);
	RID samplerState = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	ds::StateGroup* basicGroup = ds::createStateGroup();
	basicGroup->bindLayout(vertexDeclaration);
	basicGroup->bindShader(gridShader);
	basicGroup->bindConstantBuffer(constantBuffer, ds::ShaderType::VERTEX, &_constantBuffer);
	basicGroup->bindConstantBuffer(constantBuffer, ds::ShaderType::GEOMETRY, &_constantBuffer);
	basicGroup->bindBlendState(blendState);
	//basicGroup->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	basicGroup->bindTexture(descriptor.textureID, ds::ShaderType::PIXEL, 0);
	
	basicGroup->bindVertexBuffer(_vertexBuffer);

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST };
	
	_drawItem = ds::compile(drawCmd, basicGroup);

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

// -------------------------------------------------------
// render particles
// -------------------------------------------------------
void Particlesystem::render() {
	ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
	ds::matrix w = ds::matIdentity();
	_constantBuffer.wvp = ds::matTranspose(_viewprojectionMatrix);
	for (int i = 0; i < _array.countAlive; ++i) {
		ds::vec4 t = ds::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		_vertices[i] = ParticleVertex(_array.positions[i], _descriptor.particleDimension, ds::vec3(_array.scales[i].x, _array.scales[i].y, _array.rotations[i]), _array.colors[i]);
	}
	ds::mapBufferData(_vertexBuffer, _vertices, _array.countAlive * sizeof(ParticleVertex));

	_drawItem->command.size = _array.countAlive;
	ds::submit(_drawItem);
	ds::setDepthBufferState(ds::DepthBufferState::ENABLED);

}