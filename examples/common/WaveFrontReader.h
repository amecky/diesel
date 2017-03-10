#pragma once
#include "..\..\diesel.h"
#include <vector>
#include <map>

struct ObjVertex {
	ds::vec3 position;
	ds::vec2 texture;
	ds::vec3 normal;
	ds::Color color;

	ObjVertex() : position(0.0f), texture(0.0f) , normal(0.0f) , color(255,255,255,255) {}
	ObjVertex(const ds::vec3& p, const ds::vec2& t, const ds::vec3& n) : position(p), texture(t) , normal(n) {}
};

class WaveFrontReader {

public:
	int load(const char* fileName, ds::matrix* world = 0);
	size_t size() const {
		return _vertices.size();
	}
	const ObjVertex& get(int i) const {
		return _vertices[i];
	}
	const ds::vec3& getExtent() const {
		return _extent;
	}
private:
	void loadMaterials(const char* fileName);
	std::vector<ObjVertex> _vertices;
	std::map<uint32_t, ds::Color> _materials;
	ds::vec3 _extent;
};