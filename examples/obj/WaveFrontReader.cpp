#include "WaveFrontReader.h"
#include <fstream>
#include "..\..\math.h"
#include <vector>

int WaveFrontReader::load(const char * fileName, ObjVertex* objVertices, int max) {
	std::ifstream file(fileName);
	if (!file) {
		return -1;
	}
	char cmd[256] = { 0 };
	std::vector<v3> vertices;
	std::vector<v2> textures;
	std::vector<v3> normals;
	int cnt = 0;
	for (;; ) {
		file >> cmd;
		if (!file) {
			break;
		}
		if (*cmd == '#') {

		}
		/*
		v 0.500000 -0.500000 -0.500000
		vt 1 1
		vn 0.000000 -1.000000 0.000000
		f 1/1/1 2/2/1 3/3/1 4/4/1
		*/
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
		}
		else if (strcmp(cmd, "vn") == 0) {
			float x, y, z;
			file >> x >> y >> z;
			v3 v(x, y, z);
			normals.push_back(v);
		}
		else if (strcmp(cmd, "f") == 0) {
			for (int i = 0; i < 4; ++i) {
				int v, t, n;
				file >> v;
				file.ignore();
				file >> t;
				file.ignore();
				file >> n;
				printf("%d %d %d %d\n", i, v, t, n);
				ObjVertex ov;
				ov.position = vertices[v - 1];
				ov.texture = textures[t - 1];
				ov.normals = normals[n - 1];
				if (cnt < max) {
					objVertices[cnt++] = ov;
				}
			}
		}

	}
	printf("vertices: %d\n", cnt);
	for (int i = 0; i < cnt; ++i) {
		const ObjVertex& ov = objVertices[i];
		printf("%d : %g %g %g\n", i,ov.position.x, ov.position.y, ov.position.z);
	}
	return cnt;
}
