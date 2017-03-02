#include "WaveFrontReader.h"
#include <fstream>
#include <vector>

int WaveFrontReader::load(const char * fileName, ObjVertex* objVertices, int max, matrix* world) {
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
	for (;; ) {
		file >> cmd;
		if (!file) {
			break;
		}
		if (*cmd == '#') {

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
					ov.position = *world * ov.position;
				}
				if (t != -1) {
					ov.texture = textures[t - 1];
				}
				if (n != -1) {
					ov.normal = normals[n - 1];
				}
				if (cnt < max) {
					objVertices[cnt++] = ov;
				}
			}
		}

	}	
	return cnt;
}
