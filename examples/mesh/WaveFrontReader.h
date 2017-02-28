#pragma once
#include "..\..\diesel.h"

struct ObjVertex {
	v3 position;
	v2 texture;
	v3 normal;
	ds::Color color;

	ObjVertex() : position(0.0f), texture(0.0f) , normal(0.0f) , color(192,0,0,255) {}
	ObjVertex(const v3& p, const v2& t, const v3& n) : position(p), texture(t) , normal(n), color(192, 0, 0, 255) {}
};

class WaveFrontReader {

public:
	int load(const char* fileName, ObjVertex* objVertices, int max);
};