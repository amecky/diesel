#include "Particlesystem.h"
#include "GPUParticles_PS_Main.h"
#include "GPUParticles_VS_Main.h"
// -------------------------------------------------------
// create new particlesystem
// -------------------------------------------------------
Particlesystem::Particlesystem(ds::Camera* camera, ParticlesystemDescriptor descriptor, RID renderPass) : _camera(camera), _descriptor(descriptor) , _renderPass(renderPass) {
	_array.initialize(descriptor.maxParticles);
	_vertices = new GPUParticle[descriptor.maxParticles];

	//ds::BlendStateInfo blendStateInfo{ ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
	//ds::BlendStateInfo blendStateInfo = { ds::BlendStates::ONE, ds::BlendStates::ZERO, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::ZERO, true };
	ds::BlendStateInfo blendStateInfo = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::ZERO, ds::BlendStates::ONE, ds::BlendStates::ZERO, true };
	RID blendState = ds::createBlendState(ds::BlendStateDesc()
		.SrcBlend(ds::BlendStates::SRC_ALPHA)
		.SrcAlphaBlend(ds::BlendStates::ZERO)
		.DestBlend(ds::BlendStates::ONE)
		.DestAlphaBlend(ds::BlendStates::ZERO)
		.AlphaEnabled(true)
	);

	RID vertexShader = ds::createShader(ds::ShaderDesc()
		.Data(GPUParticles_VS_Main)
		.DataSize(sizeof(GPUParticles_VS_Main))
		.ShaderType(ds::ShaderType::ST_VERTEX_SHADER)
		, "ParticlesVS"
	); 
	
	RID pixelShader = ds::createShader(ds::ShaderDesc()
		.Data(GPUParticles_PS_Main)
		.DataSize(sizeof(GPUParticles_PS_Main))
		.ShaderType(ds::ShaderType::ST_PIXEL_SHADER)
		, "ParticlesPS"
	);


	RID constantBuffer = ds::createConstantBuffer(sizeof(ParticleConstantBuffer), &_constantBuffer);
	RID samplerState = ds::createSamplerState(ds::SamplerStateDesc()
		.AddressMode(ds::TextureAddressModes::CLAMP)
		.Filter(ds::TextureFilters::LINEAR)
	);

	int indices[] = { 0,1,2,1,3,2 };
	RID idxBuffer = ds::createQuadIndexBuffer(descriptor.maxParticles, indices);

	_structuredBufferId = ds::createStructuredBuffer(ds::StructuredBufferDesc()
		.CpuWritable(true)
		.ElementSize(sizeof(GPUParticle))
		.NumElements(descriptor.maxParticles)
		.GpuWritable(false)
	);

	RID basicGroup = ds::StateGroupBuilder()
		.blendState(blendState)
		.constantBuffer(constantBuffer, vertexShader)
		.structuredBuffer(_structuredBufferId, vertexShader, 1)
		.vertexBuffer(NO_RID)
		.vertexShader(vertexShader)
		.indexBuffer(idxBuffer)
		.pixelShader(pixelShader)
		.samplerState(samplerState, pixelShader)
		.texture(descriptor.texture, pixelShader, 0)
		.build();

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST, 0 };

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
		_array.accelerations[start] = descriptor.acceleration;
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

void swap(GPUParticle* a, GPUParticle* b) {
	GPUParticle t = *a;
	*a = *b;
	*b = t;
}
/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition(GPUParticle* arr, int low, int high) {
	float pivot = arr[high].position.z;    // pivot
	int i = (low - 1);  // Index of smaller element
	for (int j = low; j <= high - 1; j++) {
		// If current element is smaller than or
		// equal to pivot
		if (arr[j].position.z <= pivot)
		{
			i++;    // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low  --> Starting index,
high  --> Ending index */
void quickSort(GPUParticle* arr, int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(arr, low, high);

		// Separately sort elements before
		// partition and after partition
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
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
	_constantBuffer.textureRect = ds::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	for (int i = 0; i < _array.countAlive; ++i) {
		_vertices[i] = {
			_array.positions[i],
			_array.velocities[i],
			_array.accelerations[i],
			ds::vec2(_array.timers[i].x, _array.timers[i].y),
			ds::vec2(_array.sizes[i].x,_array.sizes[i].y),
			ds::vec2(_array.sizes[i].z,_array.sizes[i].w)
		};
	}
	//quickSort(_vertices, 0, _array.countAlive);
	ds::mapBufferData(_structuredBufferId, _vertices, _array.countAlive * sizeof(GPUParticle));
	ds::submit(_renderPass, _drawItem, _array.countAlive * 6);

}