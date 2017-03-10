#include "..\..\diesel.h"

struct GridVertex {

	ds::vec3 p;
	ds::vec2 uv;

	GridVertex() : p(0.0f), uv(0.0f) {}
	GridVertex(const ds::vec3& pv, float u, float v) : p(pv), uv(u, v) {}
};

// ---------------------------------------------------------------
// the cube constant buffer
// ---------------------------------------------------------------
struct GridConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
};

class Grid {

public:
	Grid() {}
	~Grid() {}
	void create(ds::vec3* positions, int numCells, RID shaderID, RID textureID);
	void render();
private:
	GridVertex _vertices[4];
	GridConstantBuffer _constantBuffer;
	ds::StateGroup* _gridStates;
	ds::DrawCommand drawCmd;
	//RID _blendState;
	//RID _shader;
	//RID _vertexDeclaration;
	//RID _bufferID;
	//RID _indexBuffer;
	//RID _gridBuffer;
	//RID _samplerState;
	//RID _texture;
};