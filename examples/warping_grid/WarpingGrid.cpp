#include "WarpingGrid.h"

const float TIME_STEPSIZE2 = 0.95f * 0.95f;
const v3 DOT_POINTS[] = { v3(-2, 2, 0), v3(2, 2, 0), v3(2, -2, 0), v3(-2, -2, 0) };

WarpingGrid::WarpingGrid() : _grid(0) {
}

WarpingGrid::~WarpingGrid() {
	if (_grid != 0) {
		delete[] _grid;
	}
}

// -------------------------------------------------------
// create grid
// -------------------------------------------------------
void WarpingGrid::createGrid(const WarpingGridData& data) {
	_width = data.width;
	_height = data.height;
	_grid = new GridPoint[_width * _height];
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			GridPoint& gp = _grid[x + y * _width];
			gp.acceleration = v2(0.0f);
			gp.damping = 0.98f;
			gp.invMass = 1.0f;
			gp.movable = true;
			if (x == 0 || y == 0 || x == (_width - 1) || y == (_height - 1)) {
				gp.invMass = 0.0f;
				gp.movable = false;
			}
			gp.velocity = v2(0.0f);
			gp.pos = v2(2.0f + x * data.cellSize, 50.0f + y * data.cellSize);
			gp.old_pos = gp.pos;
			gp.color = ds::Color(1.0f, 0.0f, 0.0f, 1.0f);
			gp.fading = false;
			gp.timer = 0.0f;
		}
	}

	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			const float stiffness = 0.28f;
			const float damping = 0.06f;
			if (x > 0)
				addSpring(v2(x-1,y), v2(x,y), stiffness, damping);
			if (y > 0)
				addSpring(v2(x, y - 1), v2(x, y), stiffness, damping);
		}
	}

}

const GridPoint& WarpingGrid::get(const v2& gp) const {
	int gx = static_cast<int>(gp.x);
	int gy = static_cast<int>(gp.y);
	return _grid[gx + gy * _width];
}

GridPoint& WarpingGrid::get(const v2& gp) {
	int gx = static_cast<int>(gp.x);
	int gy = static_cast<int>(gp.y);
	return _grid[gx + gy * _width];
}
// -------------------------------------------------------
// add spring
// -------------------------------------------------------
void WarpingGrid::addSpring(v2 start, v2 end, float stiffness, float damping) {
	Spring spring;
	spring.start = start;
	spring.end = end;
	spring.stiffness = stiffness;
	spring.damping = damping;
	const GridPoint& sp = get(start);
	const GridPoint& ep = get(end);
	spring.targetLength = distance(sp.pos, ep.pos);// *0.95f;
	_springs.push_back(spring);
}

// -------------------------------------------------------
// tick
// -------------------------------------------------------
void WarpingGrid::tick(float dt) {	
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			GridPoint& gp = _grid[x + y * _width];
			if (gp.movable) {
				v2 temp = gp.pos;
				gp.pos = gp.pos + (gp.pos - gp.old_pos) * gp.damping + gp.acceleration * TIME_STEPSIZE2;
				gp.old_pos = temp;
				gp.acceleration = v2(0.0f);
			}
			/*
			if (gp.fading) {
				gp.timer += dt;
				gp.color = ds::color::lerp(_settings->grid.flash, _settings->grid.regular, gp.timer/_settings->grid.ttl);
				if (gp.timer >= _settings->grid.ttl) {
					gp.fading = false;
					gp.color = _settings->grid.regular;
				}
			}
			*/
		}
	}

	for (uint32_t i = 0; i < _springs.size(); ++i) {
		Spring& spring = _springs[i];
		GridPoint& sp = get(spring.start);
		GridPoint& ep = get(spring.end);
		v2 delta = ep.pos - sp.pos; 
		float current_distance = length(delta); 
		v2 x = sp.pos - ep.pos;
		float l = length(x);
		if (l > spring.targetLength) {
			x = (x / l) * (l - spring.targetLength);
			v2 dv = ep.velocity - sp.velocity;
			v2 force = spring.stiffness * x - dv * spring.damping;
			sp.acceleration -= force;
			ep.acceleration += force;
		}
	}
}

// -------------------------------------------------------
// apply force
// -------------------------------------------------------
void WarpingGrid::applyForce(const v2& pos, float force, float radius) {
	float sr = radius * radius;
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			GridPoint& gp = _grid[x + y * _width];
			if (gp.movable) {
				v2 d = gp.pos - pos;
				if (sqr_length(d) < sr) {
					applyForce(v2(x, y), d * force);
				}
			}
		}
	}
}

// -------------------------------------------------------
// apply force
// -------------------------------------------------------
void WarpingGrid::applyForce(const v2& pos, float force, float innerRadius, float outerRadius) {
	float isr = innerRadius * innerRadius;
	float osr = outerRadius * outerRadius;
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			GridPoint& gp = _grid[x + y * _width];
			if (gp.movable) {
				v2 d = gp.pos - pos;
				if (sqr_length(d) > isr && sqr_length(d) < osr) {
					applyForce(v2(x, y), d * force);
					gp.color = ds::Color(128, 0, 0, 255);
					gp.fading = true;
					gp.timer = 0.2f;
				}
			}
		}
	}
}

// -------------------------------------------------------
// applay force
// -------------------------------------------------------
void WarpingGrid::applyForce(v2 p, const v2& f) {
	GridPoint& gp = get(p);
	gp.acceleration += f * gp.invMass;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
int WarpingGrid::mapData(GridVertex* vertices,int maxVertices) {
	int cnt = 0;
	for (int y = 1; y < _height; ++y) {
		for (int x = 1; x < _width; ++x) {
			const GridPoint& gp = _grid[x + y * _width];
			if ((cnt + 4) <= maxVertices) {
				vertices[cnt++] = { _grid[x - 1 + (y - 1) * _width].pos,v2(0.0f,1.0f),gp.color };
				vertices[cnt++] = { _grid[x - 1 + y * _width].pos,v2(0.0f,0.0f),gp.color };
				vertices[cnt++] = { _grid[x + y * _width].pos,v2(1.0f,0.0f),gp.color };
				vertices[cnt++] = { _grid[x + (y - 1) * _width].pos,v2(1.0f,1.0f),gp.color };
			}
		}
	}
	return cnt;
}