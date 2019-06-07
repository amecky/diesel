#pragma once
#include "..\..\diesel.h"

struct Bullet {
	ds::vec3 pos;
	ds::vec3 velocity;
};

const int MAX_BULLETS = 1024;

struct Bullets {
	Bullet bullet[MAX_BULLETS];
	int num;
	int capacity;
};

void move_bullets(Bullets* bullets, float dt);

void createBulletBillboards(Bullets* bullets);