#include "Particlesystem.h"
#include "GPUParticles_PS_Main.h"
#include "GPUParticles_VS_Main.h"
// -------------------------------------------------------
// create new particlesystem
// -------------------------------------------------------
Particlesystem::Particlesystem(ds::Camera* camera, ParticlesystemDescriptor descriptor, RID renderPass) : _camera(camera), _descriptor(descriptor) , _renderPass(renderPass) {
	_array.initialize(descriptor.maxParticles);
	_vertices = new GPUParticle[descriptor.maxParticles];

	ds::BlendStateInfo blendInfo = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
	RID blendState = ds::createBlendState(blendInfo);

	ds::ShaderInfo vsInfo = { 0, GPUParticles_VS_Main, sizeof(GPUParticles_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID vertexShader = ds::createShader(vsInfo, "ParticlesVS");
	ds::ShaderInfo psInfo = { 0, GPUParticles_PS_Main, sizeof(GPUParticles_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID pixelShader = ds::createShader(psInfo, "ParticlesPS");


	RID constantBuffer = ds::createConstantBuffer(sizeof(ParticleConstantBuffer), &_constantBuffer);
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID samplerState = ds::createSamplerState(samplerInfo);

	//int indices[] = { 0,1,2,1,3,2 };
	int indices[] = { 0,1,3,1,2,3 };
	RID idxBuffer = ds::createQuadIndexBuffer(descriptor.maxParticles, indices);

	ds::StructuredBufferInfo sbInfo;
	sbInfo.cpuWritable = true;
	sbInfo.data = 0;
	sbInfo.elementSize = sizeof(GPUParticle);
	sbInfo.numElements = descriptor.maxParticles;
	sbInfo.gpuWritable = false;
	sbInfo.renderTarget = NO_RID;
	sbInfo.textureID = NO_RID;
	_structuredBufferId = ds::createStructuredBuffer(sbInfo);

	RID basicGroup = ds::StateGroupBuilder()
		.constantBuffer(constantBuffer, vertexShader)
		.structuredBuffer(_structuredBufferId, vertexShader, 1)
		.vertexBuffer(NO_RID)
		.vertexShader(vertexShader)
		.indexBuffer(idxBuffer)
		.pixelShader(pixelShader)
		.samplerState(samplerState, pixelShader)
		.texture(descriptor.texture, pixelShader, 0)
		.build();

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST, 0 };

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
	_constantBuffer.wvp = ds::matTranspose(_camera->viewProjectionMatrix);
	_constantBuffer.startColor = _descriptor.startColor;
	_constantBuffer.endColor = _descriptor.endColor;
	_constantBuffer.eyePos = _camera->position;
	_constantBuffer.padding = 0.0f;
	_constantBuffer.world = ds::matTranspose(w);
	/*
	ds::vec3 position;
	ds::vec3 velocity;
	ds::vec3 acceleration;
	ds::vec2 timer;
	ds::vec3 scale;
	ds::vec3 growth;
	*/
	for (int i = 0; i < _array.countAlive; ++i) {
		_vertices[i] = { 
			_array.positions[i], 
			_array.velocities[i], 
			ds::vec3(0.0f),
			ds::vec2(_array.timers[i].x, _array.timers[i].y), 
			ds::vec3(_array.sizes[i].xy(),1.0),
			ds::vec3(0.0f)
		};
	}
	ds::mapBufferData(_structuredBufferId, _vertices, _array.countAlive * sizeof(GPUParticle));
	ds::submit(_renderPass, _drawItem, _array.countAlive * 6);

}