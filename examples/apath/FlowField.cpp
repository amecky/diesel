#include "FlowField.h"
#include <list>

const p2i DIRECTIONS[] = { p2i(1,0),p2i(1,1),p2i(0,1),p2i(-1,1),p2i(-1,0),p2i(-1,-1),p2i(0,-1),p2i(1,-1) };

FlowField::FlowField(Grid* grid) : _grid(grid) {
	_fields = new int[_grid->width * _grid->height];
	_dir = new int[_grid->width * _grid->height];
}

FlowField::~FlowField() {
}

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

int FlowField::findLowestCost(int x, int y) {
	int m = 255;
	//int cidx = x + y * _grid->width;
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

void FlowField::resetFields() {
	int total = _grid->width * _grid->height;
	for (int i = 0; i < total; ++i) {
		_fields[i] = 255;
		_dir[i] = -1;
	}
}

void FlowField::build(const p2i & end) {

	unsigned int targetID = end.y * _grid->width + end.x;

	resetFields();//Set total cost in all cells to 65535
	std::list<unsigned int> openList;

	//Set goal node cost to 0 and add it to the open list
	_fields[targetID] = 0;
	openList.push_back(targetID);
	int neighbors[4];

	while (openList.size() > 0)	{
		//Get the next node in the open list
		unsigned currentID = openList.front();
		openList.pop_front();

		unsigned short currentX = currentID % _grid->width;
		unsigned short currentY = currentID / _grid->width;

		//Get the N, E, S, and W neighbors of the current node
		int neighborCount = getNeighbors(currentX, currentY, neighbors, 4);

		//Iterate through the neighbors of the current node
		for (int i = 0; i < neighborCount; ++i) {             
			//Calculate the new cost of the neighbor node             
			// based on the cost of the current node and the weight of the next node             
			unsigned int endNodeCost = _fields[currentID] + 1;// getCostField()->getCostByIndex(neighbors[i]);

																 //If a shorter path has been found, add the node into the open list
			if (endNodeCost < _fields[neighbors[i]]) {
				//Check if the neighbor cell is already in the list.
				//If it is not then add it to the end of the list.
				if (!checkIfContains(neighbors[i], openList)) {
					openList.push_back(neighbors[i]);
				}
				//Set the new cost of the neighbor node.
				_fields[neighbors[i]] = endNodeCost;
			}
		}

	}

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

int FlowField::get(int x, int y) const {
	int idx = x + y * _grid->width;
	return _dir[idx];
}

int FlowField::getCost(int x, int y) const {
	int idx = x + y * _grid->width;
	return _fields[idx];
}

/*
const ds::vec2 & FlowField::get(int x, int y) const {
	int idx = x + y * _grid->width;
	int xv = _fields[idx];
	return ds::vec2(xv, 0);
}
*/