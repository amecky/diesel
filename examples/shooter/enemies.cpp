#include "enemies.h"

void move_enemies(Enemies& enemies, float dt) {
	for (uint32_t i = 0; i < enemies.numObjects; ++i) {
		enemies.objects[i].timer += dt;
	}
}