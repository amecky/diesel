#pragma once
#include "..\..\diesel.h"

struct ObjVertex {
	ds::vec3 position;
	ds::vec2 texture;

	ObjVertex() : position(0.0f), texture(0.0f) {}
	ObjVertex(const ds::vec3& p, const ds::vec2& t) : position(p), texture(t) {}
};

class WaveFrontReader {

public:
	int load(const char* fileName, ObjVertex* objVertices, int max);
};