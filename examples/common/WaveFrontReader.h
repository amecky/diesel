#pragma once
#include "..\..\diesel.h"
#include <vector>
#include <map>
#include "math.h"

struct ObjVertex {
	v3 position;
	v2 texture;
	v3 normal;
	ds::Color color;

	ObjVertex() : position(0.0f), texture(0.0f) , normal(0.0f) , color(255,255,255,255) {}
	ObjVertex(const v3& p, const v2& t, const v3& n) : position(p), texture(t) , normal(n) {}
};

class WaveFrontReader {

public:
	int load(const char* fileName, float* world = 0);
	size_t size() const {
		return _vertices.size();
	}
	const ObjVertex& get(int i) const {
		return _vertices[i];
	}
	const v3& getExtent() const {
		return _extent;
	}
private:
	void loadMaterials(const char* fileName);
	std::vector<ObjVertex> _vertices;
	std::map<uint32_t, ds::Color> _materials;
	v3 _extent;
};