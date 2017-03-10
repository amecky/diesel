#include "WaveFrontReader.h"
#include <fstream>
#include <vector>

int WaveFrontReader::load(const char * fileName, ObjVertex* objVertices, int max) {
	std::ifstream file(fileName);
	if (!file) {
		return -1;
	}
	char cmd[256] = { 0 };
	std::vector<ds::vec3> vertices;
	std::vector<ds::vec2> textures;
	std::vector<ds::vec3> normals;
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
			ds::vec3 v(x, y, z);
			vertices.push_back(v);
		}
		else if (strcmp(cmd, "vt") == 0) {
			float u,v;
			file >> u >> v;
			ds::vec2 uv(u, v);
			textures.push_back(uv);
		}
		else if (strcmp(cmd, "vn") == 0) {
			float x, y, z;
			file >> x >> y >> z;
			ds::vec3 v(x, y, z);
			normals.push_back(v);
		}
		else if (strcmp(cmd, "f") == 0) {
			for (int i = 0; i < 4; ++i) {
				int v, t;
				file >> v;
				file.ignore();
				file >> t;
				ObjVertex ov;
				ov.position = vertices[v - 1];
				ov.texture = textures[t - 1];
				if (cnt < max) {
					objVertices[cnt++] = ov;
				}
			}
		}

	}	
	return cnt;
}
