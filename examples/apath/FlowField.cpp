#include "FlowField.h"
#include <list>

// the directions are:
//  321
//  4x0
//  567
//
const p2i DIRECTIONS[] = { p2i(1,0),p2i(1,1),p2i(0,1),p2i(-1,1),p2i(-1,0),p2i(-1,-1),p2i(0,-1),p2i(1,-1) };

FlowField::FlowField(Grid* grid) : _grid(grid) {
	_fields = new int[_grid->width * _grid->height];
	_dir = new int[_grid->width * _grid->height];
}

FlowField::~FlowField() {
	delete[] _dir;
	delete[] _fields;
}

// -------------------------------------------------------------
// Checks whether the index is already in the list
// -------------------------------------------------------------
bool FlowField::checkIfContains(unsigned int idx, const std::list<unsigned int>& lst) const {
	std::list<unsigned int>::const_iterator it = lst.begin();
	while (it != lst.end()) {
		if (*it == idx) {
			return true;
		}
		++it;
	}
	return false;
}

// -------------------------------------------------------------
// get neighbors (N, S, W and E). Will return the indices
// -------------------------------------------------------------
int FlowField::getNeighbors(int x, int y, int * ret, int max) {
	int cnt = 0;
	if (_grid->isValid(x, y - 1) && _grid->isAvailable(x, y - 1)) {
		ret[cnt++] = x + (y - 1) * _grid->width;
	}
	if (_grid->isValid(x, y + 1) && _grid->isAvailable(x, y + 1)) {
		ret[cnt++] = x + (y + 1) * _grid->width;
	}
	if (_grid->isValid(x - 1, y) && _grid->isAvailable(x - 1, y)) {
		ret[cnt++] = x - 1 + y * _grid->width;
	}
	if (_grid->isValid(x + 1, y) && _grid->isAvailable(x + 1, y)) {
		ret[cnt++] = x + 1 + y * _grid->width;
	}
	return cnt;
}

// -------------------------------------------------------------
// find the index of the neighbor with the lowest cost
// -------------------------------------------------------------
int FlowField::findLowestCost(int x, int y) {
	int m = 255;
	int ret = 14;
	for (int i = 0; i < 8; ++i) {
		p2i c = p2i(x, y) + DIRECTIONS[i];
		if (_grid->isAvailable(c.x, c.y)) {
			int idx = c.x + c.y * _grid->width;
			if (_fields[idx] < m) {
				ret = i;
				m = _fields[idx];
			}
		}
	}
	return ret;
}

// -------------------------------------------------------------
// reset fields and directions
// -------------------------------------------------------------
void FlowField::resetFields() {
	int total = _grid->width * _grid->height;
	for (int i = 0; i < total; ++i) {
		_fields[i] = 255;
		_dir[i] = -1;
	}
}

// -------------------------------------------------------------
// build the flow field
// -------------------------------------------------------------
void FlowField::build(const p2i & end) {
	_end = end;
	// simple Dijstra flood fill first
	unsigned int targetID = end.y * _grid->width + end.x;
	resetFields();
	std::list<unsigned int> openList;
	_fields[targetID] = 0;
	openList.push_back(targetID);
	int neighbors[4];
	while (openList.size() > 0)	{
		unsigned currentID = openList.front();
		openList.pop_front();
		unsigned short currentX = currentID % _grid->width;
		unsigned short currentY = currentID / _grid->width;
		int neighborCount = getNeighbors(currentX, currentY, neighbors, 4);
		for (int i = 0; i < neighborCount; ++i) {             
			unsigned int endNodeCost = _fields[currentID] + 1;
			if (endNodeCost < _fields[neighbors[i]]) {
				if (!checkIfContains(neighbors[i], openList)) {
					openList.push_back(neighbors[i]);
				}
				_fields[neighbors[i]] = endNodeCost;
			}
		}

	}
	// now calculate the directions
	for (int x = 0; x < _grid->width; ++x) {
		for (int y = 0; y < _grid->height; ++y) {
			if (_grid->isAvailable(x, y)) {
				_dir[x + _grid->width * y] = findLowestCost(x, y);
			}
			else {
				_dir[x + _grid->width * y] = 16;
			}
		}
	}

}

// -------------------------------------------------------------
// get direction
// -------------------------------------------------------------
int FlowField::get(int x, int y) const {
	int idx = x + y * _grid->width;
	return _dir[idx];
}

// -------------------------------------------------------------
// get cost of cell
// -------------------------------------------------------------
int FlowField::getCost(int x, int y) const {
	int idx = x + y * _grid->width;
	return _fields[idx];
}

// -------------------------------------------------------------
// get the next field based on the direction of the current cell
// -------------------------------------------------------------
p2i FlowField::next(const p2i & current) {
	int dir = get(current.x, current.y);
	return current + DIRECTIONS[dir];
}

// -------------------------------------------------------------
// check wether there are steps left
// -------------------------------------------------------------
bool FlowField::hasNext(const p2i & current) {
	return current != _end;
}
