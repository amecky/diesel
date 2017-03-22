#include "..\..\diesel.h"

typedef uint8_t BYTE;

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
struct SpriteBufferVertex {

	ds::vec3 position;
	ds::vec4 texture;
	ds::vec3 size;
	ds::Color color;

	SpriteBufferVertex() : position(0, 0, 0) {}
	SpriteBufferVertex(const ds::vec3& p, const ds::vec4& t, const ds::Color& c) : position(p), texture(t), color(c) {}
	SpriteBufferVertex(const ds::vec2& p, const ds::vec4& t, const ds::Color& c) : position(p, 1.0f), texture(t), color(c) {}
	SpriteBufferVertex(const ds::vec2& p, const ds::vec4& t, const ds::vec3& s, const ds::Color& c) : position(p, 1.0f), texture(t), size(s), color(c) {}
};

struct SpriteBufferConstantBuffer {
	ds::vec4 screenDimension;
	ds::matrix wvp;
};

class SpriteBuffer {

public:
	SpriteBuffer(int maxSprites, RID textureID);
	void begin();
	void add(const ds::vec2& position, const ds::vec4& rect,const ds::vec2& scale = ds::vec2(1.0f), float rotation = 0.0f, const ds::Color& clr = ds::Color(1.0f,1.0f,1.0f,1.0f));
	void flush();
private:	
	//RID _currentTexture;
	int _max;
	int _current;
	SpriteBufferConstantBuffer _constantBuffer;
	RID _vertexBufferID;
	RID _item;
	RID _pixelShader;
	RID _textureID;
	RID _orthoPass;
	SpriteBufferVertex* _vertices;
	//ds::matrix _viewprojectionMatrix;
};