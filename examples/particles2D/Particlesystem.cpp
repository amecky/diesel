#include "Particlesystem.h"

// -------------------------------------------------------
// create new particlesystem
// -------------------------------------------------------
Particlesystem::Particlesystem(ParticlesystemDescriptor descriptor) : _descriptor(descriptor) {
	_array.initialize(descriptor.maxParticles);
	_vertices = new ParticleVertex[descriptor.maxParticles];
	_constantBuffer.screenDimension = v4(1024.0f, 768.0f, 128.0f, 128.0f);
	_constantBuffer.screenCenter = v4(512.0f, 384.0f, 0.0f, 0.0f);
	matrix viewMatrix = mat_identity();
	matrix projectionMatrix = mat_OrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	_viewProjectionMatrix = viewMatrix * projectionMatrix;
}

// -------------------------------------------------------
// add particle based on ParticleDescriptor
// -------------------------------------------------------
void Particlesystem::add(const v2& pos, ParticleDescriptor descriptor) {
	int start = _array.countAlive;
	if ((start + 1) < _array.count) {	
		_array.colors[start] = descriptor.color;
		_array.scales[start] = descriptor.scale;
		_array.grow[start] = descriptor.growth;
		_array.rotations[start] = descriptor.rotation;
		_array.rotationSpeeds[start] = descriptor.rotationSpeed;
		_array.timers[start] = v3(0.0f, descriptor.ttl, 1);
		_array.velocities[start] = v3(descriptor.velocity);
		_array.frictions[start] = descriptor.friction;
		_array.positions[start] = v3(pos);
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
		v3 t = _array.timers[i];
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

	ds::setBlendState(_descriptor.blendState);
	ds::setShader(_descriptor.shader);
	ds::setSamplerState(_descriptor.samplerState);
	ds::setTexture(_descriptor.texture, ds::ShaderType::PIXEL);
	// prepare constant buffers
	matrix w = mat_identity();
	_constantBuffer.wvp = mat_Transpose(_viewProjectionMatrix);
	ds::updateConstantBuffer(_descriptor.constantBuffer, &_constantBuffer, sizeof(ParticleConstantBuffer));
	ds::setConstantBuffer(_descriptor.constantBuffer,ds::ShaderType::GEOMETRY);
	ds::setConstantBuffer(_descriptor.constantBuffer,ds::ShaderType::VERTEX);
	for (int i = 0; i < _array.countAlive; ++i) {		
		v4 t = v4(0.0f,0.0f,1.0f,1.0f);
		_vertices[i] = ParticleVertex(_array.positions[i], _descriptor.particleDimension, v3(_array.scales[i].x, _array.scales[i].y, _array.rotations[i]), _array.colors[i]);
	}
	unsigned int stride = sizeof(ParticleVertex);
	unsigned int offset = 0;
	ds::setVertexBuffer(_descriptor.vertexBuffer, &stride, &offset, ds::PrimitiveTypes::POINT_LIST);
	ds::setVertexDeclaration(_descriptor.vertexDeclaration);
	ds::mapBufferData(_descriptor.vertexBuffer, _vertices, _array.countAlive * sizeof(ParticleVertex));
	ds::draw(_array.countAlive);
}