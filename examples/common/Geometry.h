#pragma once
#include "..\..\diesel.h"

namespace geometry {

	void buildCube(const ds::matrix& m, ds::vec3* positions, ds::vec2* uvs = 0, ds::vec3* normals = 0);

	void buildCylinder(const ds::matrix& m, int segments, ds::vec3* positions, ds::vec2* uvs = 0, ds::vec3* normals = 0);
}