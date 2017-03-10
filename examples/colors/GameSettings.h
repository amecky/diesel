#pragma once
#include "..\..\diesel.h"

struct GameSettings {

	// TTL for tiles to move in
	float moveInTTL;
	// the initial which is added to y
	float moveInYAdd;
	// the individual y offset based on row
	float moveInYOffset;

	float flashTTL;
	float droppingTTL;
	
};