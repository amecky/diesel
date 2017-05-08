#pragma once
#include <stdint.h>
#include "..\..\diesel.h"
#include <vector>

struct GPUParticle {
	ds::vec2 position;
	float rotation;
	ds::vec2 velocity;
	ds::vec2 acceleration;
	ds::vec2 timer;
};

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct ParticleConstantBuffer {
	ds::vec4 screenCenter;
	ds::Color startColor;
	ds::Color endColor;
	ds::vec4 scale;
	ds::vec4 texture;
	ds::vec2 dimension;
	ds::vec2 dummy;
	ds::matrix wvp;
};

// -------------------------------------------------------
// Particle array
// -------------------------------------------------------
struct ParticleArray {

	ds::vec3* positions;
	ds::vec3* velocities;
	ds::vec3* accelerations;
	float* rotations;
	ds::vec3* timers;
	char* buffer;

	uint32_t count;
	uint32_t countAlive;

	ParticleArray() : count(0), countAlive(0), buffer(0) {}

	~ParticleArray() {
		if (buffer != 0) {
			delete[] buffer;
		}
	}

	void initialize(unsigned int maxParticles) {
		int size = maxParticles * (sizeof(ds::vec3) + sizeof(ds::vec3) + sizeof(ds::vec3) + sizeof(float) + sizeof(ds::vec3));
		buffer = new char[size];
		positions = (ds::vec3*)(buffer);
		velocities = (ds::vec3*)(positions + maxParticles);
		accelerations = (ds::vec3*)(velocities + maxParticles);
		rotations = (float*)(accelerations + maxParticles);
		timers = (ds::vec3*)(rotations + maxParticles);
		count = maxParticles;
		countAlive = 0;
	}

	void swapData(uint32_t a, uint32_t b) {
		if (a != b) {
			positions[a] = positions[b];
			velocities[a] = velocities[b];
			accelerations[a] = accelerations[b];
			rotations[a] = rotations[b];
			timers[a] = timers[b];
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
	ds::vec2 particleDimension;
	RID textureID;
	ds::vec2 scale;
	ds::vec2 growth;
	ds::Color startColor;
	ds::Color endColor;
	ds::vec4 textureRect;
};

// -------------------------------------------------------
// Particlesystem
// -------------------------------------------------------
struct Particlesystem {
	ParticlesystemDescriptor* descriptor;
	ParticleArray array;
};

// -------------------------------------------------------
// ParticlesystemRenderer
// -------------------------------------------------------
class ParticleManager {

public:
	ParticleManager(int maxParticles, RID textureID);
	//void add(Particlesystem* system);
	uint32_t add(ParticlesystemDescriptor* descriptor);
	void add(uint32_t id, const ds::vec2& pos, const ds::vec2& velocity, const ds::vec2& acceleration, float ttl, float rotation);
	void tick(float dt);
	void render();
	int numAlive(uint32_t id) const {
		return _partSystems[id].array.countAlive;
	}
private:
	void prepareBuffer(const ParticlesystemDescriptor& descriptor);
	//std::vector<Particlesystem*> _systems;
	ParticleConstantBuffer _constantBuffer;
	std::vector<Particlesystem> _partSystems;
	GPUParticle* _vertices;
	ds::matrix _viewprojectionMatrix;
	RID _drawItem;
	RID _orthoPass;
	RID _structuredBufferId;
};