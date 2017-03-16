#include "Particlesystem.h"

// -------------------------------------------------------
// create new particlesystem
// -------------------------------------------------------
Particlesystem::Particlesystem(ParticlesystemDescriptor descriptor) : _descriptor(descriptor) {
	_array.initialize(descriptor.maxParticles);
	_vertices = new ParticleVertex[descriptor.maxParticles];

	RID blendState = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::ONE, ds::BlendStates::ONE, true);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "GPUParticles_vs.cso" },
		{ ds::ShaderType::PIXEL, "GPUParticles_ps.cso" },
		{ ds::ShaderType::GEOMETRY, "GPUParticles_gs.cso" }
	};
	RID shader = ds::createShader(desc, 3);

	// very special buffer layout 
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	RID vertexDeclaration = ds::createVertexDeclaration(decl, 4, shader);

	RID constantBuffer = ds::createConstantBuffer(sizeof(ParticleConstantBuffer));
	_vertexBuffer = ds::createVertexBuffer(ds::BufferType::DYNAMIC, descriptor.maxParticles, sizeof(ParticleVertex));
	RID samplerState = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	ds::StateGroup* basicGroup = ds::createStateGroup();
	basicGroup->bindLayout(vertexDeclaration);	
	basicGroup->bindConstantBuffer(constantBuffer, ds::ShaderType::VERTEX, 0, &_constantBuffer);
	basicGroup->bindConstantBuffer(constantBuffer, ds::ShaderType::GEOMETRY, 0, &_constantBuffer);
	basicGroup->bindBlendState(blendState);
	basicGroup->bindShader(shader);
	//basicGroup->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	basicGroup->bindTexture(descriptor.texture, ds::ShaderType::PIXEL, 0);

	basicGroup->bindVertexBuffer(_vertexBuffer);

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST, 0 };

	_drawItem = ds::compile(drawCmd, basicGroup);
}

// -------------------------------------------------------
// add particle based on ParticleDescriptor
// -------------------------------------------------------
void Particlesystem::add(const ds::vec3& pos, ParticleDescriptor descriptor) {
	int start = _array.countAlive;
	if ((start + 1) < _array.count) {	
		_array.timers[start] = ds::vec3(0.0f,0.0f,descriptor.ttl);
		_array.velocities[start] = ds::vec3(descriptor.velocity);
		_array.positions[start] = pos;
		_array.sizes[start] = ds::vec4(descriptor.minScale.x, descriptor.minScale.y, descriptor.maxScale.x, descriptor.maxScale.y);
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

	// prepare constant buffers
	ds::matrix w = ds::matIdentity();
	_constantBuffer.wvp = ds::matTranspose(ds::getViewProjectionMatrix());
	_constantBuffer.startColor = _descriptor.startColor;
	_constantBuffer.endColor = _descriptor.endColor;
	_constantBuffer.eyePos = ds::getViewPosition();
	_constantBuffer.padding = 0.0f;
	_constantBuffer.world = ds::matTranspose(w);
	for (int i = 0; i < _array.countAlive; ++i) {
		_vertices[i] = ParticleVertex(_array.positions[i], _array.velocities[i], ds::vec2(_array.timers[i].x, _array.timers[i].y), _array.sizes[i]);
	}
	ds::mapBufferData(_vertexBuffer, _vertices, _array.countAlive * sizeof(ParticleVertex));

	_drawItem->command.size = _array.countAlive;
	ds::submit(_drawItem);

}