#include "..\..\diesel.h"

struct GridVertex {

	ds::vec3 p;
	ds::vec2 uv;

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
	Grid(ds::Camera* camera) : _camera(camera) {}
	~Grid() {}
	void create(ds::vec3* positions, int numCells, RID vertexShader, RID pixelShader, RID textureID, RID renderPass);
	void render();
private:
	ds::Camera* _camera;
	GridVertex _vertices[4];
	GridConstantBuffer _constantBuffer;
	RID _drawItem;
	RID _renderPass;
};