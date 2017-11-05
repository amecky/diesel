#include "hex.h"

bool operator == (Hex a, Hex b) {
	return a.q == b.q && a.r == b.r && a.s == b.s;
}

bool operator != (Hex a, Hex b) {
	return !(a == b);
}

const Hex HEX_DIRECTIONS[] = {
	Hex(1, 0), Hex(1, -1), Hex(0, -1), Hex(-1, 0), Hex(-1, 1), Hex(0, 1)
};

namespace hex_math {

	ds::vec2 to_pixel(const Hex& hex, float size) {
		float x = size * 3.0f / 2.0f * hex.q;
		float y = size * sqrt(3.0f) * (hex.r + hex.q / 2.0f);
		return ds::vec2(x, y);
	}

	ds::vec2 hex_to_pixel(const Layout& layout, const Hex& h) {
		const Orientation& M = layout.orientation;
		float x = (M.f0 * h.q + M.f1 * h.r) * layout.size.x;
		float y = (M.f2 * h.q + M.f3 * h.r) * layout.size.y;
		return ds::vec2(x + layout.origin.x, y + layout.origin.y);
	}

	FractionalHex pixel_to_hex(const Layout& layout, const ds::vec2& p) {
		const Orientation& M = layout.orientation;
		ds::vec2 pt = ds::vec2((p.x - layout.origin.x) / layout.size.x, (p.y - layout.origin.y) / layout.size.y);
		float q = M.b0 * pt.x + M.b1 * pt.y;
		float r = M.b2 * pt.x + M.b3 * pt.y;
		return FractionalHex(q, r, -q - r);
	}

	Hex hex_round(const FractionalHex& h) {
		int q = int(round(h.q));
		int r = int(round(h.r));
		int s = int(round(h.s));
		double q_diff = abs(q - h.q);
		double r_diff = abs(r - h.r);
		double s_diff = abs(s - h.s);
		if (q_diff > r_diff && q_diff > s_diff) {
			q = -r - s;
		}
		else if (r_diff > s_diff) {
			r = -q - s;
		}
		else {
			s = -q - r;
		}
		return Hex(q, r, s);
	}

	Hex neighbor(const Hex& hex, int direction) {
		Hex dir = HEX_DIRECTIONS[direction];
		return Hex(hex.q + dir.q, hex.r + dir.r);
	}
}