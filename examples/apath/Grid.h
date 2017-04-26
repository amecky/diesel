#pragma once

struct p2i {

	int x;
	int y;
	
	p2i() : x(0), y(0) {}
	p2i(int xp, int yp) : x(xp), y(yp) {}
	
};

inline bool operator ==(const p2i& first, const p2i& other) {
	return first.x == other.x && first.y == other.y;
	
}

inline bool operator !=(const p2i& first, const p2i& other) {
	return !(first == other);	
}

inline p2i operator +(const p2i& lhs, const p2i& rhs) {
	return p2i(lhs.x + rhs.x, lhs.y + rhs.y);
}

const p2i INVALID_POINT = p2i(1, 1);

struct Grid {
	
	int* items;
	int width;
	int height;
	p2i start;
	p2i end;
	
	Grid() : items(0), width(0), height(0), start(-1, -1), end(-1, -1) {}
	
	Grid(int w, int h) : width(w), height(h) {
		int total = width * height;
		items = new int[total];
		for (int i = 0; i < total; ++i) {
			items[i] = 0;
			
		}
		
	}
	
	~Grid() {
		if (items != 0) {
			delete[] items;
			
		}
		
	}
	
	void setStart(int x, int y) {
		setStart(p2i(x, y));
		
	}
	
	void setStart(p2i s) {
		if (isValid(s) && isAvailable(s)) {
			if (s != INVALID_POINT) {
				set(start, 0);
			}
			set(s, 2);
			start = s;
		}
	}
	
	void setEnd(int x, int y) {
		setEnd(p2i(x, y));
	}
	
	void setEnd(p2i e) {
		if (isValid(e) && isAvailable(e)) {
			if (end != INVALID_POINT) {
				set(end, 0);
			}
			set(e, 3);
			end = e;
		}
	}
	
	bool isValid(p2i p) const {
		return isValid(p.x, p.y);
	}
	
	void set(p2i p, int v) {
		set(p.x, p.y, v);
	}
	
	void set(int x, int y, int v) {
		if (isValid(x, y)) {
			int idx = x + y * width;
			items[idx] = v;
		}
	}
	
	bool isValid(int x, int y) const {
		if (x < 0 || y < 0 || x >= width || y >= height) {
			return false;
		}
		return true;
	}
	
	bool isAvailable(p2i p) const {
		return isAvailable(p.x, p.y);
		
	}
	
	bool isAvailable(int x, int y) const {
		if (isValid(x, y)) {
			int idx = x + y * width;
			return items[idx] != 1 && items[idx] < 4;
		}
		return false;
		
	}
	
	int getIndex(p2i p) const {
		return getIndex(p.x, p.y);
	}
	
	int getIndex(int x, int y) const {
		return x + y * width;
	}
	
};
