#include "Geometry.h"
#include <cmath>

const ds::vec3 CUBE_VERTICES[8] = {
	ds::vec3(-0.5f,-0.5f,-0.5f),
	ds::vec3(-0.5f, 0.5f,-0.5f),
	ds::vec3(0.5f, 0.5f,-0.5f),
	ds::vec3(0.5f,-0.5f,-0.5f),
	ds::vec3(-0.5f,-0.5f, 0.5f),
	ds::vec3(-0.5f, 0.5f, 0.5f),
	ds::vec3(0.5f, 0.5f, 0.5f),
	ds::vec3(0.5f,-0.5f, 0.5f)
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

	void buildCube(const ds::matrix& m, ds::vec3* positions, ds::vec2* uvs, ds::vec3* normals) {
		for (int side = 0; side < 6; ++side) {
			int idx = side * 4;
			float u[4] = { 0.0f,0.0f,1.0f,1.0f };
			float v[4] = { 1.0f,0.0f,0.0f,1.0f };
			for (int i = 0; i < 4; ++i) {
				int p = idx + i;
				positions[p] = m * CUBE_VERTICES[CUBE_PLANES[side][i]];
				if (uvs != 0) {
					uvs[p] = ds::vec2(u[i], v[i]);
				}
			}
			if (normals != 0) {
				ds::vec3 d0 = positions[idx + 1] - positions[idx];
				ds::vec3 d1 = positions[idx + 2] - positions[idx];
				ds::vec3 c = cross(d0, d1);
				for (int i = 0; i < 4; ++i) {
					normals[idx + i] = c;
				}
			}
		}
	}

	void buildCylinder(const ds::matrix& m, int segments, ds::vec3* positions, ds::vec2* uvs, ds::vec3* normals) {
		float du = 1.0f / static_cast<float>(segments);
		float su = 0.0f;
		float steps = ds::TWO_PI / static_cast<float>(segments);
		ds::vec2 uvMap[] = {ds::vec2(0.0f,0.0f), ds::vec2(1.0f,0.0f), ds::vec2(1.0f,1.0f), ds::vec2(0.0f,1.0f) };
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
			positions[cnt++] = m * ds::vec3(fx, 0.5f, fz);
			positions[cnt++] = m * ds::vec3(sx, 0.5f, sz);
			positions[cnt++] = m * ds::vec3(sx, -0.5f, sz);
			positions[cnt++] = m * ds::vec3(fx, -0.5f, fz);
			if (uvs != 0) {
				for (int j = 0; j < 4; ++j) {
					uvs[uvcnt++] = uvMap[j];
				}
			}
			if (normals != 0) {

			}
		}
	}

	void buildSphere(float radius, int sliceCount, int stackCount, MeshData* data) {

		data->add(ds::vec3(0.0f, radius, 0.0f),ds::vec3(0, 1, 0), ds::vec3(1, 0, 0),ds::vec2(0, 0));

		float phiStep = ds::PI / stackCount;
		float thetaStep = 2.0f * ds::PI / sliceCount;

		for (int i = 1; i <= stackCount - 1; ++i) {
			float phi = i * phiStep;
			for (int j = 0; j <= sliceCount; ++j) {
				float theta = j * thetaStep;
				ds::vec3 p = ds::vec3(
					(radius*sin(phi)*cos(theta)),
					(radius*cos(phi)),
					(radius* sin(phi)*sin(theta))
					);

				ds::vec3 t = ds::vec3(-radius*sin(phi)*sin(theta), 0.0f, radius*sin(phi)*cos(theta));
				t = normalize(t);
				ds::vec3 n = p;
				n = normalize(n);
				ds::vec2 uv = ds::vec2(theta / (ds::PI * 2), phi / ds::PI);
				data->add(p, n, t, uv);
			}
		}
		data->add(ds::vec3(0.0f, -radius, 0.0f),ds::vec3( 0, -1, 0), ds::vec3(1, 0, 0), ds::vec2(0, 1));

		for (int i = 1; i <= sliceCount; ++i) {
			data->indices.push_back(0);
			data->indices.push_back(i + 1);
			data->indices.push_back(i);
		}
		int baseIndex = 1;
		int ringVertexCount = sliceCount + 1;
		for (int i = 0; i < stackCount - 2; ++i) {
			for (int j = 0; j < sliceCount; ++j) {
				data->indices.push_back(baseIndex + i*ringVertexCount + j);
				data->indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				data->indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

				data->indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
				data->indices.push_back(baseIndex + i*ringVertexCount + j + 1);
				data->indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
			}
		}
		int southPoleIndex = data->vertices.size() - 1;
		baseIndex = southPoleIndex - ringVertexCount;
		for (int i = 0; i < sliceCount; ++i) {
			data->indices.push_back(southPoleIndex);
			data->indices.push_back(baseIndex + i);
			data->indices.push_back(baseIndex + i + 1);
		}

	}

	void buildSphere2(float radius, int tessellation, MeshData* data) {
		size_t verticalSegments = tessellation;
		size_t horizontalSegments = tessellation * 2;		

		// Create rings of vertices at progressively higher latitudes.
		for (size_t i = 0; i <= verticalSegments; i++)
		{
			float v = 1 - (float)i / verticalSegments;

			float latitude = (i * ds::PI / verticalSegments) - ds::PI * 0.5f;
			//float dy, dxz;

			float dy = sin(latitude);
			float dxz = cos(latitude);
			//XMScalarSinCos(&dy, &dxz, latitude);

			// Create a single ring of vertices at this latitude.
			for (size_t j = 0; j <= horizontalSegments; j++)
			{
				float u = (float)j / horizontalSegments;

				float longitude = j * ds::TWO_PI / horizontalSegments;
				//float dx, dz;

				float dx = sin(longitude);
				float dz = cos(longitude);

				//XMScalarSinCos(&dx, &dz, longitude);

				dx *= dxz;
				dz *= dxz;

				ds::vec3 normal = ds::vec3(dx, dy, dz);
				//ds:: vec23textureCoordinate = XMVectorSet(u, v, 0, 0);

				data->add(normal * radius, normal, ds::vec3(0,0,0),ds::vec2(u,v));
			}
		}

		// Fill the index buffer with triangles joining each pair of latitude rings.
		size_t stride = horizontalSegments + 1;

		for (size_t i = 0; i < verticalSegments; i++)
		{
			for (size_t j = 0; j <= horizontalSegments; j++)
			{
				size_t nextI = i + 1;
				size_t nextJ = (j + 1) % stride;

				data->indices.push_back(i * stride + j);
				data->indices.push_back(nextI * stride + j);
				data->indices.push_back(i * stride + nextJ);

				data->indices.push_back(i * stride + nextJ);
				data->indices.push_back(nextI * stride + j);
				data->indices.push_back(nextI * stride + nextJ);
			}
		}
	}
}