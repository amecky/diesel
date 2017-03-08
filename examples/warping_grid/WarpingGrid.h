#pragma once
#include "..\..\diesel.h"
#include <vector>

struct GridVertex {
	v3 position;
	v2 texture;
	ds::Color color;

	GridVertex() : position(0.0f), texture(0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	GridVertex(const v3& p, const v2& t, const ds::Color& c) : position(p), texture(t), color(c) {}
};

//const int GRID_DIM_X = 41;
//const int GRID_DIM_Y = 22;
//const float GRID_SIZE = 30.0f;

struct GridPoint {
	v2 pos;
	v2 old_pos;
	v2 velocity;
	float invMass;
	float damping;
	v2 acceleration;
	bool movable;
	float timer;
	ds::Color color;
	bool fading;
};

struct Spring {
	v2 start;
	v2 end;
	float targetLength;
	float stiffness;
	float damping;

};

struct WarpingGridData {
	int width;
	int height;
	float cellSize;
};

class WarpingGrid {

public:
	WarpingGrid();
	~WarpingGrid();
	void createGrid(const WarpingGridData& data);
	void tick(float dt);
	int mapData(GridVertex* vertices,int maxVertices);
	void applyForce(v2 p,const v2& f);
	void applyForce(const v2& pos, float force, float radius);
	void applyForce(const v2& pos, float force, float innerRadius, float outerRadius);
private:
	void addSpring(v2 start, v2 end, float stiffness, float damping);
	const GridPoint& get(const v2& gp) const;
	GridPoint& get(const v2& gp);

	int _width;
	int _height;
	GridPoint* _grid;// [GRID_DIM_X][GRID_DIM_Y];
	std::vector<Spring> _springs;
};

