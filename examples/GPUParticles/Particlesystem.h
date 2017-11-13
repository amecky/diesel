#pragma once
#include <stdint.h>
#include "..\..\diesel.h"

struct GPUParticle {
	ds::vec3 position;
	ds::vec3 velocity;
	ds::vec3 acceleration;
	ds::vec2 timer;
	ds::vec2 scale;
	ds::vec2 growth;

};

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct ParticleConstantBuffer {
	ds::Color startColor;
	ds::Color endColor;
	ds::matrix wvp;
	ds::matrix world;
	ds::vec3 eyePos;
	float padding;
};

// -------------------------------------------------------
// Particle array
// -------------------------------------------------------
struct ParticleArray {

	ds::vec3* positions;
	ds::vec3* velocities;
	ds::vec3* timers;
	ds::vec4* sizes;
	ds::vec3* accelerations;
	char* buffer;

	uint32_t count;
	uint32_t countAlive;

	ParticleArray() : count(0), countAlive(0), buffer(0), positions(0), velocities(0), timers(0), sizes(0), accelerations(0) {}

	~ParticleArray() {
		if (buffer != 0) {
			delete[] buffer;
		}
	}

	void initialize(unsigned int maxParticles) {
		int size = maxParticles * (sizeof(ds::vec3) + sizeof(ds::vec3) + sizeof(ds::vec3) + sizeof(ds::vec4) + sizeof(ds::vec3));
		buffer = new char[size];
		positions = (ds::vec3*)(buffer);
		velocities = (ds::vec3*)(positions + maxParticles);
		timers = (ds::vec3*)(velocities + maxParticles);
		sizes = (ds::vec4*)(timers + maxParticles);
		accelerations = (ds::vec3*)(sizes + maxParticles);
		count = maxParticles;
		countAlive = 0;
	}

	void swapData(uint32_t a, uint32_t b) {
		if (a != b) {
			positions[a] = positions[b];
			velocities[a] = velocities[b];
			timers[a] = timers[b];
			sizes[a] = sizes[b];
			accelerations[a] = accelerations[b];
		}
	}

	void kill(uint32_t id) {
		if (countAlive > 0) {
			swapData(id, countAlive - 1);
			--countAlive;
		}
	}

	void wake(uint32_t id) {
		if (countAlive < count) {
			++countAlive;
		}
	}

};

// -------------------------------------------------------
// Particlesystem descriptor
// -------------------------------------------------------
struct ParticlesystemDescriptor {
	uint16_t maxParticles;
	RID texture;
	ds::vec2 particleDimension;
	ds::Color startColor;
	ds::Color endColor;
};

// -------------------------------------------------------
// Particle descriptor
// -------------------------------------------------------
struct ParticleDescriptor {
	float ttl;
	ds::vec2 minScale;
	ds::vec2 maxScale;
	ds::vec3 velocity;
	float friction;	
	ds::vec3 acceleration;
};

// -------------------------------------------------------
// Particlesystem
// -------------------------------------------------------
class Particlesystem {

public:
	Particlesystem(ds::Camera* camera, ParticlesystemDescriptor descriptor, RID renderPass);
	void add(const ds::vec3& pos, ParticleDescriptor descriptor);
	void tick(float dt);
	void render();
	int countAlive() const {
		return _array.countAlive;
	}
private:
	RID _renderPass;
	ParticlesystemDescriptor _descriptor;
	ParticleConstantBuffer _constantBuffer;
	ParticleArray _array;
	GPUParticle* _vertices;
	RID _drawItem;
	ds::Camera* _camera;
	RID _structuredBufferId;
};