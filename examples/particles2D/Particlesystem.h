#pragma once
#include <stdint.h>
#include "..\..\diesel.h"

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
struct ParticleVertex {

	v3 position;
	v2 dim;
	v3 size;
	ds::Color color;

	ParticleVertex() : position(0, 0, 0) , dim(100,100) , size(1.0f,1.0f,0.0f) , color(ds::Color(1.0f,1.0f,1.0f,1.0f)) {}
	ParticleVertex(const v3& p, const v2& d, const v3& s, const ds::Color& c) : position(p), dim(d), size(s), color(c) {}
};

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct ParticleConstantBuffer {
	v4 screenDimension;
	v4 screenCenter;
	matrix wvp;
};

// -------------------------------------------------------
// Particle array
// -------------------------------------------------------
struct ParticleArray {

	v3* positions;
	v3* velocities;
	float* frictions;
	float* decays;
	v2* scales;
	v2* grow;
	float* rotations;
	float* rotationSpeeds;
	v3* timers;
	ds::Color* colors;
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
		int size = maxParticles * (sizeof(v3) + sizeof(v3) + sizeof(float) + sizeof(float) + sizeof(v2) + sizeof(v2) + sizeof(float) + sizeof(float) + sizeof(v3) + sizeof(ds::Color));
		buffer = new char[size];
		positions = (v3*)(buffer);
		velocities = (v3*)(positions + maxParticles);
		frictions = (float*)(velocities + maxParticles);
		decays = (float*)(frictions + maxParticles);
		scales = (v2*)(decays + maxParticles);
		grow = (v2*)(scales + maxParticles);
		rotations = (float*)(grow + maxParticles);
		rotationSpeeds = (float*)(rotations + maxParticles);
		timers = (v3*)(rotationSpeeds + maxParticles);
		colors = (ds::Color*)(timers + maxParticles);
		count = maxParticles;
		countAlive = 0;
	}

	void swapData(uint32_t a, uint32_t b) {
		if (a != b) {
			positions[a] = positions[b];
			velocities[a] = velocities[b];
			frictions[a] = frictions[b];
			decays[a] = decays[b];
			scales[a] = scales[b];
			grow[a] = grow[b];
			rotations[a] = rotations[b];
			rotationSpeeds[a] = rotationSpeeds[b];
			timers[a] = timers[b];
			colors[a] = colors[b];
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
	v2 particleDimension;
	RID textureID;
};

// -------------------------------------------------------
// Particle descriptor
// -------------------------------------------------------
struct ParticleDescriptor {
	float ttl;
	v2 scale;
	v2 growth;
	v2 velocity;
	float friction;
	ds::Color color;
	float rotation;
	float rotationSpeed;
	bool alphaFading;
};

// -------------------------------------------------------
// Particlesystem
// -------------------------------------------------------
class Particlesystem {

public:
	Particlesystem(ParticlesystemDescriptor descriptor);
	void add(const v2& pos, ParticleDescriptor descriptor);
	void tick(float dt);
	void render();
private:
	ParticlesystemDescriptor _descriptor;
	ParticleConstantBuffer _constantBuffer;
	ParticleArray _array;
	ParticleVertex* _vertices;
	matrix _viewProjectionMatrix;
	ds::DrawItem* _drawItem;
	RID _vertexBuffer;
};