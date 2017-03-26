#include "APath.h"
#include <stdio.h>

void APath::print(const WayPoint& wp) {
	printf("x: %d y: %d f: %g g: %g h: %g parent: %d\n", wp.p.x, wp.p.y, wp.f, wp.g, wp.h, wp.parent);
}

void APath::print(int index) {
	const WayPoint& wp = _internalGrid[index];
	printf("x: %d y: %d f: %g g: %g h: %g parent: %d\n", wp.p.x, wp.p.y, wp.f, wp.g, wp.h, wp.parent);
}

// http://www.policyalmanac.org/games/aStarTutorial_de.html
APath::APath(Grid* grid) : _grid(grid) {
	_open = new int[grid->width * grid->height];
	_closed = new int[grid->width * grid->height];
	_numOpen = 0;
	_numClosed = 0;
	_internalGrid = new WayPoint[grid->width * grid->height];
	for (int x = 0; x < grid->width; ++x) {
		for (int y = 0; y < grid->height; ++y) {
			int idx = x + y * grid->width;
			_internalGrid[idx].index = idx;
			_internalGrid[idx].p = p2i(x, y);
			_internalGrid[idx].parent = -1;
			if ( !grid->isAvailable(x,y)) {
				_internalGrid[idx].available = false;
			}
		}
	}
	_width = grid->width;
	_height = grid->height;
}

APath::~APath() {
	delete[] _internalGrid;
	delete[] _open;
	delete[] _closed;
}

// ----------------------------------------------------------
// add
// ----------------------------------------------------------
float APath::add(p2i p, float g, int parent) {
	int idx = p.x + p.y * _width;
	if (idx >= 0) {
		WayPoint& wp = _internalGrid[idx];
		wp.p = p;
		int ex = _end.x - p.x;
		if (ex < 0) {
			ex *= -1;
		}
		int ey = _end.y - p.y;
		if (ey < 0) {
			ey *= -1;
		}
		wp.g = g;
		if (parent != -1) {
			wp.g += _internalGrid[parent].g;
		}
		wp.h = (ex + ey) * 10;
		wp.f = wp.g + wp.h;
		wp.parent = parent;
		//printf("=> ");
		//print(wp);
		_open[_numOpen++] = idx;
		return wp.f;
	}
	return 1000.0f;
}

// ----------------------------------------------------------
// find cheapest
// ----------------------------------------------------------
int APath::findCheapest() {
	float current = 1000.0f;
	int idx = -1;
	for (int i = 0; i < _numOpen; ++i) {
		int cidx = _open[i];
		float f = _internalGrid[cidx].f;
		if (f < current) {
			idx = cidx;
			current = _internalGrid[_open[i]].f;
		}
	}
	return idx;
}

float APath::calculateH(p2i p) {
	int ex = _end.x - p.x;
	if (ex < 0) {
		ex *= -1;
	}
	int ey = _end.y - p.y;
	if (ey < 0) {
		ey *= -1;
	}
	return (ex + ey) * 10;
}

float APath::calculateG(int firstIndex, int secondIndex) {
	p2i first = _internalGrid[firstIndex].p;
	p2i second = _internalGrid[secondIndex].p;
	return calculateG(first, second);
}

float APath::calculateG(p2i first, p2i second) {
	int dx = second.x - first.x;
	int dy = second.y - first.y;
	if (dx != 0 && dy != 0) {
		return 14.0f;
	}
	else {
		return 10.0f;
	}
}

// ----------------------------------------------------------
// get index of neighbours
// ----------------------------------------------------------
/*
int APath::getNeighbours(p2i p, int* result, int max) {
p2i dirs[] = { p2i(-1,0),p2i(1,0),p2i(0,-1),p2i(0,1) };
int num = 0;
for (int i = 0; i < 4; ++i) {
p2i current = p2i(p.x + dirs[i].x, p.y + dirs[i].y);
int cidx = current.x + current.y * _width;
if (cidx >= 0) {
const WayPoint& wp = _internalGrid[cidx];
if (wp.available && num < max) {
if (current.x != p.x || current.y != p.y) {
result[num++] = cidx;
}
}
}
}
return num;
}
*/
int APath::getNeighbours(p2i p, int* result, int max) {
	int num = 0;
	int sx = p.x;
	int sy = p.y;
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			p2i current = p2i(p.x + x, p.y + y);
			if (_grid->isAvailable(sx + x, sy + y) && num < max) {
				if (current != p) {
					result[num++] = _grid->getIndex(current);					
				}
			}
		}
	}
	return num;
}

// ----------------------------------------------------------
// close item
// ----------------------------------------------------------
void APath::close(p2i p) {
	int current = p.x + p.y * _width;
	//printf("x> ");
	//print(current);
	_closed[_numClosed++] = current;
	removeWaypoint(p);
}

// ----------------------------------------------------------
// check if open
// ----------------------------------------------------------
bool APath::isOpen(int index) {
	for (int i = 0; i < _numOpen; ++i) {
		if (_open[i] == index) {
			return true;
		}
	}
	return false;
}

int APath::find(p2i p) {
	int idx = p.x + p.y * _width;
	for (int i = 0; i < _numOpen; ++i) {
		int current = _open[i];
		if (current == idx) {
			return i;
		}
	}
	return -1;
}

void APath::removeWaypoint(p2i p) {
	int idx = getIndex(p);
	for (int i = 0; i < _numOpen; ++i) {
		if (_open[i] == idx) {
			if (_numOpen > 0) {
				_open[i] = _open[_numOpen - 1];
			}
			--_numOpen;
		}
	}
}

// ----------------------------------------------------------
// check if item is in open or closed list
// ----------------------------------------------------------
bool APath::visited(int current) {
	for (int i = 0; i < _numOpen; ++i) {
		if (_open[i] == current) {
			return true;
		}
	}
	for (int i = 0; i < _numClosed; ++i) {
		if (_closed[i] == current) {
			return true;
		}
	}
	return false;
}

int APath::getIndex(p2i p) {
	return p.x + p.y * _width;
}

// ----------------------------------------------------------
// move next
// ----------------------------------------------------------
int APath::moveNext(p2i p) {
	close(p);
	int tmp[8];
	float far[8];
	int cidx = getIndex(p);
	int ret = -1;
	float gm = 1000.0f;
	float cg = _internalGrid[cidx].g;
	int num = getNeighbours(p, tmp, 8);
	for (int i = 0; i < num; ++i) {
		int tidx = tmp[i];
		if (!visited(tidx) && _grid->isAvailable(p)) {
			//printf("=> ");
			//print(tidx);
			float g = calculateG(tidx, cidx);
			far[i] = add(_internalGrid[tidx].p, g, cidx);
		}
		else {
			if (isOpen(tidx)) {
				//printf(":> ");
				//print(tidx);
				float g = calculateG(_internalGrid[tidx].p, p) + cg;
				float h = calculateH(_internalGrid[tidx].p);
				far[i] = g + h;
			}
			else {
				far[i] = 1000.0f;
			}
		}
	}

	for (int i = 0; i < num; ++i) {
		if (far[i] < gm) {
			ret = tmp[i];
			gm = far[i];
		}
	}
	if (ret == -1) {
		return findCheapest();
	}
	return ret;
}

// ----------------------------------------------------------
// find
// ----------------------------------------------------------
int APath::find(p2i start, p2i end, p2i* points, int max) {
	_numOpen = 0;
	_numClosed = 0;
	_start = start;
	_end = end;
	add(_start, 0.0f);
	p2i c = _start;
	int cnt = 0;
	bool running = true;
	int endIdx = getIndex(end);
	while (running) {
		//for ( int i = 0; i < 4; ++i) {
		int idx = moveNext(c);
		if (idx == -1) {
			running = false;
		}
		else {
			WayPoint ch = _internalGrid[idx];
			c = ch.p;
			//printf("idx: %d p: %d %d\n", idx, ch.p.x, ch.p.y);
		}
		++cnt;
		if (cnt > 30) {
			running = false;
		}
		if (idx == endIdx) {
			close(_end);
			running = false;
		}
	}
	//for (int i = 0; i < _numWaypoints; ++i) {
	//printf("%d = %d %d g: %g h: %g f: %g\n", i, _waypoints[i].p.x, _waypoints[i].p.y, _waypoints[i].g, _waypoints[i].h, _waypoints[i].f);
	//}
	//printf("-------------------------------\n");
	//printf("iterations: %d\n", cnt);
	//for (int i = 0; i < _numClosed; ++i) {
		//printf("%d = %d %d\n", i, _internalGrid[_closed[i]].p.x, _internalGrid[_closed[i]].p.y);
	//}
	int ret = 0;
	const WayPoint& last = _internalGrid[_closed[_numClosed - 1]];
	points[ret++] = last.p;
	//printf("#> ");
	//print(last);
	int parent = _internalGrid[_closed[_numClosed - 1]].parent;
	//for (int i = 0; i < _numClosed; ++i) {
	while (parent != -1) {
		//printf("%d %d\n", _internalGrid[parent].p.x, _internalGrid[parent].p.y);
		points[ret++] = _internalGrid[parent].p;
		parent = _internalGrid[parent].parent;
	}
	return ret;
	//print();
}

void APath::print() {
	FILE* fp = fopen("test.log", "w");
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			int idx = x + y * _width;
			WayPoint wp = _internalGrid[idx];
			if (wp.available) {
				fprintf(fp, "%2.0f/%2.0f/%2.0f/%2d  ", wp.f, wp.g, wp.h, wp.parent);
			}
			else {
				fprintf(fp, "xx/xx/xx/xx  ");
			}
		}
		fprintf(fp, "\n");
	}
}

