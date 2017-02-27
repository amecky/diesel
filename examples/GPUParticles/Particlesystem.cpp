#include "Particlesystem.h"

// -------------------------------------------------------
// create new particlesystem
// -------------------------------------------------------
Particlesystem::Particlesystem(ParticlesystemDescriptor descriptor) : _descriptor(descriptor) {
	_array.initialize(descriptor.maxParticles);
	_vertices = new ParticleVertex[descriptor.maxParticles];
}

// -------------------------------------------------------
// add particle based on ParticleDescriptor
// -------------------------------------------------------
void Particlesystem::add(const v3& pos, ParticleDescriptor descriptor) {
	int start = _array.countAlive;
	if ((start + 1) < _array.count) {	
		_array.timers[start] = v3(0.0f,0.0f,descriptor.ttl);
		_array.velocities[start] = v3(descriptor.velocity);
		_array.positions[start] = pos;
		_array.sizes[start] = v4(descriptor.minScale.x, descriptor.minScale.y, descriptor.maxScale.x, descriptor.maxScale.y);
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
		t.y = t.x / t.z;
		_array.timers[i] = t;
		if (t.y >= 1.0f) {
			_array.kill(i);
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
	_constantBuffer.wvp = mat_Transpose(ds::getViewProjectionMatrix());
	_constantBuffer.startColor = _descriptor.startColor;
	_constantBuffer.endColor = _descriptor.endColor;
	_constantBuffer.eyePos = ds::getViewPosition();
	_constantBuffer.padding = 0.0f;
	_constantBuffer.world = mat_Transpose(w);
	ds::updateConstantBuffer(_descriptor.constantBuffer, &_constantBuffer, sizeof(ParticleConstantBuffer));
	ds::setConstantBuffer(_descriptor.constantBuffer,ds::ShaderType::GEOMETRY);
	ds::setConstantBuffer(_descriptor.constantBuffer,ds::ShaderType::VERTEX);

	for (int i = 0; i < _array.countAlive; ++i) {		
		_vertices[i] = ParticleVertex(_array.positions[i], _array.velocities[i], v2(_array.timers[i].x,_array.timers[i].y),_array.sizes[i]);
	}

	unsigned int stride = sizeof(ParticleVertex);
	unsigned int offset = 0;
	ds::setVertexBuffer(_descriptor.vertexBuffer, &stride, &offset, ds::PrimitiveTypes::POINT_LIST);
	ds::setVertexDeclaration(_descriptor.vertexDeclaration);
	ds::mapBufferData(_descriptor.vertexBuffer, _vertices, _array.countAlive * sizeof(ParticleVertex));
	ds::draw(_array.countAlive);
}