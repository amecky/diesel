#include "Geometry.h"
#include <cmath>

const v3 CUBE_VERTICES[8] = {
	v3(-0.5f,-0.5f,-0.5f),
	v3(-0.5f, 0.5f,-0.5f),
	v3(0.5f, 0.5f,-0.5f),
	v3(0.5f,-0.5f,-0.5f),
	v3(-0.5f,-0.5f, 0.5f),
	v3(-0.5f, 0.5f, 0.5f),
	v3(0.5f, 0.5f, 0.5f),
	v3(0.5f,-0.5f, 0.5f)
};

const int CUBE_PLANES[6][4] = {
	{ 0, 1, 2, 3 }, // front
	{ 3, 2, 6, 7 }, // left
	{ 1, 5, 6, 2 }, // top
	{ 4, 5, 1, 0 }, // right
	{ 4, 0, 3, 7 }, // bottom
	{ 7, 6, 5, 4 }, // back
};

namespace geometry {

	void buildCube(const matrix& m, v3* positions, v2* uvs, v3* normals) {
		for (int side = 0; side < 6; ++side) {
			int idx = side * 4;
			float u[4] = { 0.0f,0.0f,1.0f,1.0f };
			float v[4] = { 1.0f,0.0f,0.0f,1.0f };
			for (int i = 0; i < 4; ++i) {
				int p = idx + i;
				positions[p] = m * CUBE_VERTICES[CUBE_PLANES[side][i]];
				if (uvs != 0) {
					uvs[p] = v2(u[i], v[i]);
				}
			}
			if (normals != 0) {
				v3 d0 = positions[idx + 1] - positions[idx];
				v3 d1 = positions[idx + 2] - positions[idx];
				v3 c = cross(d0, d1);
				for (int i = 0; i < 4; ++i) {
					normals[idx + i] = c;
				}
			}
		}
	}

	void buildCylinder(const matrix& m, int segments, v3* positions, v2* uvs, v3* normals) {
		float du = 1.0f / static_cast<float>(segments);
		float su = 0.0f;
		float steps = ds::TWO_PI / static_cast<float>(segments);
		v2 uvMap[] = {v2(0.0f,0.0f), v2(1.0f,0.0f), v2(1.0f,1.0f), v2(0.0f,1.0f) };
		int cnt = 0;
		int uvcnt = 0;
		float radius = 0.5f;
		float angle = 0.0f;
		for (int i = 0; i < segments; ++i) {
			float fx = radius * cos(angle);
			float fz = radius * sin(angle);
			angle += steps;
			float sx = radius * cos(angle);
			float sz = radius * sin(angle);
			positions[cnt++] = m * v3(fx, 0.5f, fz);
			positions[cnt++] = m * v3(sx, 0.5f, sz);
			positions[cnt++] = m * v3(sx, -0.5f, sz);
			positions[cnt++] = m * v3(fx, -0.5f, fz);
			if (uvs != 0) {
				for (int j = 0; j < 4; ++j) {
					uvs[uvcnt++] = uvMap[j];
				}
			}
			if (normals != 0) {

			}
		}
	}
}