#pragma once
#include "..\..\diesel.h"

enum WaveFrontTopology {
	WFT_QUAD,
	WFT_TRIANGLE
};

enum WaveFrontItem {
	WFI_TEXTURE = 1,
	WFI_NORMALS = 2

};
struct WaveFrontFormat {
	int formats;
	WaveFrontTopology topology;
};
struct ObjVertex {
	ds::vec3 position;
	ds::vec2 texture;

	ObjVertex() : position(0.0f), texture(0.0f) {}
	ObjVertex(const ds::vec3& p, const ds::vec2& t) : position(p), texture(t) {}
};

class WaveFrontReader {

public:
	int load(const char* fileName, const WaveFrontFormat& format, ds::vec3* positions, ds::vec2* uvs,ds::vec3* normals, int max);
};