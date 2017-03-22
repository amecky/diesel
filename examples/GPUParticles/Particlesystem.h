#pragma once
#include <stdint.h>
#include "..\..\diesel.h"

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
struct ParticleVertex {

	ds::vec3 position;
	ds::vec3 velocity;
	ds::vec2 ttl;
	ds::vec4 size;

	ParticleVertex() : position(0, 0, 0) , velocity(0.0f) , ttl(0.0f) , size(1.0f) {}
	ParticleVertex(const ds::vec3& p, const ds::vec3& v, const ds::vec2& t, const ds::vec4& s) : position(p), velocity(v), ttl(t) , size(s) {}
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
		int size = maxParticles * (sizeof(ds::vec3) + sizeof(ds::vec3) + sizeof(ds::vec3) + sizeof(ds::vec4));
		buffer = new char[size];
		positions = (ds::vec3*)(buffer);
		velocities = (ds::vec3*)(positions + maxParticles);
		timers = (ds::vec3*)(velocities + maxParticles);
		sizes = (ds::vec4*)(timers + maxParticles);
		count = maxParticles;
		countAlive = 0;
	}

	void swapData(uint32_t a, uint32_t b) {
		if (a != b) {
			positions[a] = positions[b];
			velocities[a] = velocities[b];
			timers[a] = timers[b];
			sizes[a] = sizes[b];
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
};

// -------------------------------------------------------
// Particlesystem
// -------------------------------------------------------
class Particlesystem {

public:
	Particlesystem(ParticlesystemDescriptor descriptor, RID renderPass);
	void add(const ds::vec3& pos, ParticleDescriptor descriptor);
	void tick(float dt);
	void render();
private:
	RID _renderPass;
	ParticlesystemDescriptor _descriptor;
	ParticleConstantBuffer _constantBuffer;
	ParticleArray _array;
	ParticleVertex* _vertices;
	//ds::matrix _viewprojectionMatrix;
	RID _drawItem;
	RID _vertexBuffer;
};