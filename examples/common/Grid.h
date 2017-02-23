#include "..\..\diesel.h"

struct GridVertex {

	v3 p;
	v2 uv;

	GridVertex() : p(0.0f), uv(0.0f) {}
	GridVertex(const v3& pv, float u, float v) : p(pv), uv(u, v) {}
};

// ---------------------------------------------------------------
// the cube constant buffer
// ---------------------------------------------------------------
struct GridConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
};

class Grid {

public:
	Grid() {}
	~Grid() {}
	void create(v3* positions, int numCells, RID shaderID, RID textureID);
	void render(matrix* viewProjectionMatrix = 0);
private:
	GridVertex _vertices[4];
	GridConstantBuffer _constantBuffer;
	RID _blendState;
	RID _shader;
	RID _vertexDeclaration;
	RID _bufferID;
	RID _indexBuffer;
	RID _gridBuffer;
	RID _samplerState;
	RID _texture;
};