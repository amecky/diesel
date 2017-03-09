#include "WaveFrontReader.h"
#include <fstream>
#include <vector>
#include <stdint.h>

const uint32_t FNV_Prime = 0x01000193; //   16777619
const uint32_t FNV_Seed = 0x811C9DC5; // 2166136261

inline uint32_t fnv1a(const char* text, uint32_t hash = FNV_Seed) {
	const unsigned char* ptr = (const unsigned char*)text;
	while (*ptr) {
		hash = (*ptr++ ^ hash) * FNV_Prime;
	}
	return hash;
}

v3 matVec3Multiply(const float* m, const v3& v) {
	float tmp[4] = { v.x,v.y,v.z,1.0f };
	ds::matVec4Multiply(tmp, m, tmp);
	v3 ret(0.0f);
	for (int i = 0; i < 3; ++i) {
		ret.data[i] = tmp[i];
	}
	return ret;
}

int WaveFrontReader::load(const char * fileName, float* world) {
	std::ifstream file(fileName);
	if (!file) {
		return -1;
	}
	char cmd[256] = { 0 };
	std::vector<v3> vertices;
	std::vector<v2> textures;
	std::vector<v3> normals;
	bool hasNormals = false;
	bool hasUV = false;
	int cnt = 0;
	uint32_t current = 0;
	for (;; ) {
		file >> cmd;
		if (!file) {
			break;
		}
		if (*cmd == '#') {

		}
		else if (strcmp(cmd, "mtllib") == 0) {
			char name[256];
			file >> name;
			loadMaterials(name);
		}
		else if (strcmp(cmd, "usemtl") == 0) {
			char name[256];
			file >> name;
			current = fnv1a(name);
		}
		else if (strcmp(cmd, "v") == 0) {
			float x, y, z;
			file >> x >> y >> z;
			v3 v(x, y, z);
			vertices.push_back(v);
		}
		else if (strcmp(cmd, "vt") == 0) {
			float u,v;
			file >> u >> v;
			v2 uv(u, v);
			textures.push_back(uv);
			hasUV = true;
		}
		else if (strcmp(cmd, "vn") == 0) {
			float x, y, z;
			file >> x >> y >> z;
			v3 v(x, y, z);
			normals.push_back(v);
			hasNormals = true;
		}
		else if (strcmp(cmd, "f") == 0) {
			for (int i = 0; i < 3; ++i) {
				int v = -1;
				int t = -1;
				int n = -1;
				file >> v;
				file.ignore();
				if (hasUV) {
					file >> t;
				}
				file.ignore();
				if (hasNormals) {
					file >> n;
				}
				ObjVertex ov;
				ov.position = vertices[v - 1];
				if (world != 0) {
					ov.position = matVec3Multiply(world,ov.position);
				}
				if (t != -1) {
					ov.texture = textures[t - 1];
				}
				if (n != -1) {
					if (world != 0) {
						ov.normal = matVec3Multiply(world, normals[n - 1]);
					}
					else {
						ov.normal = normals[n - 1];
					}
				}
				if (current != 0) {
					ov.color = _materials[current];
				}
				_vertices.push_back(ov);
			}
		}

	}	
	v3 e(0.0f);
	for (size_t i = 0; i < _vertices.size(); ++i) {
		e.x += std::abs(_vertices[i].position.x);
		e.y += std::abs(_vertices[i].position.y);
		e.z += std::abs(_vertices[i].position.z);
	}
	for (int i = 0; i < 3; ++i) {
		_extent[i] = e[i] / _vertices.size();
	}
	return cnt;
}

void WaveFrontReader::loadMaterials(const char * fileName) {
	std::ifstream file(fileName);
	if (file) {
		char cmd[256] = { 0 };	
		uint32_t current = 0;
		for (;; ) {
			file >> cmd;
			if (!file) {
				break;
			}
			if (*cmd == '#') {

			}
			else if (strcmp(cmd, "newmtl") == 0) {
				char name[256];
				file >> name;
				current = fnv1a(name);
			}
			else if (strcmp(cmd, "Kd") == 0) {
				float r,g,b;
				file >> r >> g >> b;
				ds::Color c(r, g, b, 1.0f);
				_materials[current] = c;
			}
		}
	}
}
