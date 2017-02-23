#pragma once
#include <stdint.h>
#include "..\..\diesel.h"

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
struct ParticleVertex {

	v3 position;
	v3 velocity;
	v2 ttl;
	v4 size;

	ParticleVertex() : position(0, 0, 0) , velocity(0.0f) , ttl(0.0f) , size(1.0f) {}
	ParticleVertex(const v3& p, const v3& v, const v2& t, const v4& s) : position(p), velocity(v), ttl(t) , size(s) {}
};

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct ParticleConstantBuffer {
	ds::Color startColor;
	ds::Color endColor;
	matrix wvp;
	matrix world;
	v3 eyePos;
	float padding;
};

// -------------------------------------------------------
// Particle array
// -------------------------------------------------------
struct ParticleArray {

	v3* positions;
	v3* velocities;
	v3* timers;
	v4* sizes;
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
		int size = maxParticles * (sizeof(v3) + sizeof(v3) + sizeof(v3) + sizeof(v4));
		buffer = new char[size];
		positions = (v3*)(buffer);
		velocities = (v3*)(positions + maxParticles);
		timers = (v3*)(velocities + maxParticles);
		sizes = (v4*)(timers + maxParticles);
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
	RID blendState;
	RID shader;
	RID samplerState;
	RID texture;
	RID constantBuffer;
	RID vertexBuffer;
	RID vertexDeclaration;
	v2 particleDimension;
	ds::Color startColor;
	ds::Color endColor;
};

// -------------------------------------------------------
// Particle descriptor
// -------------------------------------------------------
struct ParticleDescriptor {
	float ttl;
	v2 minScale;
	v2 maxScale;
	v3 velocity;
	float friction;	
};

// -------------------------------------------------------
// Particlesystem
// -------------------------------------------------------
class Particlesystem {

public:
	Particlesystem(ParticlesystemDescriptor descriptor);
	void add(const v3& pos, ParticleDescriptor descriptor);
	void tick(float dt);
	void render();
private:
	ParticlesystemDescriptor _descriptor;
	ParticleConstantBuffer _constantBuffer;
	ParticleArray _array;
	ParticleVertex* _vertices;
	//matrix _viewProjectionMatrix;
};