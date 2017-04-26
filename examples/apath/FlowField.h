#pragma once
#include "Grid.h"
#include "..\..\diesel.h"
#include <list>

class FlowField {

public:
	FlowField(Grid* grid);
	~FlowField();
	void build(const p2i& end);
	int get(int x, int y) const;
	int getCost(int x, int y) const;
	p2i next(const p2i& current);
	bool hasNext(const p2i& current);
private:
	bool checkIfContains(unsigned int idx, const std::list<unsigned int>& lst) const;
	int getNeighbors(int x, int y, int* ret, int max);
	int findLowestCost(int x, int y);
	void resetFields();
	int* _fields;
	int* _dir;
	Grid* _grid;
	p2i _end;
};

