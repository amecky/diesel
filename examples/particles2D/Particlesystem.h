#pragma once
#include <stdint.h>
#include "..\..\diesel.h"

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
struct ParticleVertex {

	ds::vec3 position;
	ds::vec2 dim;
	ds::vec3 size;
	ds::Color color;

	ParticleVertex() : position(0, 0, 0) , dim(100,100) , size(1.0f,1.0f,0.0f) , color(ds::Color(1.0f,1.0f,1.0f,1.0f)) {}
	ParticleVertex(const ds::vec3& p, const ds::vec2& d, const ds::vec3& s, const ds::Color& c) : position(p), dim(d), size(s), color(c) {}
};

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct ParticleConstantBuffer {
	ds::vec4 screenDimension;
	ds::vec4 screenCenter;
	ds::matrix wvp;
};

// -------------------------------------------------------
// Particle array
// -------------------------------------------------------
struct ParticleArray {

	ds::vec3* positions;
	ds::vec3* velocities;
	float* frictions;
	float* decays;
	ds::vec2* scales;
	ds::vec2* grow;
	float* rotations;
	float* rotationSpeeds;
	ds::vec3* timers;
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
		int size = maxParticles * (sizeof(ds::vec3) + sizeof(ds::vec3) + sizeof(float) + sizeof(float) + sizeof(ds::vec2) + sizeof(ds::vec2) + sizeof(float) + sizeof(float) + sizeof(ds::vec3) + sizeof(ds::Color));
		buffer = new char[size];
		positions = (ds::vec3*)(buffer);
		velocities = (ds::vec3*)(positions + maxParticles);
		frictions = (float*)(velocities + maxParticles);
		decays = (float*)(frictions + maxParticles);
		scales = (ds::vec2*)(decays + maxParticles);
		grow = (ds::vec2*)(scales + maxParticles);
		rotations = (float*)(grow + maxParticles);
		rotationSpeeds = (float*)(rotations + maxParticles);
		timers = (ds::vec3*)(rotationSpeeds + maxParticles);
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
	ds::vec2 particleDimension;
	RID textureID;
};

// -------------------------------------------------------
// Particle descriptor
// -------------------------------------------------------
struct ParticleDescriptor {
	float ttl;
	ds::vec2 scale;
	ds::vec2 growth;
	ds::vec2 velocity;
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
	void add(const ds::vec2& pos, ParticleDescriptor descriptor);
	void tick(float dt);
	void render();
private:
	ParticlesystemDescriptor _descriptor;
	ParticleConstantBuffer _constantBuffer;
	ParticleArray _array;
	ParticleVertex* _vertices;
	ds::matrix _viewprojectionMatrix;
	RID _drawItem;
	RID _vertexBuffer;
};