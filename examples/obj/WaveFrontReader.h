#pragma once
#include "..\..\math.h"

struct ObjVertex {
	v3 position;
	v2 texture;
	v3 normals;
};

class WaveFrontReader {

public:
	int load(const char* fileName, ObjVertex* objVertices, int max);
};