#pragma once
#include "..\..\diesel.h"
#include <vector>

struct GridVertex {
	ds::vec3 position;
	ds::vec2 texture;
	ds::Color color;

	GridVertex() : position(0.0f), texture(0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	GridVertex(const ds::vec3& p, const ds::vec2& t, const ds::Color& c) : position(p), texture(t), color(c) {}
};

//const int GRID_DIM_X = 41;
//const int GRID_DIM_Y = 22;
//const float GRID_SIZE = 30.0f;

struct GridPoint {
	ds::vec2 pos;
	ds::vec2 old_pos;
	ds::vec2 velocity;
	float invMass;
	float damping;
	ds::vec2 acceleration;
	bool movable;
	float timer;
	ds::Color color;
	bool fading;
};

struct Spring {
	ds::vec2 start;
	ds::vec2 end;
	float targetLength;
	float stiffness;
	float damping;

};

struct WarpingGridData {
	int width;
	int height;
	float cellSize;
	ds::Color regularColor;
	ds::Color flashColor;
	float flashTTL;
};

class WarpingGrid {

public:
	WarpingGrid();
	~WarpingGrid();
	void createGrid(const WarpingGridData& data);
	void tick(float dt);
	int mapData(GridVertex* vertices,int maxVertices);
	void applyForce(int x,int y,const ds::vec2& f);
	void applyForce(const ds::vec2& pos, float force, float radius);
	void applyNegativeForce(const ds::vec2& pos, float force, float radius);
	void applyForce(const ds::vec2& pos, float force, float innerRadius, float outerRadius);
	void applyNegativeForce(const ds::vec2& pos, float force, float innerRadius, float outerRadius);
private:
	void addSpring(ds::vec2 start, ds::vec2 end, float stiffness, float damping);
	const GridPoint& get(const ds::vec2& gp) const;
	GridPoint& get(const ds::vec2& gp);

	WarpingGridData _data;
	int _width;
	int _height;
	GridPoint* _grid;// [GRID_DIM_X][GRID_DIM_Y];
	std::vector<Spring> _springs;
};

