#pragma once
#include "..\..\diesel.h"

struct Star {
	ds::vec3 pos;
	ds::vec3 velocity;
	int layer;
};

const int STARFIELD_LAYERS = 3;
const int STARS_PER_LAYER = 128;
const int TOTAL_STARS = STARFIELD_LAYERS * STARS_PER_LAYER;

class Starfield {

public:
	Starfield();
	void moveStars(float dt);
	void createBillboards();
private:
	Star _stars[TOTAL_STARS];

};