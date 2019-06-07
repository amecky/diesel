#pragma once
#include "..\diesel.h"
#include <vector>
#include <map>

#define DS_OBJ_LOADER

namespace ds {

	struct ObjVertex {
		ds::vec3 position;
		ds::vec2 texture;
		ds::vec3 normal;
		ds::Color color;

		ObjVertex() : position(0.0f), texture(0.0f), normal(0.0f), color(255, 255, 255, 255) {}
		ObjVertex(const ds::vec3& p, const ds::vec2& t, const ds::vec3& n) : position(p), texture(t), normal(n) {}
	};

	class ObjLoader {

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

}

#ifdef DS_OBJ_LOADER

#include <fstream>
#include <vector>
#include <stdint.h>

const uint32_t FNV_Prime = 0x01000193; //   16777619
const uint32_t FNV_Seed = 0x811C9DC5; // 2166136261

inline uint32_t ds__ol_fnv1a(const char* text, uint32_t hash = FNV_Seed) {
	const unsigned char* ptr = (const unsigned char*)text;
	while (*ptr) {
		hash = (*ptr++ ^ hash) * FNV_Prime;
	}
	return hash;
}

namespace ds {

	int ObjLoader::load(const char * fileName, ds::matrix* world) {
		std::ifstream file(fileName);
		if (!file) {
			return -1;
		}
		char cmd[256] = { 0 };
		std::vector<ds::vec3> vertices;
		std::vector<ds::vec2> textures;
		std::vector<ds::vec3> normals;
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
				ds::vec3 v(x, y, z);
				vertices.push_back(v);
			}
			else if (strcmp(cmd, "vt") == 0) {
				float u, v;
				file >> u >> v;
				ds::vec2 uv(u, v);
				textures.push_back(uv);
				hasUV = true;
			}
			else if (strcmp(cmd, "vn") == 0) {
				float x, y, z;
				file >> x >> y >> z;
				ds::vec3 v(x, y, z);
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
						ov.position = *world * ov.position;
					}
					if (t != -1) {
						ov.texture = textures[t - 1];
					}
					if (n != -1) {
						if (world != 0) {
							ov.normal = *world * normals[n - 1];
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
		ds::vec3 minExt(1000.0f, 1000.0f, 1000.0f);
		ds::vec3 maxExt(0.0f, 0.0f, 0.0f);
		ds::vec3 e(0.0f);
		for (size_t i = 0; i < _vertices.size(); ++i) {
			const ds::vec3& current = _vertices[i].position;
			for (int j = 0; j < 3; ++j) {
				if (current.data[j] > maxExt.data[j]) {
					maxExt.data[j] = current.data[j];
				}
				if (current.data[j] < minExt.data[j]) {
					minExt.data[j] = current.data[j];
				}
			}
			e.x += std::abs(_vertices[i].position.x);
			e.y += std::abs(_vertices[i].position.y);
			e.z += std::abs(_vertices[i].position.z);
		}
		ds::vec3 center = (minExt + maxExt) * 0.5f;
		for (int i = 0; i < 3; ++i) {
			_extent.data[i] = e.data[i] / _vertices.size();
		}
		return _vertices.size();
	}

	void ObjLoader::loadMaterials(const char * fileName) {
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
					float r, g, b;
					file >> r >> g >> b;
					ds::Color c(r, g, b, 1.0f);
					_materials[current] = c;
				}
			}
		}
	}

}

#endif