#include "..\..\diesel.h"
#include "math.h"

typedef uint8_t BYTE;

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
struct SpriteBufferVertex {

	v3 position;
	v4 texture;
	v3 size;
	ds::Color color;

	SpriteBufferVertex() : position(0, 0, 0) {}
	SpriteBufferVertex(const v3& p, const v4& t, const ds::Color& c) : position(p), texture(t), color(c) {}
	SpriteBufferVertex(const v2& p, const v4& t, const ds::Color& c) : position(p, 1.0f), texture(t), color(c) {}
	SpriteBufferVertex(const v2& p, const v4& t, const v3& s, const ds::Color& c) : position(p, 1.0f), texture(t), size(s), color(c) {}
};

struct SpriteBufferConstantBuffer {
	v4 screenDimension;
	v4 screenCenter;
	float wvp[16];
};

class SpriteBuffer {

public:
	SpriteBuffer(int maxSprites);
	void begin();
	void add(const v2& position, RID textureID, const v4& rect,const v2& scale = v2(1.0f), float rotation = 0.0f, const ds::Color& clr = ds::Color(1.0f,1.0f,1.0f,1.0f));
	void flush();
private:	
	RID _currentTexture;
	int _max;
	int _current;
	SpriteBufferConstantBuffer _constantBuffer;
	RID _vertexBufferID;
	ds::DrawItem* _item;
	SpriteBufferVertex* _vertices;
};