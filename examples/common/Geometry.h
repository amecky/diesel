#pragma once
#include "..\..\diesel.h"
#include <vector>

namespace geometry {

	struct MeshData {
		std::vector<ds::vec3> vertices;
		std::vector<ds::vec2> uvs;
		std::vector<ds::vec3> normals;
		std::vector<ds::vec3> tangents;
		std::vector<int> indices;

		void add(const ds::vec3& p, const ds::vec3& n, const ds::vec3& t, const ds::vec2& uv) {
			vertices.push_back(p);
			normals.push_back(n);
			tangents.push_back(t);
			uvs.push_back(uv);
		}
	};

	void buildCube(const ds::matrix& m, ds::vec3* positions, ds::vec2* uvs = 0, ds::vec3* normals = 0);

	void buildCylinder(const ds::matrix& m, int segments, ds::vec3* positions, ds::vec2* uvs = 0, ds::vec3* normals = 0);

	void buildSphere(float radius, int sliceCount, int stackCount, MeshData* data);

	void buildSphere2(float radius, int tessellation, MeshData* data);
}