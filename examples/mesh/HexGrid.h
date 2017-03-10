#pragma once
#include "hex.h"
#include <vector>

namespace ds {

	template<class T>
	class HexGrid {

	public:
		HexGrid() : _qMax(0), _rMax(0), _items(0), _layout(layout_pointy, ds::vec2(1.0f, 1.0f), ds::vec2(1, 1)) {
		}

		~HexGrid() {
			if (_items != 0) {
				delete[] _items;
			}
		}

		int qMax() const {
			return _qMax;
		}

		int rMax() const {
			return _rMax;
		}

		void setLayout(const Layout& l) {
			_layout = l;
		}
		// -------------------------------------------------------
		// resize
		// -------------------------------------------------------
		void resize(int qMax, int rMax) {
			if (_items != 0) {
				delete[] _items;
			}
			_qMax = qMax;
			_rMax = rMax;
			_items = new T[qMax * rMax * sizeof(T)];
		}
		
		// -------------------------------------------------------
		// get
		// -------------------------------------------------------
		const T& get(const Hex& hex) const {
			int q_offset = hex.r >> 1;
			int idx = (hex.q + q_offset) + hex.r * _qMax;
			return _items[idx];
		}

		// -------------------------------------------------------
		// get by hex
		// -------------------------------------------------------
		T& get(const Hex& hex) {
			int q_offset = hex.r >> 1;
			int idx = (hex.q + q_offset) + hex.r * _qMax;
			return _items[idx];
		}

		// -------------------------------------------------------
		// neighbors
		// -------------------------------------------------------
		int neighbors(const Hex& hex, Hex* ret) {
			int cnt = 0;
			for (int i = 0; i < 6; ++i) {
				Hex n = hex_math::neighbor(hex, i);
				if (isValid(n)) {
					ret[cnt++] = n;
				}
			}
			return cnt;
		}

		ds::vec2 to_screen(const Hex& h) {
			return hex_math::hex_to_pixel(_layout, h);
		}
		// -------------------------------------------------------
		// convert from mouse pos
		// -------------------------------------------------------
		Hex convertFromMousePos() {
			ds::vec2 mp = renderer::getMousePosition();
			return hex_math::hex_round(hex_math::pixel_to_hex(_layout, mp));
		}
		// -------------------------------------------------------
		// get by index
		// -------------------------------------------------------
		const T& get(int index) const {
			return _items[index];
		}

		// -------------------------------------------------------
		// get by index
		// -------------------------------------------------------
		T& get(int index) {
			return _items[index];
		}

		// -------------------------------------------------------
		// size
		// -------------------------------------------------------
		const int size() const {
			return _rMax * _qMax;
		}

		// -------------------------------------------------------
		// select
		// -------------------------------------------------------
		int select(int x, int y) {
			ds::vec2 mp = renderer::getMousePosition();
			Hex h = hex_math::hex_round(hex_math::pixel_to_hex(_layout, mp));
			int q_offset = h.r >> 1;
			int selected = -1;
			if (!isValid(h)) {
				return -1;
			}
			for (size_t i = 0; i < size(); ++i) {
				if (_items[i].hex == h) {
					selected = i;

				}
			}
			return selected;
		}

		// -------------------------------------------------------
		// is valid
		// -------------------------------------------------------
		bool isValid(int q, int r) const {
			int q_offset = r >> 1;
			if (r < 0 || r >= _rMax) {
				return false;
			}
			if (q_offset + q < 0 || q_offset + q >= _qMax) {
				return false;
			}
			return true;
		}

		// -------------------------------------------------------
		// is valid
		// -------------------------------------------------------
		bool isValid(const Hex& hex) const {
			return isValid(hex.q, hex.r);
		}

		// -------------------------------------------------------
		// set origin
		// -------------------------------------------------------
		void setOrigin(const ds::vec2& origin) {
			_layout.origin = origin;
		}

		// -------------------------------------------------------
		// get index
		// -------------------------------------------------------
		int getIndex(const Hex& hex) const {
			int q_offset = hex.r >> 1;
			return (hex.q + q_offset) + hex.r * _qMax;
		}

		// -------------------------------------------------------
		// swap colors
		// -------------------------------------------------------
		void swap(int firstIndex, int secondIndex) {
			T first = _items[firstIndex];
			_items[firstIndex].color = _items[secondIndex].color;
			_items[secondIndex].color = first.color;
		}

		// -------------------------------------------------------
		// find connected items
		// -------------------------------------------------------
		void findConnectedItems(const Hex& h, std::vector<Hex>& list) {
			if (isValid(h)) {
				Hex n[6];
				int cnt = neighbors(h, n);
				T& current = get(h);
				for (int i = 0; i < cnt; ++i) {
					T& item = get(n[i]);
					if (current.matches(item)) {
						bool found = false;
						for (size_t j = 0; j < list.size(); ++j) {
							if (list[j] == n[i]) {
								found = true;
							}
						}
						if (!found) {
							list.push_back(n[i]);
							findConnectedItems(n[i], list);
						}
					}
				}
			}
		}
	private:
		int _qMax;
		int _rMax;
		T* _items;
		Layout _layout;
	};

}