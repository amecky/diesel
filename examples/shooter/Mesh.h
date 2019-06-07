#pragma once
#include "..\..\diesel.h"

namespace ds {

	struct PNCVertex {
		ds::vec3 p;
		ds::vec3 n;
		ds::Color c;
	};

	struct LightBuffer {
		ds::matrix viewProjectionMatrix;
		ds::matrix worldMatrix;
		ds::Color ambientColor;
		ds::vec3 lightDirection;
		float more;
		ds::Color diffuseColor;
	};

	struct Mesh {
		RID drawItem;
		PNCVertex* vertices;
		LightBuffer lightBuffer;
		ds::vec3 pos;
		ds::vec3 scale;
		ds::vec3 rotation;
	};

	void createMesh(Mesh* mesh, const char* fileName);

	void renderMesh(Mesh* mesh, RID renderPass, const ds::matrix& viewProjectionMatrix);

	void moveMeshByKeyboard(Mesh* mesh, float velocity, float dt);

}