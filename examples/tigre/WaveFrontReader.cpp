#include "WaveFrontReader.h"
#include <fstream>
#include <vector>

int WaveFrontReader::load(const char * fileName, const WaveFrontFormat& format, ds::vec3* positions, ds::vec2* uvs, ds::vec3* normals, int max) {
	std::ifstream file(fileName);
	if (!file) {
		return -1;
	}
	char cmd[256] = { 0 };
	std::vector<ds::vec3> vertices;
	std::vector<ds::vec2> textures;
	std::vector<ds::vec3> ns;
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
			ns.push_back(v);
		}
		else if (strcmp(cmd, "f") == 0) {
			int nr = 3;
			if (format.topology == WFT_QUAD) {
				nr = 4;
			}
			for (int i = 0; i < nr; ++i) {
				int v, t, n;
				file >> v;
				file.ignore();
				if ((format.formats & 1) == 1) {
					file >> t;
				}
				file.ignore();
				if ((format.formats & 2) == 2) {
					file >> n;
				}
				if (cnt < max) {
					positions[cnt] = vertices[v - 1];
					if ((format.formats & 1) == 1 && uvs != 0) {
						uvs[cnt] = textures[t - 1];
					}
					if ((format.formats & 2) == 2 && normals != 0) {
						normals[cnt] = ns[n - 1];
					}
					++cnt;
				}
			}			
		}

	}	
	return cnt;
}
