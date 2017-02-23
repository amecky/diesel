#pragma once
#include "..\..\diesel.h"

namespace geometry {

	void buildCube(const matrix& m, v3* positions, v2* uvs = 0, v3* normals = 0);

	void buildCylinder(const matrix& m, int segments, v3* positions, v2* uvs = 0, v3* normals = 0);
}