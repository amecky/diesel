#include "WarpingGrid.h"

const float TIME_STEPSIZE2 = 0.95f * 0.95f;

ds::Color lerp(const ds::Color& lhs, const ds::Color& rhs, float t) {
	if (t <= 0.0f) {
		return lhs;
	}
	if (t >= 1.0f) {
		return rhs;
	}
	float invT = 1.0f - t;
	return ds::Color(lhs.r * invT + rhs.r * t, lhs.g * invT + rhs.g * t, lhs.b * invT + rhs.b * t, lhs.a * invT + rhs.a * t);
}

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
	_data = data;
	_width = data.width;
	_height = data.height;
	_grid = new GridPoint[_width * _height];
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			GridPoint& gp = _grid[x + y * _width];
			gp.acceleration = ds::vec2(0.0f);
			gp.damping = 0.98f;
			gp.invMass = 1.0f;
			gp.movable = true;
			if (x == 0 || y == 0 || x == (_width - 1) || y == (_height - 1)) {
				gp.invMass = 0.0f;
				gp.movable = false;
			}
			gp.velocity = ds::vec2(0.0f);
			gp.pos = ds::vec2(2.0f + x * data.cellSize, 50.0f + y * data.cellSize);
			gp.old_pos = gp.pos;
			gp.color = data.regularColor;
			gp.fading = false;
			gp.timer = 0.0f;
		}
	}

	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			const float stiffness = 0.28f;
			const float damping = 0.06f;
			if (x > 0)
				addSpring(ds::vec2(x-1,y), ds::vec2(x,y), stiffness, damping);
			if (y > 0)
				addSpring(ds::vec2(x, y - 1), ds::vec2(x, y), stiffness, damping);
		}
	}

}

const GridPoint& WarpingGrid::get(const ds::vec2& gp) const {
	int gx = static_cast<int>(gp.x);
	int gy = static_cast<int>(gp.y);
	return _grid[gx + gy * _width];
}

GridPoint& WarpingGrid::get(const ds::vec2& gp) {
	int gx = static_cast<int>(gp.x);
	int gy = static_cast<int>(gp.y);
	return _grid[gx + gy * _width];
}
// -------------------------------------------------------
// add spring
// -------------------------------------------------------
void WarpingGrid::addSpring(ds::vec2 start, ds::vec2 end, float stiffness, float damping) {
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
				ds::vec2 temp = gp.pos;
				gp.pos = gp.pos + (gp.pos - gp.old_pos) * gp.damping + gp.acceleration * TIME_STEPSIZE2;
				gp.old_pos = temp;
				gp.acceleration = ds::vec2(0.0f);
			}
			
			if (gp.fading) {
				gp.timer += dt;
				gp.color = lerp(_data.flashColor, _data.regularColor, gp.timer / _data.flashTTL);
				if (gp.timer >= _data.flashTTL) {
					gp.fading = false;
					gp.color = _data.regularColor;
				}
			}
			
		}
	}

	for (uint32_t i = 0; i < _springs.size(); ++i) {
		Spring& spring = _springs[i];
		GridPoint& sp = get(spring.start);
		GridPoint& ep = get(spring.end);
		ds::vec2 delta = ep.pos - sp.pos; 
		float current_distance = length(delta); 
		ds::vec2 x = sp.pos - ep.pos;
		float l = length(x);
		if (l > spring.targetLength) {
			x = (x / l) * (l - spring.targetLength);
			ds::vec2 dv = ep.velocity - sp.velocity;
			ds::vec2 force = spring.stiffness * x - dv * spring.damping;
			sp.acceleration -= force;
			ep.acceleration += force;
		}
	}
}

// -------------------------------------------------------
// apply force
// -------------------------------------------------------
void WarpingGrid::applyForce(const ds::vec2& pos, float force, float radius) {
	float sr = radius * radius;
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			GridPoint& gp = _grid[x + y * _width];
			if (gp.movable) {
				ds::vec2 d = gp.pos - pos;
				if (sqr_length(d) < sr) {
					applyForce(x, y, d * force);
				}
			}
		}
	}
}

// -------------------------------------------------------
// apply force
// -------------------------------------------------------
void WarpingGrid::applyNegativeForce(const ds::vec2& pos, float force, float radius) {
	float sr = radius * radius;
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			GridPoint& gp = _grid[x + y * _width];
			if (gp.movable) {
				ds::vec2 d = pos - gp.pos;
				if (sqr_length(d) < sr) {
					applyForce(x, y, d * force);
				}
			}
		}
	}
}


// -------------------------------------------------------
// apply force
// -------------------------------------------------------
void WarpingGrid::applyForce(const ds::vec2& pos, float force, float innerRadius, float outerRadius) {
	float isr = innerRadius * innerRadius;
	float osr = outerRadius * outerRadius;
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			GridPoint& gp = _grid[x + y * _width];
			if (gp.movable) {
				ds::vec2 d = gp.pos - pos;
				if (sqr_length(d) > isr && sqr_length(d) < osr) {
					applyForce(x, y, d * force);
					gp.color = ds::Color(128, 0, 0, 255);
					gp.fading = true;
					gp.timer = 0.2f;
				}
			}
		}
	}
}

// -------------------------------------------------------
// apply negative force using inner and outer radius
// -------------------------------------------------------
void WarpingGrid::applyNegativeForce(const ds::vec2& pos, float force, float innerRadius, float outerRadius) {
	float isr = innerRadius * innerRadius;
	float osr = outerRadius * outerRadius;
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			GridPoint& gp = _grid[x + y * _width];
			if (gp.movable) {
				ds::vec2 d = pos - gp.pos;
				if (sqr_length(d) > isr && sqr_length(d) < osr) {
					applyForce(x, y, d * force);
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
void WarpingGrid::applyForce(int x, int y, const ds::vec2& f) {
	GridPoint& gp = _grid[x + y * _width];
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
				vertices[cnt++] = { _grid[x - 1 + (y - 1) * _width].pos,ds::vec2(0.0f,1.0f),gp.color };
				vertices[cnt++] = { _grid[x - 1 + y * _width].pos,ds::vec2(0.0f,0.0f),gp.color };
				vertices[cnt++] = { _grid[x + y * _width].pos,ds::vec2(1.0f,0.0f),gp.color };
				vertices[cnt++] = { _grid[x + (y - 1) * _width].pos,ds::vec2(1.0f,1.0f),gp.color };
			}
		}
	}
	return cnt;
}