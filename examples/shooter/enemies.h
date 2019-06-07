#include "..\..\diesel.h"
#include "Mesh.h"
#include "DataArray.h"

typedef void(*animationPtr)(ds::Mesh*);

struct Enemy {
	int id;
	float timer;
	ds::Mesh* mesh;
	animationPtr animation;
};

typedef ds::DataArray<Enemy, 32> Enemies;

void move_enemies(Enemies& enemies, float dt);
