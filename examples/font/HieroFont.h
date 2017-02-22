#pragma once

// char id=10    x=0    y=0    width=0    height=0    xoffset=0    yoffset=0    xadvance=0    page=0    chnl=0 
struct HieroFontItem {
	int id;
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;

	HieroFontItem() : id(-1), x(0), y(0), width(0), height(0), xoffset(0), yoffset(0), xadvance(0) {}
};

class HieroFont {

public:
	bool load(const char* fileName);
	const HieroFontItem& get(char c) const {
		return _items[c];
	}
private:
	int extractInt(const char* txt);
	HieroFontItem _items[256];
};
