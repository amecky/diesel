#pragma once
#include "..\..\diesel.h"

struct ObjVertex {
	v3 position;
	v2 texture;

	ObjVertex() : position(0.0f), texture(0.0f) {}
	ObjVertex(const v3& p, const v2& t) : position(p), texture(t) {}
};

class WaveFrontReader {

public:
	int load(const char* fileName, ObjVertex* objVertices, int max);
};