#pragma once
#include "..\..\diesel.h"
#include "math.h"

namespace geometry {

	void buildCube(const float* m, v3* positions, v2* uvs = 0, v3* normals = 0);

	void buildCylinder(const float* m, int segments, v3* positions, v2* uvs = 0, v3* normals = 0);
}