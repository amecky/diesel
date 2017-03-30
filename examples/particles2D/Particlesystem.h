#pragma once
#include <stdint.h>
#include "..\..\diesel.h"
#include <vector>

// ---------------------------------------------------------------
// The particle vertex
// ---------------------------------------------------------------
struct ParticleVertex {
	ds::vec3 pos;
	ds::vec3 velocity;
	ds::vec3 acceleration;
	ds::vec4 timer; // timer / ttl / norm / rotation
};


// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct ParticleConstantBuffer {
	ds::vec4 screenDimension;
	ds::vec4 screenCenter;
	ds::Color startColor;
	ds::Color endColor;
	ds::vec4 scale;
	ds::vec4 texture;
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
class Particlesystem {

public:
	Particlesystem(ParticlesystemDescriptor descriptor);
	void add(const ds::vec2& pos, const ds::vec2& velocity, const ds::vec2& acceleration, float ttl, float rotation);
	void tick(float dt);
	const ParticleArray* getArray() const {
		return &_array;
	}
	const ParticlesystemDescriptor& getDescriptor() const {
		return _descriptor;
	}
	void preapreBuffer(ParticleConstantBuffer* constantBuffer) {
		ds::vec2 endScale = _descriptor.scale;
		endScale.x += _descriptor.scale.x * _descriptor.growth.x;
		endScale.y += _descriptor.scale.y * _descriptor.growth.y;
		constantBuffer->startColor = _descriptor.startColor;
		constantBuffer->endColor = _descriptor.endColor;
		constantBuffer->scale = ds::vec4(_descriptor.scale.x, _descriptor.scale.y, endScale.x, endScale.y);
		constantBuffer->texture = _descriptor.textureRect;
	}
private:
	ParticlesystemDescriptor _descriptor;
	ParticleArray _array;
};

// -------------------------------------------------------
// ParticlesystemRenderer
// -------------------------------------------------------
class ParticleManager {

public:
	ParticleManager(int maxParticles, RID textureID);
	void add(Particlesystem* system);
	void tick(float dt);
	void render();
private:
	std::vector<Particlesystem*> _systems;
	ParticleConstantBuffer _constantBuffer;
	
	ParticleVertex* _vertices;
	ds::matrix _viewprojectionMatrix;
	RID _drawItem;
	RID _vertexBuffer;
	RID _orthoPass;
};