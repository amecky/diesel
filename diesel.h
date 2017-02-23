#pragma once
#include <stdint.h>
//#define MATH_IMPLEMENTATION
//#include "math.h"

// ----------------------------------------------------
// resource handle
// first 16 bit is the index
// second 16 bits define the reosurce type
// ----------------------------------------------------
typedef uint32_t RID;

const uint16_t INVALID_RID = UINT16_MAX;
const uint16_t NO_RID = UINT16_MAX - 1;

#ifndef DEGTORAD
#define DEGTORAD( degree ) ((degree) * (3.141592654f / 180.0f))
#endif
#ifndef RADTODEG
#define RADTODEG( radian ) ((radian) * (180.0f / 3.141592654f))
#endif

// ----------------------------------------------------------------------
//
// The necessary math
//
// ----------------------------------------------------------------------

// ------------------------------------------------
// Vector template
// ------------------------------------------------
template<int Size, class T>
struct Vector {

	typedef T Type;

	T data[Size];

};

// ------------------------------------------------
// Vector 2
// ------------------------------------------------
template <class T>
struct Vector<2, T> {
	union {
		T data[2];
		struct {
			T x, y;
		};
	};
	Vector<2, T>() : x(0), y(0) {}
	explicit Vector<2, T>(T t) : x(t), y(t) {}
	Vector<2, T>(T xv, T yv) : x(xv), y(yv) {}
	Vector<2, T>(const Vector<2, int>& other) : x(other.x), y(other.y) {}
	Vector<2, T>(const T* value) {
		x = *value;
		++value;
		y = *value;
	}
	const T& operator[] (int idx) const { return data[idx]; }
	T& operator[] (int idx) { return data[idx]; }
	T* operator() () {
		return &data[0];
	}
	Vector<2, T>& operator = (const Vector<2, T>& other) {
		x = other.x;
		y = other.y;
		return *this;
	}
};

// ------------------------------------------------
// Vector 3
// ------------------------------------------------
template <class T>
struct Vector<3, T> {
	union {
		T data[3];
		struct {
			T x, y, z;
		};
	};

	Vector<3, T>() : x(0.0f), y(0.0f), z(0.0f) {}
	explicit Vector<3, T>(T t) : x(t), y(t), z(t) {}
	Vector<3, T>(T xv, T yv, T zv) : x(xv), y(yv), z(zv) {}
	Vector<3, T>(const Vector<2, T>& other, T tz) : x(other.x), y(other.y), z(tz) {}
	Vector<3, T>(const Vector<3, T>& other) : x(other.x), y(other.y), z(other.z) {}
	Vector<3, T>(const T* value) {
		x = *value;
		++value;
		y = *value;
		++value;
		z = *value;
	}
	Vector<3, T>(const Vector<2, T>& v) { x = v.x; y = v.y; z = 0.0f; }
	const T& operator[] (int idx) const { return data[idx]; }
	T& operator[] (int idx) { return data[idx]; }
	T* operator() () {
		return &data[0];
	}
	Vector<3, T>& operator = (const Vector<3, T>& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	Vector<2, T> xy() const {
		return Vector<2, T>(x, y);
	}
};

// ------------------------------------------------
// Vector 4 
// ------------------------------------------------
template <class T> struct Vector<4, T> {
	union {
		T data[4];
		struct {
			T x, y, z, w;
		};
		struct {
			T r, g, b, a;
		};
	};
	Vector<4, T>() : x(0), y(0), z(0), w(0) {}
	explicit Vector<4, T>(T t) : x(t), y(t), z(t), w(t) {}
	Vector<4, T>(T tx, T ty, T tz, T tw) : x(tx), y(ty), z(tz), w(tw) {}
	Vector<4, T>(const Vector<4, T>& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
	Vector<4, T>(const Vector<3, T>& other, float tw) : x(other.x), y(other.y), z(other.z), w(tw) {}
	Vector<4, T>(const T* data) {
		x = *data;
		++data;
		y = *data;
		++data;
		z = *data;
		++data;
		w = *data;
	}
	const T* operator() () const {
		return &data[0];
	}
	Vector<4, T>& operator = (const Vector<4, T>& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}
	Vector<2, T> xy() const {
		return Vector<2, T>(x, y);
	}
	Vector<3, T> xyz() const {
		return Vector<3, T>(x, y, z);
	}
};

// ------------------------------------------------
// Type definitions
// ------------------------------------------------
typedef Vector<2, float> v2;
typedef Vector<3, float> v3;
typedef Vector<4, float> v4;

const v2 V2_RIGHT = v2(1, 0);
const v2 V2_LEFT = v2(-1, 0);
const v2 V2_UP = v2(0, 1);
const v2 V2_DOWN = v2(0, -1);
const v2 V2_ZERO = v2(0, 0);
const v2 V2_ONE = v2(1, 1);

const v3 V3_RIGHT = v3(1, 0, 0);
const v3 V3_LEFT = v3(-1, 0, 0);
const v3 V3_UP = v3(0, 1, 0);
const v3 V3_DOWN = v3(0, -1, 0);
const v3 V3_FORWARD = v3(0, 0, -1);
const v3 V3_BACKWARD = v3(0, 0, 1);
const v3 V3_ZERO = v3(0, 0, 0);
const v3 V3_ONE = v3(1, 1, 1);

// ------------------------------------------------
// Matrix
// ------------------------------------------------
struct matrix {

	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};

	matrix();
	matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

	operator float *() const { return (float *)&_11; }
};

matrix mat_identity();

matrix mat_OrthoLH(float w, float h, float zn, float zf);

matrix mat_Scale(const v3& scale);

matrix mat_RotationX(float angle);

matrix mat_RotationY(float angle);

matrix mat_RotationZ(float angle);

matrix mat_Rotation(const v3& r);

matrix mat_Transpose(const matrix& m);

matrix mat_Translate(const v3& pos);

matrix mat_LookAtLH(const v3& eye, const v3& lookAt, const v3& up);

matrix mat_PerspectiveFovLH(float fovy, float aspect, float zn, float zf);

v3 mat_TransformNormal(const v3& v, const matrix& m);

matrix mat_Rotation(const v3& v, float angle);

matrix operator * (const matrix& m1, const matrix& m2);

v3 operator * (const matrix& m, const v3& v);

v4 operator * (const matrix& m, const v4& v);

template<int Size, class T>
bool operator == (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	for (int i = 0; i < Size; ++i) {
		if (u.data[i] != v.data[i]) {
			return false;
		}
	}
	return true;
}

template<int Size, class T>
bool operator != (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	for (int i = 0; i < Size; ++i) {
		if (u.data[i] != v.data[i]) {
			return true;
		}
	}
	return false;
}

template<int Size, class T>
Vector<Size, T> operator - (const Vector<Size, T>& v) {
	Vector<Size, T> ret;
	for (int i = 0; i < Size; ++i) {
		ret.data[i] = -v.data[i];
	}
	return ret;
}

template<int Size, class T>
Vector<Size, T> operator += (Vector<Size, T>& u, const Vector<Size, T>& v) {
	for (int i = 0; i < Size; ++i) {
		u.data[i] += v.data[i];
	}
	return u;
}

template<int Size, class T>
Vector<Size, T> operator += (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> r;
	for (int i = 0; i < Size; ++i) {
		r.data[i] = u.data[i] + v.data[i];
	}
	return r;
}

template<int Size, class T>
Vector<Size, T> operator *= (Vector<Size, T>& u, T other) {
	for (int i = 0; i < Size; ++i) {
		u.data[i] *= other;
	}
	return u;
}

template<int Size, class T>
Vector<Size, T>& operator /= (Vector<Size, T>& u, T other) {
	for (int i = 0; i < Size; ++i) {
		u.data[i] /= other;
	}
	return u;
}

template<int Size, class T>
Vector<Size, T>& operator -= (Vector<Size, T>& u, const Vector<Size, T>& v) {
	for (int i = 0; i < Size; ++i) {
		u.data[i] -= v.data[i];
	}
	return u;
}

template<int Size, class T>
Vector<Size, T>& operator -= (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> r;
	for (int i = 0; i < Size; ++i) {
		r.data[i] = u.data[i] - v.data[i];
	}
	return r;
}

template<int Size, class T>
Vector<Size, T> operator + (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> ret = u;
	return ret += v;
}

template<int Size, class T>
Vector<Size, T> operator - (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> ret = u;
	return ret -= v;
}

template<int Size, class T>
Vector<Size, T> operator * (const Vector<Size, T>& u, const T& v) {
	Vector<Size, T> ret = u;
	return ret *= v;
}

template<int Size, class T>
Vector<Size, T> operator * (const T& v, const Vector<Size, T>& u) {
	Vector<Size, T> ret = u;
	return ret *= v;
}

template<int Size, class T>
Vector<Size, T> operator / (const Vector<Size, T>& u, const T& v) {
	Vector<Size, T> ret = u;
	return ret /= v;
}

template<int Size, class T>
T dot(const Vector<Size, T>& v, const Vector<Size, T>& u) {
	T t(0);
	for (int i = 0; i < Size; ++i) {
		t += v.data[i] * u.data[i];
	}
	return t;
}

template<int Size, class T>
T length(const Vector<Size, T>& v) {
	T t = dot(v, v);
	float tmp = std::sqrt(static_cast<float>(t));
	return static_cast<T>(tmp);
}

template<int Size, class T>
T sqr_length(const Vector<Size, T>& v) {
	return dot(v, v);
}

template<int Size, class T>
Vector<Size, T> normalize(const Vector<Size, T>& u) {
	T len = length(u);
	if (len == 0.0f) {
		return Vector<Size, T>();
	}
	return u / len;
}

template<int Size, class T>
Vector<Size, T>* normalize(const Vector<Size, T>& u, Vector<Size, T>* ret) {
	T len = length(u);
	for (int i = 0; i < Size; ++i) {
		ret->data[i] /= len;
	}
	return ret;
}

template<int Size, class T>
T distance(const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> sub = u - v;
	return length(sub);
}

template<int Size, class T>
T sqr_distance(const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> sub = u - v;
	return sqr_length(sub);
}

template<class T>
Vector<3, T> cross(const Vector<3, T>& u, const Vector<3, T>& v) {
	T x = u.y * v.z - u.z * v.y;
	T y = u.z * v.x - u.x * v.z;
	T z = u.x * v.y - u.y * v.x;
	return Vector<3, T>(x, y, z);
}

template<class T>
Vector<3, T>* cross(const Vector<3, T>& u, const Vector<3, T>& v, Vector<3, T>* ret) {
	ret->x = u.y * v.z - u.z * v.y;
	ret->y = u.z * v.x - u.x * v.z;
	ret->z = u.x * v.y - u.y * v.x;
	return ret;
}

template<class T>
T inline cross(const Vector<2, T>& v1, const Vector<2, T>& v2) {
	return v1.x * v2.y - v2.x * v1.y;
}

template<int Size>
Vector<Size, float>* lerp(const Vector<Size, float>& u, const Vector<Size, float>& v, float time, Vector<Size, float>* ret) {
	float norm = time;
	if (norm < 0.0f) {
		norm = 0.0f;
	}
	if (norm > 1.0f) {
		norm = 1.0f;
	}
	for (int i = 0; i < Size; ++i) {
		ret->data[i] = u.data[i] * (1.0f - norm) + v.data[i] * norm;
	}
	return ret;
}

template<int Size>
Vector<Size, float> lerp(const Vector<Size, float>& u, const Vector<Size, float>& v, float time) {
	Vector<Size, float> ret;
	lerp(u, v, time, &ret);
	return ret;
}

template<int Size, class T>
Vector<Size, T> vec_min(const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> ret;
	for (int i = 0; i < Size; ++i) {
		if (u.data[i] <= v.data[i]) {
			ret.data[i] = u.data[i];
		}
		else {
			ret.data[i] = v.data[i];
		}
	}
	return ret;
}

template<int Size, class T>
Vector<Size, T> vec_max(const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> ret;
	for (int i = 0; i < Size; ++i) {
		if (u.data[i] >= v.data[i]) {
			ret.data[i] = u.data[i];
		}
		else {
			ret.data[i] = v.data[i];
		}
	}
	return ret;
}

template<int Size, class T>
Vector<Size, T> clamp(const Vector<Size, T>& u, const Vector<Size, T>& min, const Vector<Size, T>& max) {
	Vector<Size, T> ret;
	for (int i = 0; i < Size; ++i) {
		ret.data[i] = u.data[i];
		if (u.data[i] > max.data[i]) {
			ret.data[i] = max.data[i];
		}
		else if (u.data[i] < min.data[i]) {
			ret.data[i] = min.data[i];
		}
	}
	return ret;
}

template<int Size, class T>
void clamp(Vector<Size, T>* u, const Vector<Size, T>& min, const Vector<Size, T>& max) {
	for (int i = 0; i < Size; ++i) {
		if (u->data[i] > max.data[i]) {
			u->data[i] = max.data[i];
		}
		else if (u->data[i] < min.data[i]) {
			u->data[i] = min.data[i];
		}
	}
}

template<int Size>
Vector<Size, float> saturate(const Vector<Size, float>& u) {
	return clamp(u, Vector<Size, float>(0.0f), Vector<Size, float>(1.0f));
}

template<int Size>
Vector<Size, int> saturate(const Vector<Size, int>& u) {
	return clamp(u, Vector<Size, int>(0), Vector<Size, int>(1));
}

template<int Size, class T>
void limit(Vector<Size, T>* v, const Vector<Size, T>& u) {
	for (int i = 0; i < Size; ++i) {
		if (v->data[i] > u.data[i]) {
			v->data[i] = u.data[i];
		}
		else if (v->data[i] < -u.data[i]) {
			v->data[i] = -u.data[i];
		}
	}
}

template<int Size, class T>
Vector<Size, T> reflect(const Vector<Size, T>& u, const Vector<Size, T>& norm) {
	Vector<Size, T> ret;
	float dp = dot(u, norm);
	for (int i = 0; i < Size; ++i) {
		ret.data[i] = u.data[i] - 2.0f * dp * norm.data[i];
	}
	return ret;
}

inline matrix::matrix() {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] = 0.0f;
		}
	}
}

inline matrix::matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44) {
	_11 = m11;
	_12 = m12;
	_13 = m13;
	_14 = m14;
	_21 = m21;
	_22 = m22;
	_23 = m23;
	_24 = m24;
	_31 = m31;
	_32 = m32;
	_33 = m33;
	_34 = m34;
	_41 = m41;
	_42 = m42;
	_43 = m43;
	_44 = m44;
}

// ----------------------------------------------------------------------
//
// The rendering API
//
// ----------------------------------------------------------------------
namespace ds {

	const float PI = 3.141592654f;
	const float TWO_PI = 2.0f * PI;

	typedef struct Color {
		union {
			float values[4];
			struct {
				float r;
				float g;
				float b;
				float a;
			};
		};
		Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
		Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
		Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.0f) {}
		Color(int _r, int _g, int _b, int _a) {
			r = static_cast<float>(_r) / 255.0f;
			g = static_cast<float>(_g) / 255.0f;
			b = static_cast<float>(_b) / 255.0f;
			a = static_cast<float>(_a) / 255.0f;
		}
		operator float* () {
			return &values[0];
		}

		operator const float* () const {
			return &values[0];
		}

	} Color;



	enum BufferAttribute {
		POSITION,
		COLOR,
		TEXCOORD,
		NORMAL
	};

	enum BufferAttributeType {
		FLOAT,
		UINT_8
	};

	enum BufferType {
		STATIC,
		DYNAMIC
	};

	enum TextureAddressModes {
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MIRROR_ONCE
	};

	enum TextureFilters {
		POINT,
		LINEAR,
		ANISOTROPIC
	};

	enum PrimitiveTypes {
		POINT_LIST,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		LINE_LIST
	};

	struct VertexDeclaration {
		BufferAttribute attribute;
		BufferAttributeType type;
		uint8_t size;
	};

	enum BlendStates {
		ZERO,
		ONE,
		SRC_COLOR,
		INV_SRC_COLOR,
		SRC_ALPHA,
		INV_SRC_ALPHA,
		DEST_ALPHA,
		INV_DEST_ALPHA,
		DEST_COLOR,
		INV_DEST_COLOR,
		SRC_ALPHA_SAT,
		BLEND_FACTOR,
		INV_BLEND_FACTOR,
		SRC1_COLOR,
		INV_SRC1_COLOR,
		SRC1_ALPHA,
		INV_SRC1_ALPHA
	};

	// ---------------------------------------------------
	// The depth buffer state
	// ---------------------------------------------------
	enum DepthBufferState {
		ENABLED,
		DISABLED
	};

	// ---------------------------------------------------
	// Supported texture formats
	// FIXME: add much more!
	// ---------------------------------------------------
	enum TextureFormat {
		R8_UINT,
		R8G8B8A8_UINT,
		R8G8B8A8_UNORM
	};

	// ---------------------------------------------------
	// The two valid formats of an index buffer
	// ---------------------------------------------------
	enum IndexType {
		UINT_16,
		UINT_32
	};

	// ---------------------------------------------------
	// The shader type
	// ---------------------------------------------------
	enum ShaderType {
		VERTEX,
		PIXEL,
		GEOMETRY
	};

	// ---------------------------------------------------
	// cull mode
	// ---------------------------------------------------
	enum CullMode {
		NONE = 1,
		FRONT = 2,
		BACK = 3
	};

	// ---------------------------------------------------
	// fill mode
	// ---------------------------------------------------
	enum FillMode {
		WIREFRAME = 2,
		SOLID = 3
	};

	// ---------------------------------------------------
	// Render settings 
	// ---------------------------------------------------
	typedef struct {
		uint16_t width;
		uint16_t height;
		Color clearColor;
		uint8_t multisampling;
		const char* title;
	} RenderSettings;

	typedef struct {
		uint16_t textures;
		uint16_t vertices;
		uint16_t indices;
		uint16_t shaders;
		uint16_t fps;
	} DrawStatistics;

	bool init(const RenderSettings& settings);

	// vertex declaration / buffer input layout

	RID createVertexDeclaration(VertexDeclaration* decl, uint8_t num, RID shaderId);

	//! Sets the current vertex declaration (input buffer layout).
	/*!
		The engine(??) will track the selected vertex declaration
		and will prevent dupilcate settings

		\param rid the resource of the vertex declaration
	*/
	void setVertexDeclaration(RID rid);

	// constant buffer

	RID createConstantBuffer(int byteWidth);

	void updateConstantBuffer(RID rid, void* data, size_t size);

	void setVertexConstantBuffer(RID rid);

	void setPixelConstantBuffer(RID rid);

	void setGeometryConstantBuffer(RID rid);

	// index buffer

	RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type);

	RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type, void* data);

	RID createQuadIndexBuffer(int numQuads);

	void setIndexBuffer(RID rid);

	// vertex buffer

	RID createVertexBuffer(BufferType type, int numVertices, RID vertexDecl);

	RID createVertexBuffer(BufferType type, int numVertices, RID vertexDecl, void* data, unsigned long long vertexSize);

	void setVertexBuffer(RID rid, uint32_t* stride, uint32_t* offset, PrimitiveTypes topology);

	void mapBufferData(RID rid, void* data, uint32_t size);

	// sampler state

	RID createSamplerState(TextureAddressModes addressMode, TextureFilters filter);

	void setSamplerState(RID rid);

	// blendstate

	RID createBlendState(BlendStates srcBlend, BlendStates srcAlphaBlend, BlendStates destBlend, BlendStates destAlphaBlend, bool alphaEnabled);	

	RID createAlphaBlendState(BlendStates srcBlend, BlendStates destBlend);

	void setBlendState(RID rid);

	void setBlendState(RID rid, float* blendFactor,uint32_t mask);

	// shader

	RID createShader();

	void loadVertexShader(RID shader, const char* csoName);

	void loadPixelShader(RID shader, const char* csoName);

	void loadGeometryShader(RID shader, const char* csoName);

	void setShader(RID rid);

	// texture

	RID createTexture(int width, int height, uint8_t channels, void* data, TextureFormat format);

	void setTexture(RID rid, ShaderType type);

	void setTexture(RID rid, ShaderType type, uint8_t slot);

	void setTextureFromRenderTarget(RID rtID, ShaderType type, uint8_t slot);

	// render target

	RID createRenderTarget(uint16_t width, uint16_t height);

	void setRenderTarget(RID rtID);

	void restoreBackBuffer();

	// rasterizer state
	RID createRasterizerState(CullMode cullMode, FillMode fillMode, bool multiSample, bool scissor, float depthBias, float slopeDepthBias);

	void setDepthBufferState(DepthBufferState state);

	void drawIndexed(uint32_t num);

	void draw(uint32_t num);

	void begin();

	void end();

	bool isRunning();

	void shutdown();

	// view and projection matrix

	const matrix& getViewMatrix();

	const matrix& getProjectionMatrix();

	const matrix& getViewProjectionMatrix();

	void setViewPosition(const v3& viewPos);

	v3 getViewPosition();

	void lookAt(const v3& pos);

	void setProjectionMatrix(float fieldOfView, float aspectRatio);

	void setProjectionMatrix(float fieldOfView, float aspectRatio, float minDepth, float maxDepth);

	// input

	bool isKeyPressed(uint8_t key);

	v2 getMousePosition();

	bool isMouseButtonPressed(int button);	

	// timing

	uint64_t getElapsedTicks();

	double getElapsedSeconds();

	uint64_t getTotalTicks();

	double GetTotalSeconds();

	uint32_t getFrameCount();

	uint32_t getFramesPerSecond();

	const DrawStatistics& getStatistics();

	// utils

	float random(float min, float max);

	const char* getLastError();

	// Batching class
	struct BatchDescriptor {
		uint16_t size; // FIXME: use better name
		RID vertexBufferID;
		PrimitiveTypes primitiveType;		
		RID vertexDeclarationID;
		// FIXME: add index buffer
	};
	template<class T>
	class Batch {

	public:
		Batch(const BatchDescriptor& descriptor) : _data(0) , _descriptor(descriptor) {
			_size = descriptor.size;
			_data = new T[_size];
		}
		~Batch() {
			if (_data != 0) {
				delete[] _data;
			}
		}

		void add(const T& t) {
			if ((_index + 1) >= _size) {
				flush();
			}
			_data[_index++] = t;
		}

		void begin() {
			_index = 0;
			_calls = 0;
		}

		void flush() {
			if (_index > 0) {
				++_calls;
				unsigned int stride = sizeof(T);
				unsigned int offset = 0;
				ds::setVertexBuffer(_descriptor.vertexBufferID, &stride, &offset, _descriptor.primitiveType);
				ds::setVertexDeclaration(_descriptor.vertexDeclarationID);
				ds::mapBufferData(_descriptor.vertexBufferID, _data, _size * sizeof(T));
				ds::draw(_index);
				_index = 0;
			}
		}
		uint16_t getCalls() const {
			return _calls;
		}
	private:
		uint16_t _index;
		uint16_t _size;
		uint16_t _calls;
		T* _data;
		BatchDescriptor _descriptor;
	};
}

#ifdef DS_IMPLEMENTATION

#include <Windows.h>
#include <crtdbg.h>  
#include <d3d11.h>
#include <vector>
#include <random>

namespace ds {

	static void assert_fmt(char* expr_str, bool expr, char* file, int line, char* format, ...);

	static void assert_fmt(char* file, int line, char* format, ...);

	static void reportLastError(const char* fileName, int line, const char* method, HRESULT hr);

	static void assert_result(const char* file, int line, HRESULT result, const char* msg);

}
#ifndef XASSERT
#define XASSERT(Expr, s, ...) do { ds::assert_fmt(#Expr, Expr,__FILE__,__LINE__,s,__VA_ARGS__); } while(false);
#endif
#ifndef ASSERT_RESULT
#define ASSERT_RESULT(r,s) do { ds::assert_result(__FILE__,__LINE__,r,s); } while(false);
#endif
#ifndef REPORT
#define REPORT(s,d) do { ds::reportLastError(__FILE__,__LINE__,s,d); } while(false);
#endif

namespace ds {

	enum ResourceType {
		RT_NONE,
		RT_TEXTURE,
		RT_VERTEX_DECLARATION,
		RT_CONSTANT_BUFFER,
		RT_INDEX_BUFFER,
		RT_VERTEX_BUFFER,
		RT_SAMPLER_STATE,
		RT_BLENDSTATE,
		RT_SHADER,
		RT_SRV,
		RT_RASTERIZER_STATE,
		RT_RENDER_TARGET
	};

	const char* RESOURCE_NAMES[] {
		"NONE",
		"TEXTURE",
		"VERTEX_DECLARATION",
		"CONSTANT_BUFFER",
		"INDEX_BUFFER",
		"VERTEX_BUFFER",
		"SAMPLER_STATE",
		"BLENDSTATE",
		"SHADER",
		"SRV",
		"RASTERIZER_STATE",
		"RENDER_TARGET"
	};

	// ------------------------------------------------------
	// query refresh rate
	// ------------------------------------------------------
	// http://www.rastertek.com/dx11tut03.html
	static bool QueryRefreshRate(UINT screenWidth, UINT screenHeight, bool vsync, DXGI_RATIONAL* rational) {
		DXGI_RATIONAL refreshRate = { 0, 1 };
		if (vsync)
		{
			IDXGIFactory* factory;
			IDXGIAdapter* adapter;
			IDXGIOutput* adapterOutput;
			DXGI_MODE_DESC* displayModeList;

			// Create a DirectX graphics interface factory.
			HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Could not create DXGIFactory instance."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			hr = factory->EnumAdapters(0, &adapter);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapters."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			hr = adapter->EnumOutputs(0, &adapterOutput);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapter outputs."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			UINT numDisplayModes;
			hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to query display mode list."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			displayModeList = new DXGI_MODE_DESC[numDisplayModes];
			//assert(displayModeList);

			hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to query display mode list."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			// Now store the refresh rate of the monitor that matches the width and height of the requested screen.
			for (UINT i = 0; i < numDisplayModes; ++i)
			{
				if (displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight)
				{
					refreshRate = displayModeList[i].RefreshRate;
				}
			}

			delete[] displayModeList;
			if (adapterOutput) {
				adapterOutput->Release();
			}
			if (adapter) {
				adapter->Release();
			}
			if (factory) {
				factory->Release();
			}
		}

		*rational = refreshRate;
		//LOG << "refresh: " << refreshRate.Numerator << " " << refreshRate.Denominator;
		return true;
	}

	// ------------------------------------------------------
	// Shader
	// ------------------------------------------------------
	struct Shader {
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11GeometryShader* geometryShader;
		void* vertexShaderBuffer;
		UINT bufferSize;
		ID3D11SamplerState* samplerState;
		ID3DBlob* geometryShaderBuffer;

		Shader() : vertexShader(0), pixelShader(0), vertexShaderBuffer(0), geometryShader(0), samplerState(0) {}

	};

	// ------------------------------------------------------
	// internal texture 
	// ------------------------------------------------------
	struct InternalTexture {
		int width;
		int height;
		RID samplerState;
		ID3D11ShaderResourceView* srv;
	};

	struct RenderTarget {
		ID3D11Texture2D* texture;
		ID3D11RenderTargetView* view;
		ID3D11ShaderResourceView* srv;
		ID3D11Texture2D* depthTexture;
		ID3D11DepthStencilView* depthStencilView;
	};

	// ------------------------------------------------------
	// Resource management
	// ------------------------------------------------------	
	class BaseResource {

	public:
		BaseResource() {}
		virtual ~BaseResource() {}
		virtual void release() = 0;
		virtual const ResourceType getType() const = 0;
	};

	template<class T>
	class AbstractResource : public BaseResource {

	public:
		AbstractResource(T t) : _data(t) {}
		virtual ~AbstractResource() {}
		virtual void release() = 0;
		T get() {
			return _data;
		}
		const T get() const {
			return _data;
		}
		
	protected:
		T _data;
	};

	class ConstantBufferResource : public AbstractResource<ID3D11Buffer*> {

	public:
		ConstantBufferResource(ID3D11Buffer* t) : AbstractResource(t) {}
		virtual ~ConstantBufferResource() {}
		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_CONSTANT_BUFFER;
		}
	};

	class InputLayoutResource : public AbstractResource<ID3D11InputLayout*> {

	public:
		InputLayoutResource(ID3D11InputLayout* t, int size) : AbstractResource(t), _size(size) {}
		virtual ~InputLayoutResource() {}
		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		int size() const {
			return _size;
		}
		const ResourceType getType() const {
			return RT_VERTEX_DECLARATION;
		}
	private:
		int _size;
	};

	class VertexBufferResource : public AbstractResource<ID3D11Buffer*> {

	public:
		VertexBufferResource(ID3D11Buffer* t, int size, BufferType type, RID inputLayout) : AbstractResource(t), _size(size), _inputLayout(inputLayout) {}
		virtual ~VertexBufferResource() {}

		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		int size() const {
			return _size;
		}
		RID getInputLayout() const {
			return _inputLayout;
		}
		const ResourceType getType() const {
			return RT_VERTEX_BUFFER;
		}
		BufferType getBufferType() const {
			return _type;
		}
	private:
		RID _inputLayout;
		int _size;
		BufferType _type;
	};

	class IndexBufferResource : public AbstractResource<ID3D11Buffer*> {

	public:
		IndexBufferResource(ID3D11Buffer* t, DXGI_FORMAT format,uint32_t numIndices,BufferType type) 
			: AbstractResource(t) , _format(format) , _numIndices(numIndices) , _type(type) {}
		virtual ~IndexBufferResource() {}
		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_INDEX_BUFFER;
		}
		DXGI_FORMAT getFormat() const {
			return _format;
		}
		uint32_t getNumIndices() const {
			return _numIndices;
		}
		BufferType getBufferType() const {
			return _type;
		}
	private:
		DXGI_FORMAT _format;
		uint32_t _numIndices;
		BufferType _type;
	};

	class ShaderResourceViewResource : public AbstractResource<ID3D11ShaderResourceView*> {

	public:
		ShaderResourceViewResource(ID3D11ShaderResourceView* t) : AbstractResource(t) {}
		virtual ~ShaderResourceViewResource() {}
		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_SRV;
		}
	};

	class BlendStateResource : public AbstractResource<ID3D11BlendState*> {

	public:
		BlendStateResource(ID3D11BlendState* t) : AbstractResource(t) {}
		virtual ~BlendStateResource() {}
		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_BLENDSTATE;
		}
	};

	class SamplerStateResource : public AbstractResource<ID3D11SamplerState*> {

	public:
		SamplerStateResource(ID3D11SamplerState* t) : AbstractResource(t) {}
		virtual ~SamplerStateResource() {}
		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_SAMPLER_STATE;
		}
	};

	class RasterizerStateResource : public AbstractResource<ID3D11RasterizerState*> {

	public:
		RasterizerStateResource(ID3D11RasterizerState* t) : AbstractResource(t) {}
		virtual ~RasterizerStateResource() {}
		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_RASTERIZER_STATE;
		}
	};

	class ShaderResource : public AbstractResource<Shader*> {

	public:
		ShaderResource(Shader* t) : AbstractResource(t) {}
		virtual ~ShaderResource() {}
		void release() {
			if (_data != 0) {
				if (_data->vertexShader != 0) {
					_data->vertexShader->Release();
					_data->vertexShader = 0;
				}
				if (_data->pixelShader != 0) {
					_data->pixelShader->Release();
					_data->pixelShader = 0;
				}
				if (_data->geometryShader != 0) {
					_data->geometryShader->Release();
					_data->geometryShader = 0;
				}
				if (_data->vertexShaderBuffer != 0) {
					delete[] _data->vertexShaderBuffer;
					_data->vertexShaderBuffer = 0;
				}
				delete _data;
			}
		}
		const ResourceType getType() const {
			return RT_SHADER;
		}
	};

	class RenderTargetResource : public AbstractResource<RenderTarget*> {

	public:
		RenderTargetResource(RenderTarget* t) : AbstractResource(t) {}
		virtual ~RenderTargetResource() {}
		void release() {
			if (_data != 0) {
				if (_data->texture != 0) {
					_data->texture->Release();
				}
				if (_data->view != 0) {
					_data->view->Release();
				}
				if (_data->srv != 0) {
					_data->srv->Release();
				}
				if (_data->depthTexture != 0) {
					_data->depthTexture->Release();
				}
				if (_data->depthStencilView != 0) {
					_data->depthStencilView->Release();
				}
				delete _data;
			}
		}
		const ResourceType getType() const {
			return RT_RENDER_TARGET;
		}
	};

	// ------------------------------------------------------
	// Internal context
	// ------------------------------------------------------
	typedef struct {
		HWND hwnd;
		HINSTANCE instance;
		uint16_t screenWidth;
		uint16_t screenHeight;
		Color clearColor;
		uint8_t multisampling;
		bool running;
		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;

		ID3D11Device* d3dDevice;
		ID3D11DeviceContext* d3dContext;
		IDXGISwapChain* swapChain;
		ID3D11RenderTargetView* backBufferTarget;

		ID3D11Texture2D* depthTexture;
		ID3D11DepthStencilView* depthStencilView;

		ID3D11DepthStencilState* depthDisabledStencilState;
		ID3D11DepthStencilState* depthEnabledStencilState;

		matrix viewMatrix;
		matrix projectionMatrix;
		matrix viewProjectionMatrix;

		std::vector<BaseResource*> _resources;

		v3 viewPosition;
		v3 lookAt;
		v3 up;

		int mouseButtonState[2];
		int keyState[256];

		DepthBufferState depthBufferState;

		// Timing
		LARGE_INTEGER timerFrequency;
		LARGE_INTEGER lastTime;
		// Derived timing data uses a canonical tick format. 
		uint64_t elapsedTicks;
		uint64_t totalTicks;
		uint64_t leftOverTicks;

		// Members for tracking the framerate. 
		uint32_t frameCount;
		uint32_t framesPerSecond;
		uint32_t framesThisSecond;
		uint64_t secondCounter;
		uint64_t maxDelta;

		char errorBuffer[256];
		bool broken;

		uint32_t selectedShaderId;
		uint32_t selectedVertexDeclaration;
		uint32_t selectedIndexBuffer;
		uint32_t selectedVertexBuffer;
		uint32_t selectedRasterizerState;

		uint32_t selectedPSTextures[16];
		uint32_t selectedVSTextures[16];
		uint32_t selectedGSTextures[16];

		DrawStatistics stats;

	} InternalContext;

	static InternalContext* _ctx;

	static uint16_t id_mask(RID rid) {
		return rid & 0xffff;
	}

	static uint16_t type_mask(RID rid) {
		return (rid << 16) & 0xffff;
	}

	static void assert_resource(RID rid, ResourceType type) {
		XASSERT(type_mask(rid) == type,"The selected resource %d is not the required type %s", rid, RESOURCE_NAMES[type]);
	}

	static RID addResource(BaseResource* res, ResourceType type) {
		XASSERT((_ctx->_resources.size() + 1) < NO_RID, "The maximum number of resources reached");
		_ctx->_resources.push_back(res);
		return static_cast<uint16_t>(_ctx->_resources.size() - 1) + (type >> 16);
	}

	uint16_t getResourceIndex(RID rid,ResourceType type) {
		if (rid != NO_RID) {
			uint16_t idx = id_mask(rid);
			XASSERT(idx < _ctx->_resources.size(), "Invalid resource selected - Out of bounds");
			BaseResource* cbr = _ctx->_resources[idx];
			XASSERT(cbr->getType() == type, "The selected resource %d is not the required type: %s", idx, RESOURCE_NAMES[idx]);
			return idx;
		}
		return NO_RID;
	}
	// -------------------------------------------------------
	// random
	// -------------------------------------------------------
	static std::mt19937 mt;

	static void init_random(unsigned long seed) {
		std::random_device r;
		std::seed_seq new_seed{ r(), r(), r(), r(), r(), r(), r(), r() };
		mt.seed(new_seed);
	}

	
	float random(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(mt);
	}

	const char* getLastError() {
		return _ctx->errorBuffer;
	}

	static const uint64_t TicksPerSecond = 10000000;

	static double TicksToSeconds(uint64_t ticks) { 
		return static_cast<double>(ticks) / TicksPerSecond; 
	}
	static uint64_t SecondsToTicks(double seconds) { 
		return static_cast<uint64_t>(seconds * TicksPerSecond); 
	}

	uint64_t getElapsedTicks() { 
		return _ctx->elapsedTicks;
	}
	double getElapsedSeconds() { 
		return TicksToSeconds(_ctx->elapsedTicks);
	}

	// Get total time since the start of the program. 
	uint64_t getTotalTicks() { 
		return _ctx->totalTicks;
	}
	double GetTotalSeconds() { 
		return TicksToSeconds(_ctx->totalTicks);
	}

	// Get total number of updates since start of the program. 
	uint32_t getFrameCount() { 
		return _ctx->frameCount;
	}

	// Get the current framerate. 
	uint32_t getFramesPerSecond() { 
		return _ctx->framesPerSecond;
	}

	static void reportLastError(const char* fileName, int line, const char* method, HRESULT hr) {
		char msg[256];
		DWORD result = FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&msg,
			255, NULL);
		if (result > 0) {
			sprintf_s(_ctx->errorBuffer,"file: %s (%d) method: %s - %s\n", fileName, line, method, msg);
			MessageBox(GetDesktopWindow(), _ctx->errorBuffer, "ERROR", NULL);
			_ctx->running = false;
			exit(-1);
		}
	}

	// ------------------------------------------------------
	// assert functions
	// ------------------------------------------------------
	static void assert_fmt(char* expr_str, bool expr, char* file, int line, char* format, ...) {
		if (!expr) {
			va_list args;
			va_start(args, format);
			char buffer[1024];
			memset(buffer, 0, sizeof(buffer));
			int written = vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, args);			
			MessageBox(_ctx->hwnd, buffer, "ERROR", NULL);
			va_end(args);
			exit(-1);
		}
	}

	static void assert_fmt(char* file, int line, char* format, ...) {
		va_list args;
		va_start(args, format);
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		int written = vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, args);
		MessageBox(_ctx->hwnd, buffer, "ERROR", NULL);
		va_end(args);
		exit(-1);
	}

	static void assert_result(HRESULT result, const char* msg) {
		if (FAILED(result)) {
			REPORT(msg, result);
		}
	}

	static void assert_result(const char* file, int line, HRESULT result, const char* msg) {
		if (FAILED(result)) {
			reportLastError(file, line, msg, result);
		}
	}

	// ------------------------------------------------------
	// is running
	// ------------------------------------------------------
	bool isRunning() {
		MSG msg = { 0 };
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		uint64_t timeDelta = currentTime.QuadPart - _ctx->lastTime.QuadPart;

		_ctx->lastTime = currentTime;
		_ctx->secondCounter += timeDelta;

		// Clamp excessively large time deltas (e.g. after paused in the debugger). 
		if (timeDelta > _ctx->maxDelta)
		{
			timeDelta = _ctx->maxDelta;
		}

		// Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp. 
		timeDelta *= TicksPerSecond;
		timeDelta /= _ctx->timerFrequency.QuadPart;

		uint32_t lastFrameCount = _ctx->frameCount;
		
		// Variable timestep update logic. 
		_ctx->elapsedTicks = timeDelta;
		_ctx->totalTicks += timeDelta;
		_ctx->leftOverTicks = 0;
		_ctx->frameCount++;
		// Track the current framerate. 
		if (_ctx->frameCount != lastFrameCount) {
			_ctx->framesThisSecond++;
		}

		if (_ctx->secondCounter >= static_cast<uint64_t>(_ctx->timerFrequency.QuadPart))	{
			_ctx->framesPerSecond = _ctx->framesThisSecond;
			_ctx->framesThisSecond = 0;
			_ctx->secondCounter %= _ctx->timerFrequency.QuadPart;
		}

		return _ctx->running;
	}


	// ------------------------------------------------------
	// initialize device
	// ------------------------------------------------------
	static bool initializeDevice(const RenderSettings& settings) {
		_ctx->clearColor = settings.clearColor;
		_ctx->multisampling = settings.multisampling;
		RECT dimensions;
		GetClientRect(_ctx->hwnd, &dimensions);

		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
		};

		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);



		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = _ctx->screenWidth;
		swapChainDesc.BufferDesc.Height = _ctx->screenHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_RATIONAL refreshRate;
		QueryRefreshRate(_ctx->screenWidth, _ctx->screenHeight, true, &refreshRate);
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = _ctx->hwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = _ctx->multisampling;
		swapChainDesc.SampleDesc.Quality = 0;

		unsigned int creationFlags = 0;

#ifdef DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT result;
		unsigned int driver = 0;

		for (driver = 0; driver < totalDriverTypes; ++driver) {
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
				featureLevels, totalFeatureLevels,
				D3D11_SDK_VERSION, &swapChainDesc, &_ctx->swapChain, &_ctx->d3dDevice, &_ctx->featureLevel, &_ctx->d3dContext);

			if (SUCCEEDED(result)) {
				_ctx->driverType = driverTypes[driver];
				break;
			}
		}

		if (FAILED(result)) {
			REPORT("Failed to create the Direct3D device!", result);
			return false;
		}

		ID3D11Texture2D* backBufferTexture;

		result = _ctx->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);

		if (FAILED(result)) {
			REPORT("Failed to get the swap chain back buffer!",result);
			return false;
		}

		result = _ctx->d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &_ctx->backBufferTarget);

		if (backBufferTexture)
			backBufferTexture->Release();

		if (FAILED(result))
		{
			REPORT("Failed to create the render target view!", result);
			return false;
		}



		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = _ctx->screenWidth;
		depthTexDesc.Height = _ctx->screenHeight;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = _ctx->multisampling;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;

		result = _ctx->d3dDevice->CreateTexture2D(&depthTexDesc, 0, &_ctx->depthTexture);
		if (FAILED(result)) {
			REPORT("Failed to create the depth texture!", result);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		descDSV.Texture2D.MipSlice = 0;

		result = _ctx->d3dDevice->CreateDepthStencilView(_ctx->depthTexture, &descDSV, &_ctx->depthStencilView);
		if (FAILED(result)) {
			REPORT("Failed to create the depth stencil view!", result);
			return false;
		}

		_ctx->d3dContext->OMSetRenderTargets(1, &_ctx->backBufferTarget, _ctx->depthStencilView);

		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(_ctx->screenWidth);
		viewport.Height = static_cast<float>(_ctx->screenHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		_ctx->d3dContext->RSSetViewports(1, &viewport);

		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
		// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the state using the device.
		result = _ctx->d3dDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &_ctx->depthDisabledStencilState);
		if (FAILED(result)) {
			REPORT("failed to create depth stencil state", result);
			return false;
		}

		depthDisabledStencilDesc.DepthEnable = true;
		result = _ctx->d3dDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &_ctx->depthEnabledStencilState);
		if (FAILED(result)) {
			REPORT("failed to create depth stencil state", result);
			return false;
		}

		_ctx->viewMatrix = mat_identity();

		_ctx->viewPosition = v3(0, 0, -6);
		_ctx->lookAt = v3(0, 0, 0);
		_ctx->up = v3(0, 1, 0);
		_ctx->viewMatrix = mat_LookAtLH(_ctx->viewPosition, _ctx->lookAt, _ctx->up);

		float fieldOfView = PI / 4.0f;
		float screenAspect = (float)_ctx->screenWidth / (float)_ctx->screenHeight;
		_ctx->projectionMatrix = mat_PerspectiveFovLH(fieldOfView, screenAspect, 0.01f, 100.0f);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
		return true;
	}

	// ------------------------------------------------------
	// set view position
	// ------------------------------------------------------
	void setViewPosition(const v3& viewPos) {
		_ctx->viewPosition = viewPos;
		_ctx->viewMatrix = mat_LookAtLH(_ctx->viewPosition, _ctx->lookAt, _ctx->up);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
	}

	v3 getViewPosition() {
		return _ctx->viewPosition;
	}

	// ------------------------------------------------------
	// look at
	// ------------------------------------------------------
	void lookAt(const v3& pos) {
		_ctx->lookAt = pos;
		_ctx->viewMatrix = mat_LookAtLH(_ctx->viewPosition, _ctx->lookAt, _ctx->up);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
	}

	// ------------------------------------------------------
	// set projection matrix
	// ------------------------------------------------------
	void setProjectionMatrix(float fieldOfView, float aspectRatio) {
		_ctx->projectionMatrix = mat_PerspectiveFovLH(fieldOfView, aspectRatio, 0.01f, 100.0f);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
	}

	// ------------------------------------------------------
	// set projection matrix
	// ------------------------------------------------------
	void setProjectionMatrix(float fieldOfView, float aspectRatio, float minDepth, float maxDepth) {
		_ctx->projectionMatrix = mat_PerspectiveFovLH(fieldOfView, aspectRatio, minDepth, maxDepth);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
	}

	// ------------------------------------------------------
	// internal windows procedure
	// ------------------------------------------------------
	bool update_input(RAWINPUT* raw) {
		HWND current = GetForegroundWindow();
		if (current == _ctx->hwnd) {
			if (raw->header.dwType == RIM_TYPEKEYBOARD) {
				const RAWKEYBOARD& rawKB = raw->data.keyboard;
				UINT virtualKey = rawKB.VKey;
				UINT scanCode = rawKB.MakeCode;
				UINT flags = rawKB.Flags;
				if (flags == 0) {
					_ctx->keyState[virtualKey] = 80;
				}
				else {
					_ctx->keyState[virtualKey] = 0;

				}
			}
			if (raw->header.dwType == RIM_TYPEMOUSE) {
				if (raw->data.mouse.ulButtons == 1) {
					_ctx->mouseButtonState[0] = 1;
				}
				if (raw->data.mouse.ulButtons == 2) {
					_ctx->mouseButtonState[0] = 0;
				}
				if (raw->data.mouse.ulButtons == 4) {
					_ctx->mouseButtonState[1] = 1;
				}
				if (raw->data.mouse.ulButtons == 8) {
					_ctx->mouseButtonState[1] = 0;
				}
			}
		}
		return true;
	}

	// ------------------------------------------------------
	// get mouse position
	// ------------------------------------------------------
	v2 getMousePosition() {
		v2 mp;
		tagPOINT p;
		if (GetCursorPos(&p)) {
			if (ScreenToClient(ds::_ctx->hwnd, &p)) {
				mp.x = static_cast<float>(p.x);
				mp.y = static_cast<float>(ds::_ctx->screenHeight - p.y);
			}
		}
		return mp;
	}

	// ------------------------------------------------------
	// is key pressed
	// ------------------------------------------------------
	bool isKeyPressed(uint8_t key) {
		return _ctx->keyState[key] == 80;
	}

	// ------------------------------------------------------
	// is mouse button pressed
	// ------------------------------------------------------
	bool isMouseButtonPressed(int button) {
		return _ctx->mouseButtonState[button] == 80;
	}

	// ------------------------------------------------------
	// internal windows procedure
	// ------------------------------------------------------
	LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			case WM_INPUT: {
				char buffer[sizeof(RAWINPUT)] = {};
				UINT size = sizeof(RAWINPUT);
				GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);
				update_input(raw);
			}
			return 0;
			case WM_LBUTTONDOWN:
				_ctx->mouseButtonState[0] = 80;
				return 0;
			case WM_LBUTTONUP:
				_ctx->mouseButtonState[0] = 0;
				return 0;
			case WM_RBUTTONDOWN:
				_ctx->mouseButtonState[1] = 80;
				return 0;
			case WM_RBUTTONUP:
				_ctx->mouseButtonState[1] = 0;
				return 0;
			case WM_CLOSE:
				_ctx->running = false;
				return 0;
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	// ------------------------------------------------------
	// init
	// ------------------------------------------------------
	bool init(const RenderSettings& settings) {
#ifdef DEBUG
		int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
		flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
		flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
										 //flag |= _CRTDBG_DELAY_FREEMEM_DF;
		_CrtSetDbgFlag(flag); // Set flag to the new value
#endif
		SetThreadAffinityMask(GetCurrentThread(), 1);

		_ctx = new InternalContext;
		_ctx->instance = GetModuleHandle(NULL);
		_ctx->screenWidth = settings.width;
		_ctx->screenHeight = settings.height;
		_ctx->clearColor = settings.clearColor;

		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WinProc;
		wndClass.hInstance = _ctx->instance;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "D11";
		if (!RegisterClassEx(&wndClass)) {
			REPORT("Failed to create window", GetLastError());
			return false;
		}
		RECT DesktopSize;
		GetClientRect(GetDesktopWindow(), &DesktopSize);
		_ctx->hwnd = CreateWindow("D11", "D11",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			(DesktopSize.right - settings.width) / 2, (DesktopSize.bottom - settings.height) / 2,
			settings.width, settings.height,
			NULL, NULL, _ctx->instance, NULL);

		if (!_ctx->hwnd) {
			REPORT("Failed to create window",GetLastError());
			return false;
		}
		RECT rect = { 0, 0, settings.width, settings.height };
		AdjustWindowRect(&rect, GetWindowLong(_ctx->hwnd, GWL_STYLE), FALSE);
		SetWindowPos(_ctx->hwnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOZORDER | SWP_NOMOVE);
		ShowWindow(_ctx->hwnd, SW_SHOW);
		UpdateWindow(_ctx->hwnd);

		SetWindowText(_ctx->hwnd, settings.title);

		RAWINPUTDEVICE Rid[2];

		// Keyboard
		Rid[0].usUsagePage = 1;
		Rid[0].usUsage = 6;
		Rid[0].dwFlags = 0;
		Rid[0].hwndTarget = NULL;

		// Mouse
		Rid[1].usUsagePage = 1;
		Rid[1].usUsage = 2;
		Rid[1].dwFlags = 0;
		Rid[1].hwndTarget = NULL;

		RegisterRawInputDevices(Rid, 2, sizeof(RAWINPUTDEVICE));

		for (int i = 0; i < 256; ++i) {
			_ctx->keyState[i] = 0;
		}
		_ctx->mouseButtonState[0] = 0;
		_ctx->mouseButtonState[1] = 0;

		init_random(GetTickCount());

		QueryPerformanceFrequency(&_ctx->timerFrequency);
		QueryPerformanceCounter(&_ctx->lastTime);

		_ctx->leftOverTicks = 0;
		_ctx->framesPerSecond = 0;
		_ctx->framesThisSecond = 0;
		_ctx->secondCounter = 0;
		_ctx->maxDelta = _ctx->timerFrequency.QuadPart / 10;
		for (int i = 0; i < 256; ++i) {
			_ctx->errorBuffer[i] = '\0';
		}
		return initializeDevice(settings);
	}

	// ------------------------------------------------------
	// shutdown
	// ------------------------------------------------------
	void shutdown() {
		if (_ctx != 0) {
			for (size_t i = 0; i < _ctx->_resources.size(); ++i) {
				_ctx->_resources[i]->release();
				delete _ctx->_resources[i];
			}			
			if (_ctx->backBufferTarget) _ctx->backBufferTarget->Release();
			if (_ctx->swapChain) _ctx->swapChain->Release();
			if (_ctx->d3dContext) _ctx->d3dContext->Release();
			if (_ctx->depthStencilView) _ctx->depthStencilView->Release();
			if (_ctx->depthTexture) _ctx->depthTexture->Release();
			if (_ctx->depthDisabledStencilState) _ctx->depthDisabledStencilState->Release();
			if (_ctx->depthEnabledStencilState) _ctx->depthEnabledStencilState->Release();
			if (_ctx->d3dDevice) _ctx->d3dDevice->Release();
			delete _ctx;
		}
	}

	// ------------------------------------------------------
	// get view matrix
	// ------------------------------------------------------
	const matrix& getViewMatrix() {
		return _ctx->viewMatrix;
	}

	// ------------------------------------------------------
	// get projection matrix
	// ------------------------------------------------------
	const matrix& getProjectionMatrix() {
		return _ctx->projectionMatrix;
	}

	// ------------------------------------------------------
	// get view projection matrix
	// ------------------------------------------------------
	const matrix& getViewProjectionMatrix() {
		return _ctx->viewProjectionMatrix;
	}

	// ------------------------------------------------------
	// begin rendering
	// ------------------------------------------------------
	void begin() {
		_ctx->d3dContext->OMSetRenderTargets(1, &_ctx->backBufferTarget, _ctx->depthStencilView);
		_ctx->d3dContext->ClearRenderTargetView(_ctx->backBufferTarget, _ctx->clearColor);
		_ctx->d3dContext->ClearDepthStencilView(_ctx->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		_ctx->d3dContext->OMSetDepthStencilState(_ctx->depthEnabledStencilState, 1);
		_ctx->depthBufferState = DepthBufferState::ENABLED;
		_ctx->selectedShaderId = INVALID_RID;
		_ctx->selectedVertexDeclaration = INVALID_RID;
		_ctx->selectedIndexBuffer = INVALID_RID;
		_ctx->selectedRasterizerState = INVALID_RID;
		_ctx->selectedVertexBuffer = INVALID_RID;
		for (int i = 0; i < 16; ++i) {
			_ctx->selectedVSTextures[i] = NO_RID;
			_ctx->selectedPSTextures[i] = NO_RID;
			_ctx->selectedGSTextures[i] = NO_RID;
		}
		_ctx->stats.indices = 0;
		_ctx->stats.shaders = 0;
		_ctx->stats.textures = 0;
		_ctx->stats.vertices = 0;
	}

	// ------------------------------------------------------
	// end rendering
	// ------------------------------------------------------
	void end() {
		_ctx->swapChain->Present(0, 0);
	}

	static const char* DXBufferAttributeNames[] = {
		"POSITION",
		"COLOR",
		"TEXCOORD",
		"NORMAL"
	};

	struct DXBufferAttributeType {
		BufferAttributeType type;
		DXGI_FORMAT format;
		int size;
		int bytes;
	};

	static const DXBufferAttributeType DXBufferAttributeTypes[] = {
		{ FLOAT, DXGI_FORMAT_R32G32_FLOAT , 2, 8 },
		{ FLOAT, DXGI_FORMAT_R32G32B32_FLOAT , 3, 12},
		{ FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 16 },

	};

	static int find_format(BufferAttributeType type, int size) {
		for (int i = 0; i < 3; ++i) {
			if (DXBufferAttributeTypes[i].type == type && DXBufferAttributeTypes[i].size == size) {
				return i;
			}
		}
		return -1;
	}
	// ------------------------------------------------------
	// input layout / vertex declaration
	// ------------------------------------------------------
	RID createVertexDeclaration(VertexDeclaration* decl, uint8_t num, RID shaderId) {
		D3D11_INPUT_ELEMENT_DESC* descriptors = new D3D11_INPUT_ELEMENT_DESC[num];
		uint32_t index = 0;
		uint32_t counter = 0;
		int si[8] = { 0 };
		for (int i = 0; i < num; ++i) {
			D3D11_INPUT_ELEMENT_DESC& desc = descriptors[i];
			const VertexDeclaration& current = decl[i];
			int fidx = find_format(decl[i].type, decl[i].size);
			if (fidx == -1) {
				return INVALID_RID;
			}
			const DXBufferAttributeType& formatType = DXBufferAttributeTypes[fidx];
			desc.SemanticName = DXBufferAttributeNames[decl[i].attribute];
			desc.SemanticIndex = si[decl[i].attribute];
			desc.Format = formatType.format;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = index;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			index += formatType.bytes;
			si[decl[i].attribute] += 1;
		}		
		uint16_t sidx = getResourceIndex(shaderId, RT_SHADER);
		ShaderResource* sres = (ShaderResource*)_ctx->_resources[sidx];
		Shader* s = sres->get();
		ID3D11InputLayout* layout = 0;
		assert_result(_ctx->d3dDevice->CreateInputLayout(descriptors, num, s->vertexShaderBuffer, s->bufferSize, &layout), "Failed to create input layout");
		InputLayoutResource* res = new InputLayoutResource(layout, index);
		return addResource(res, RT_VERTEX_DECLARATION);
	}

	// ------------------------------------------------------
	// set vertex declaration
	// ------------------------------------------------------
	void setVertexDeclaration(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_VERTEX_DECLARATION);		
		if (_ctx->selectedVertexDeclaration != rid) {
			if (ridx == NO_RID) {
				_ctx->d3dContext->IASetInputLayout(NULL);
			}
			else {				
				InputLayoutResource* res = (InputLayoutResource*)_ctx->_resources[ridx];
				_ctx->d3dContext->IASetInputLayout(res->get());
			}
			_ctx->selectedVertexDeclaration = id_mask(rid);
		}
	}

	// ------------------------------------------------------
	// constant buffer
	// ------------------------------------------------------
	RID createConstantBuffer(int byteWidth) {
		D3D11_BUFFER_DESC constDesc;
		ZeroMemory(&constDesc, sizeof(constDesc));
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = byteWidth;
		constDesc.Usage = D3D11_USAGE_DYNAMIC;
		constDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ID3D11Buffer* buffer = 0;
		assert_result(_ctx->d3dDevice->CreateBuffer(&constDesc, 0, &buffer), "Failed to create constant buffer");	
		ConstantBufferResource* res = new ConstantBufferResource(buffer);
		return addResource(res, RT_CONSTANT_BUFFER);
	}

	// ------------------------------------------------------
	// update constant buffer
	// ------------------------------------------------------
	void updateConstantBuffer(RID rid, void* data, size_t size) {
		uint16_t ridx = getResourceIndex(rid, RT_CONSTANT_BUFFER);
		if ( ridx != NO_RID) {
			ConstantBufferResource* cbr = (ConstantBufferResource*)_ctx->_resources[ridx];
			ID3D11Buffer* buffer = cbr->get();
			D3D11_MAPPED_SUBRESOURCE resource;
			assert_result(_ctx->d3dContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource), "Failed to update constant buffer");
			void* ptr = resource.pData;
			memcpy(ptr, data, size);
			_ctx->d3dContext->Unmap(buffer, 0);
		}
	}

	// ------------------------------------------------------
	// set vertex shader constant buffer
	// ------------------------------------------------------
	void setVertexConstantBuffer(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_CONSTANT_BUFFER);
		if (ridx != NO_RID) {
			ConstantBufferResource* cbr = (ConstantBufferResource*)_ctx->_resources[ridx];
			ID3D11Buffer* buffer = cbr->get();
			_ctx->d3dContext->VSSetConstantBuffers(0, 1, &buffer);
		}
		else {
			_ctx->d3dContext->VSSetConstantBuffers(0, 1, NULL);
		}
	}

	// ------------------------------------------------------
	// set pixel shader constant buffer
	// ------------------------------------------------------
	void setPixelConstantBuffer(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_CONSTANT_BUFFER);
		if (ridx != NO_RID) {
			ConstantBufferResource* cbr = (ConstantBufferResource*)_ctx->_resources[ridx];
			ID3D11Buffer* buffer = cbr->get();
			_ctx->d3dContext->PSSetConstantBuffers(0, 1, &buffer);
		}
		else {
			_ctx->d3dContext->PSSetConstantBuffers(0, 1, NULL);
		}
	}

	// ------------------------------------------------------
	// set geometry shader constant buffer
	// ------------------------------------------------------
	void setGeometryConstantBuffer(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_CONSTANT_BUFFER);
		if (ridx != NO_RID) {
			ConstantBufferResource* cbr = (ConstantBufferResource*)_ctx->_resources[ridx];
			ID3D11Buffer* buffer = cbr->get();
			_ctx->d3dContext->GSSetConstantBuffers(0, 1, &buffer);
		}
		else {
			_ctx->d3dContext->GSSetConstantBuffers(0, 1, NULL);
		}
	}

	const static int INDEX_BUFFER_SIZE[] = {
		sizeof(uint16_t),
		sizeof(uint32_t)
	};

	const static DXGI_FORMAT INDEX_BUFFER_FORMATS[] = {
		DXGI_FORMAT_R16_UINT,
		DXGI_FORMAT_R32_UINT
	};
	// ------------------------------------------------------
	// index buffer with data
	// ------------------------------------------------------
	RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type, void* data) {
		D3D11_BUFFER_DESC bufferDesc;
		if (type == BufferType::DYNAMIC) {
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else {
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
		}
		bufferDesc.ByteWidth = numIndices * INDEX_BUFFER_SIZE[indexType];
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = data;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		ID3D11Buffer* buffer = 0;
		assert_result(_ctx->d3dDevice->CreateBuffer(&bufferDesc, data ? &InitData : NULL, &buffer), "Failed to create index buffer");
		IndexBufferResource* res = new IndexBufferResource(buffer, INDEX_BUFFER_FORMATS[indexType], numIndices, type);
		return addResource(res, RT_INDEX_BUFFER);
	}

	// ------------------------------------------------------
	// index buffer
	// ------------------------------------------------------
	RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type) {
		return createIndexBuffer(numIndices, indexType, type, 0);
	}

	// ------------------------------------------------------
	// set index buffer
	// ------------------------------------------------------
	void setIndexBuffer(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_INDEX_BUFFER);
		if (ridx != _ctx->selectedIndexBuffer) {
			if (ridx != NO_RID) {
				IndexBufferResource* res = (IndexBufferResource*)_ctx->_resources[ridx];
				_ctx->d3dContext->IASetIndexBuffer(res->get(), res->getFormat(), 0);
			}
			else {
				_ctx->d3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
			}
			_ctx->selectedIndexBuffer = ridx;
		}
	}

	// ------------------------------------------------------
	// create a quad index buffer 0, 1, 2, 2, 3, 0
	// ------------------------------------------------------
	RID createQuadIndexBuffer(int numQuads) {
		int size = numQuads * 6;
		uint32_t* data = new uint32_t[size];
		int base = 0;
		int cnt = 0;
		for (int i = 0; i < numQuads; ++i) {
			data[base]     = cnt + 0;
			data[base + 1] = cnt + 1;
			data[base + 2] = cnt + 2;
			data[base + 3] = cnt + 2;
			data[base + 4] = cnt + 3;
			data[base + 5] = cnt + 0;
			base += 6;
			cnt += 4;
		}
		RID rid = createIndexBuffer(size, ds::IndexType::UINT_32, ds::BufferType::STATIC, data);
		delete[] data;
		return rid;
	}

	// ------------------------------------------------------
	// vertex buffer with data
	// ------------------------------------------------------
	RID createVertexBuffer(BufferType type, int numVertices, RID vertexDecl) {
		uint16_t vdidx = getResourceIndex(vertexDecl, RT_VERTEX_DECLARATION);
		InputLayoutResource* iRes = (InputLayoutResource*)_ctx->_resources[vdidx];
		UINT size = numVertices * iRes->size();

		D3D11_BUFFER_DESC bufferDesciption;
		ZeroMemory(&bufferDesciption, sizeof(bufferDesciption));
		if (type == BufferType::DYNAMIC) {
			bufferDesciption.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesciption.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else {
			bufferDesciption.Usage = D3D11_USAGE_DEFAULT;
			bufferDesciption.CPUAccessFlags = 0;
		}
		bufferDesciption.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesciption.ByteWidth = size;

		ID3D11Buffer* buffer = 0;
		assert_result(_ctx->d3dDevice->CreateBuffer(&bufferDesciption, 0, &buffer),"Failed to create vertex buffer");
		VertexBufferResource* res = new VertexBufferResource(buffer, size, type, vertexDecl);
		return addResource(res, RT_VERTEX_BUFFER);
	}

	// ------------------------------------------------------
	// vertex buffer with data
	// ------------------------------------------------------
	RID createVertexBuffer(BufferType type, int numVertices, RID vertexDecl, void* data, unsigned long long vertexSize) {
		uint16_t vdidx = getResourceIndex(vertexDecl, RT_VERTEX_DECLARATION);
		InputLayoutResource* iRes = (InputLayoutResource*)_ctx->_resources[vdidx];
		UINT size = numVertices * iRes->size();

		D3D11_BUFFER_DESC bufferDesciption;
		ZeroMemory(&bufferDesciption, sizeof(bufferDesciption));
		if (type == BufferType::DYNAMIC) {
			bufferDesciption.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesciption.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else {
			bufferDesciption.Usage = D3D11_USAGE_DEFAULT;
			bufferDesciption.CPUAccessFlags = 0;
		}
		bufferDesciption.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesciption.ByteWidth = size;

		ID3D11Buffer* buffer = 0;
		D3D11_SUBRESOURCE_DATA resource;
		resource.pSysMem = data;
		resource.SysMemPitch = 0;
		resource.SysMemSlicePitch = 0;
		assert_result(_ctx->d3dDevice->CreateBuffer(&bufferDesciption, &resource, &buffer),"Failed to create vertex buffer");
		VertexBufferResource* res = new VertexBufferResource(buffer, size, type, vertexDecl);
		return addResource(res, RT_VERTEX_BUFFER);
	}

	// ------------------------------------------------------
	// list of primitive topologies
	// ------------------------------------------------------
	static const D3D11_PRIMITIVE_TOPOLOGY PRIMITIVE_TOPOLOGIES[] = {
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	};

	// ------------------------------------------------------
	// set vertex buffer
	// ------------------------------------------------------
	void setVertexBuffer(RID rid, uint32_t* stride, uint32_t* offset, PrimitiveTypes topology) {
		uint16_t ridx = getResourceIndex(rid, RT_VERTEX_BUFFER);		
		if ( ridx != _ctx->selectedVertexBuffer) {
			if (ridx == NO_RID) {
				_ctx->d3dContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
				_ctx->d3dContext->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGIES[topology]);
			}
			else {
				VertexBufferResource* res = (VertexBufferResource*)_ctx->_resources[ridx];
				ID3D11Buffer* buffer = res->get();
				_ctx->d3dContext->IASetVertexBuffers(0, 1, &buffer, stride, offset);
				_ctx->d3dContext->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGIES[topology]);
			}
			_ctx->selectedVertexBuffer = ridx;
		}
	}

	// ------------------------------------------------------
	// map data to vertex buffer
	// ------------------------------------------------------
	void mapBufferData(RID rid, void* data, uint32_t size) {
		uint16_t ridx = getResourceIndex(rid, RT_VERTEX_BUFFER);
		if (ridx != NO_RID) {
			ConstantBufferResource* cbr = (ConstantBufferResource*)_ctx->_resources[ridx];
			D3D11_MAPPED_SUBRESOURCE resource;
			assert_result(_ctx->d3dContext->Map(cbr->get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource), "Failed to map data");
			void* ptr = resource.pData;
			memcpy(ptr, data, size);
			_ctx->d3dContext->Unmap(cbr->get(), 0);
		}
	}

	// ------------------------------------------------------
	// Sampler State
	// ------------------------------------------------------
	static const D3D11_TEXTURE_ADDRESS_MODE TEXTURE_ADDRESSMODES[] = {
		{ D3D11_TEXTURE_ADDRESS_WRAP },
		{ D3D11_TEXTURE_ADDRESS_MIRROR },
		{ D3D11_TEXTURE_ADDRESS_CLAMP },
		{ D3D11_TEXTURE_ADDRESS_BORDER },
		{ D3D11_TEXTURE_ADDRESS_MIRROR_ONCE },
	};

	static const D3D11_FILTER TEXTURE_FILTERMODES[] = {
		{ D3D11_FILTER_MIN_MAG_MIP_POINT },
		{ D3D11_FILTER_MIN_MAG_MIP_LINEAR },
		{ D3D11_FILTER_ANISOTROPIC },
	};

	// ------------------------------------------------------
	// create sampler state
	// ------------------------------------------------------
	RID createSamplerState(TextureAddressModes addressMode,TextureFilters filter) {
		D3D11_SAMPLER_DESC colorMapDesc;
		ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));

		colorMapDesc.AddressU = TEXTURE_ADDRESSMODES[addressMode];
		colorMapDesc.AddressV = TEXTURE_ADDRESSMODES[addressMode];
		colorMapDesc.AddressW = TEXTURE_ADDRESSMODES[addressMode];

		colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc.Filter = TEXTURE_FILTERMODES[filter];
		colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
		
		ID3D11SamplerState* sampler;
		assert_result(_ctx->d3dDevice->CreateSamplerState(&colorMapDesc, &sampler), "Failed to create SamplerState");
		SamplerStateResource* res = new SamplerStateResource(sampler);
		return addResource(res, RT_SAMPLER_STATE);
	}

	// ------------------------------------------------------
	// set sampler state
	// ------------------------------------------------------
	// FIXME: set by shader type!!
	void setSamplerState(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_SAMPLER_STATE);
		if (ridx != NO_RID) {
			SamplerStateResource* res = (SamplerStateResource*)_ctx->_resources[ridx];
			ID3D11SamplerState* state = res->get();
			_ctx->d3dContext->PSSetSamplers(0, 1, &state);
		}
	}

	// ------------------------------------------------------
	// Blend States
	// ------------------------------------------------------
	static const D3D11_BLEND BLEND_STATEMAPPINGS[] = {
		D3D11_BLEND_ZERO,
		D3D11_BLEND_ONE,
		D3D11_BLEND_SRC_COLOR,
		D3D11_BLEND_INV_SRC_COLOR,
		D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_DEST_ALPHA,
		D3D11_BLEND_INV_DEST_ALPHA,
		D3D11_BLEND_DEST_COLOR,
		D3D11_BLEND_INV_DEST_COLOR,
		D3D11_BLEND_SRC_ALPHA_SAT,
		D3D11_BLEND_BLEND_FACTOR,
		D3D11_BLEND_INV_BLEND_FACTOR,
		D3D11_BLEND_SRC1_COLOR,
		D3D11_BLEND_INV_SRC1_COLOR,
		D3D11_BLEND_SRC1_ALPHA,
		D3D11_BLEND_INV_SRC1_ALPHA
	};

	// ------------------------------------------------------
	// create alpha enabled blend state
	// ------------------------------------------------------
	RID createAlphaBlendState(BlendStates srcBlend, BlendStates destBlend) {
		return createBlendState(srcBlend, srcBlend, destBlend, destBlend, true);
	}

	// ------------------------------------------------------
	// cretae blend state
	// ------------------------------------------------------
	RID createBlendState(BlendStates srcBlend, BlendStates srcAlphaBlend, BlendStates destBlend, BlendStates destAlphaBlend, bool alphaEnabled) {
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		if (alphaEnabled) {
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
		}
		else {
			blendDesc.RenderTarget[0].BlendEnable = FALSE;
			//blendDesc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
		}
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = BLEND_STATEMAPPINGS[srcBlend];
		blendDesc.RenderTarget[0].DestBlend = BLEND_STATEMAPPINGS[destBlend];
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = BLEND_STATEMAPPINGS[srcAlphaBlend];
		blendDesc.RenderTarget[0].DestBlendAlpha = BLEND_STATEMAPPINGS[destAlphaBlend];
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

		ID3D11BlendState* state;
		assert_result(_ctx->d3dDevice->CreateBlendState(&blendDesc, &state), "Failed to create blendstate");
		BlendStateResource* res = new BlendStateResource(state);
		return addResource(res, RT_BLENDSTATE);
	}

	// ------------------------------------------------------
	// set blend state
	// ------------------------------------------------------
	void setBlendState(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_BLENDSTATE);
		if (ridx != NO_RID) {
			BlendStateResource* res = (BlendStateResource*)_ctx->_resources[ridx];
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			_ctx->d3dContext->OMSetBlendState(res->get(), blendFactor, 0xFFFFFFFF);
		}
	}

	// ------------------------------------------------------
	// set blend state with factor and mask
	// ------------------------------------------------------
	void setBlendState(RID rid, float* blendFactor,uint32_t mask) {
		uint16_t ridx = getResourceIndex(rid, RT_BLENDSTATE);
		if (ridx != NO_RID) {
			BlendStateResource* res = (BlendStateResource*)_ctx->_resources[ridx];
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			_ctx->d3dContext->OMSetBlendState(res->get(), blendFactor, mask);
		}
	}

	// ------------------------------------------------------
	// Draw indexed
	// ------------------------------------------------------
	void drawIndexed(uint32_t num) {
		_ctx->d3dContext->DrawIndexed(num, 0, 0);
	}

	// ------------------------------------------------------
	// draw
	// ------------------------------------------------------
	void draw(uint32_t num) {
		
		_ctx->d3dContext->Draw(num, 0);
	}

	// ------------------------------------------------------
	// create shader
	// ------------------------------------------------------
	RID createShader() {
		Shader* s = new Shader;
		ShaderResource* res = new ShaderResource(s);
		return addResource(res, RT_SHADER);
	}

	struct DataFile {
		char* data;
		int size;
	};

	static DataFile read_data(const char* name) {
		DataFile df;
		df.data = 0;
		df.size = -1;
		FILE *fp = fopen(name, "rb");
		XASSERT(fp != 0, "Cannot find file: '%s'", name);
		if (fp) {			
			fseek(fp, 0, SEEK_END);
			int sz = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			df.data = new char[sz + 1];
			fread(df.data, 1, sz, fp);
			df.data[sz] = '\0';
			fclose(fp);
			df.size = sz;
		}
		return df;
	}

	// ------------------------------------------------------
	// load vertex shader
	// ------------------------------------------------------
	void loadVertexShader(RID shader, const char* csoName) {
		uint16_t ridx = getResourceIndex(shader, RT_SHADER);
		if (ridx != NO_RID) {
			DataFile file = read_data(csoName);
			XASSERT(file.size != -1, "Cannot load vertex shader file: '%s'", csoName);
			ShaderResource* res = (ShaderResource*)_ctx->_resources[ridx];
			Shader* s = res->get();
			assert_result(_ctx->d3dDevice->CreateVertexShader(
				file.data,
				file.size,
				nullptr,
				&s->vertexShader), "Failed to create vertex shader");
			s->vertexShaderBuffer = new char[file.size];
			memcpy(s->vertexShaderBuffer, file.data, file.size);
			s->bufferSize = file.size;
			delete[] file.data;
		}
	}

	// ------------------------------------------------------
	// load pixel shader
	// ------------------------------------------------------
	void loadPixelShader(RID shader, const char* csoName) {
		uint16_t ridx = getResourceIndex(shader, RT_SHADER);
		if (ridx != NO_RID) {
			DataFile file = read_data(csoName);
			XASSERT(file.size != -1, "Cannot load pixel shader file: '%s'", csoName);
			ShaderResource* res = (ShaderResource*)_ctx->_resources[ridx];
			Shader* s = res->get();
			assert_result(_ctx->d3dDevice->CreatePixelShader(
				file.data,
				file.size,
				nullptr,
				&s->pixelShader), "Failed to create pixel shader");
			delete[] file.data;
		}
	}

	// ------------------------------------------------------
	// load pixel shader
	// ------------------------------------------------------
	void loadGeometryShader(RID shader, const char* csoName) {
		uint16_t ridx = getResourceIndex(shader, RT_SHADER);
		if (ridx != NO_RID) {
			DataFile file = read_data(csoName);
			XASSERT(file.size != -1, "Cannot load geometry shader file: '%s'", csoName);
			ShaderResource* res = (ShaderResource*)_ctx->_resources[ridx];
			Shader* s = res->get();
			assert_result(_ctx->d3dDevice->CreateGeometryShader(
				file.data,
				file.size,
				nullptr,
				&s->geometryShader
			), "Failed to create geometry shader");
			delete[] file.data;
		}
	}

	// ------------------------------------------------------
	// set shader
	// ------------------------------------------------------
	void setShader(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_SHADER);
		if (ridx != _ctx->selectedShaderId) {
			ShaderResource* res = (ShaderResource*)_ctx->_resources[ridx];
			Shader* s = res->get();
			// FIXME: handle NO_RID
			if (s->vertexShader != 0) {
				_ctx->d3dContext->VSSetShader(s->vertexShader, 0, 0);
			}
			else {
				_ctx->d3dContext->VSSetShader(NULL, NULL, 0);
			}
			if (s->pixelShader != 0) {
				_ctx->d3dContext->PSSetShader(s->pixelShader, 0, 0);
			}
			else {
				_ctx->d3dContext->PSSetShader(NULL, NULL, 0);
			}
			if (s->geometryShader != 0) {
				_ctx->d3dContext->GSSetShader(s->geometryShader, 0, 0);
			}
			else {
				_ctx->d3dContext->GSSetShader(NULL, NULL, 0);
			}
			_ctx->selectedShaderId = ridx;
		}
	}

	const static DXGI_FORMAT TEXTURE_FOMATS[] = {
		DXGI_FORMAT_R8_UINT,
		DXGI_FORMAT_R8G8B8A8_UINT,
		DXGI_FORMAT_R8G8B8A8_UNORM
	};
	// ------------------------------------------------------
	// create texture
	// ------------------------------------------------------
	RID createTexture(int width, int height, uint8_t channels, void* data, TextureFormat format) {
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = TEXTURE_FOMATS[format];
		desc.SampleDesc.Count = 1;// _ctx->multisampling;
		desc.SampleDesc.Quality = 0;		
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		ID3D11Texture2D *texture2D = 0;
		/*
		if (_ctx->multisampling > 1) {
			desc.Usage = D3D11_USAGE_DEFAULT;
			ASSERT_RESULT(_ctx->d3dDevice->CreateTexture2D(&desc, 0, &texture2D), "Failed to create Texture2D");
			D3D11_MAPPED_SUBRESOURCE resource;
			ASSERT_RESULT(_ctx->d3dContext->Map(texture2D, 0, D3D11_MAP_WRITE, 0, &resource), "Failed to map data");
			void* ptr = resource.pData;
			memcpy(ptr, data, width * height * channels);
			_ctx->d3dContext->Unmap(texture2D, 0);
		}
		else {
		*/
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			D3D11_SUBRESOURCE_DATA subres;
			subres.pSysMem = data;
			subres.SysMemPitch = width * channels;
			subres.SysMemSlicePitch = 0;			
			ASSERT_RESULT(_ctx->d3dDevice->CreateTexture2D(&desc, &subres, &texture2D), "Failed to create Texture2D");
		//}
		ID3D11ShaderResourceView* srv = 0;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		//if (_ctx->multisampling > 1) {
			//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		//}
		//else {
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//}
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Format = TEXTURE_FOMATS[format];

		ASSERT_RESULT(_ctx->d3dDevice->CreateShaderResourceView(texture2D, &srvDesc, &srv), "Failed to create resource view");

		InternalTexture tex;
		tex.width = width;
		tex.height = height;
		tex.srv = srv;
		ShaderResourceViewResource* res = new ShaderResourceViewResource(srv);
		return addResource(res, RT_SRV);
	}

	// ------------------------------------------------------
	// set texture
	// ------------------------------------------------------
	void setTexture(RID rid, ShaderType type) {
		setTexture(rid, type, 0);
	}

	// ------------------------------------------------------
	// set texture
	// ------------------------------------------------------
	void setTexture(RID rid, ShaderType type, uint8_t slot) {
		uint16_t ridx = getResourceIndex(rid, RT_SRV);
		XASSERT(_ctx->selectedShaderId != INVALID_RID, "Invalid or no shader selected");
		ShaderResource* sRes = (ShaderResource*)_ctx->_resources[_ctx->selectedShaderId];
		Shader* s = sRes->get();
		ID3D11ShaderResourceView* srv = 0;
		if (ridx != NO_RID) {
			ShaderResourceViewResource* res = (ShaderResourceViewResource*)_ctx->_resources[ridx];
			srv = res->get();
		}
		if (type == ShaderType::PIXEL) {
			XASSERT(s->pixelShader != 0, "No pixel shader selected");
			if (_ctx->selectedPSTextures[slot] != id_mask(rid)) {
				_ctx->d3dContext->PSSetShaderResources(slot, 1, &srv);
				_ctx->selectedPSTextures[slot] = id_mask(rid);
			}
		}
		else if (type == ShaderType::VERTEX) {
			XASSERT(s->vertexShader != 0, "No vertex shader selected");
			if (_ctx->selectedVSTextures[slot] != id_mask(rid)) {
				_ctx->d3dContext->VSSetShaderResources(slot, 1, &srv);
				_ctx->selectedVSTextures[slot] = id_mask(rid);
			}
		}
		else if (type == ShaderType::GEOMETRY) {
			XASSERT(s->geometryShader != 0, "No geometry shader selected");
			if (_ctx->selectedGSTextures[slot] != id_mask(rid)) {
				_ctx->d3dContext->GSSetShaderResources(slot, 1, &srv);
				_ctx->selectedGSTextures[slot] = id_mask(rid);
			}
		}
	}

	void setTextureFromRenderTarget(RID rtID, ShaderType type, uint8_t slot) {
		uint16_t ridx = getResourceIndex(rtID, RT_RENDER_TARGET);
		RenderTargetResource* res = (RenderTargetResource*)_ctx->_resources[ridx];
		ShaderResource* sRes = (ShaderResource*)_ctx->_resources[_ctx->selectedShaderId];
		Shader* s = sRes->get();
		if (type == ShaderType::PIXEL) {
			XASSERT(s->pixelShader != 0, "No pixel shader selected");
			if (_ctx->selectedPSTextures[slot] != ridx) {
				_ctx->d3dContext->PSSetShaderResources(slot, 1, &res->get()->srv);
				_ctx->selectedPSTextures[slot] = ridx;
			}
		}
		else if (type == ShaderType::VERTEX) {
			XASSERT(s->vertexShader != 0, "No vertex shader selected");
			if (_ctx->selectedVSTextures[slot] != ridx) {
				_ctx->d3dContext->VSSetShaderResources(slot, 1, &res->get()->srv);
				_ctx->selectedVSTextures[slot] = ridx;
			}
		}
		else if (type == ShaderType::GEOMETRY) {
			XASSERT(s->geometryShader != 0, "No geometry shader selected");
			if (_ctx->selectedGSTextures[slot] != ridx) {
				_ctx->d3dContext->GSSetShaderResources(slot, 1, &res->get()->srv);
				_ctx->selectedGSTextures[slot] = ridx;
			}
		}
	}

	// ------------------------------------------------------
	// set depth buffer state
	// ------------------------------------------------------
	void setDepthBufferState(DepthBufferState state) {
		if (_ctx->depthBufferState != state) {
			if (state == DepthBufferState::ENABLED) {
				_ctx->d3dContext->OMSetDepthStencilState(_ctx->depthEnabledStencilState, 1);
			}
			else {
				_ctx->d3dContext->OMSetDepthStencilState(_ctx->depthDisabledStencilState, 1);
			}
		}
		_ctx->depthBufferState = state;
	}

	// ------------------------------------------------------
	// create rasterizer state
	// ------------------------------------------------------
	RID createRasterizerState(CullMode cullMode,FillMode fillMode, bool multiSample, bool scissor, float depthBias, float slopeDepthBias) {
		D3D11_RASTERIZER_DESC desc;
		desc.CullMode = (D3D11_CULL_MODE)cullMode;
		desc.FillMode = (D3D11_FILL_MODE)fillMode;
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = (INT)depthBias;
		desc.DepthBiasClamp = 0.0f;
		desc.SlopeScaledDepthBias = slopeDepthBias;
		desc.AntialiasedLineEnable = FALSE;
		desc.DepthClipEnable = TRUE;
		desc.MultisampleEnable = (BOOL)multiSample;
		desc.ScissorEnable = (BOOL)scissor;
		ID3D11RasterizerState* state = 0;
		assert_result(_ctx->d3dDevice->CreateRasterizerState(&desc, &state), "Failed to create rasterizer state");
		RasterizerStateResource* res = new RasterizerStateResource(state);
		return addResource(res, RT_RASTERIZER_STATE);
	}

	// ------------------------------------------------------
	// set rasterizer state
	// ------------------------------------------------------
	void setRasterizerState(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_RASTERIZER_STATE);
		if ( ridx != NO_RID) {
			RasterizerStateResource* res = (RasterizerStateResource*)_ctx->_resources[ridx];
			_ctx->d3dContext->RSSetState(res->get());
		}
	}

	// ------------------------------------------------------
	// create render target
	// ------------------------------------------------------
	RID createRenderTarget(uint16_t width, uint16_t height) {
		RenderTarget* rt = new RenderTarget;
		
		// Initialize the render target texture description.
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		assert_result(_ctx->d3dDevice->CreateTexture2D(&textureDesc, NULL, &rt->texture), "Failed to create texture for rendertarget");
		// Setup the description of the render target view.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		// Create the render target view.
		assert_result(_ctx->d3dDevice->CreateRenderTargetView(rt->texture, &renderTargetViewDesc, &rt->view),"Failed to create render target view");
		// Setup the description of the shader resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		assert_result(_ctx->d3dDevice->CreateShaderResourceView(rt->texture, &shaderResourceViewDesc, &rt->srv),"Failed to create shader resource view");

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = height;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;

		assert_result(_ctx->d3dDevice->CreateTexture2D(&depthTexDesc, 0, &rt->depthTexture),"Failed to create depth texture");
		
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		descDSV.Texture2D.MipSlice = 0;

		assert_result(_ctx->d3dDevice->CreateDepthStencilView(rt->depthTexture, &descDSV, &rt->depthStencilView),"Failed to create depth stencil view");
		RenderTargetResource* res = new RenderTargetResource(rt);
		return addResource(res,RT_RENDER_TARGET);
	}

	void setRenderTarget(RID rtID) {
		uint16_t ridx = getResourceIndex(rtID, RT_RENDER_TARGET);
		if (ridx == NO_RID) {
			_ctx->d3dContext->OMSetRenderTargets(1, &_ctx->backBufferTarget, _ctx->depthStencilView);
		}
		else {
			RenderTargetResource* res = (RenderTargetResource*)_ctx->_resources[ridx];
			RenderTarget* rt = res->get();
			_ctx->d3dContext->OMSetRenderTargets(1, &rt->view, rt->depthStencilView);
			// Clear the back buffer.
			//_ctx->d3dContext->ClearRenderTargetView(rt->view, _descriptor.clearColor);
			// Clear the depth buffer.
			_ctx->d3dContext->ClearDepthStencilView(rt->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
	}

	void restoreBackBuffer() {
		_ctx->d3dContext->OMSetRenderTargets(1, &_ctx->backBufferTarget, _ctx->depthStencilView);
	}
}

matrix mat_identity() {
	matrix m(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	return m;
}

matrix mat_OrthoLH(float w, float h, float zn, float zf) {
	// msdn.microsoft.com/de-de/library/windows/desktop/bb204940(v=vs.85).aspx
	matrix tmp = mat_identity();
	tmp._11 = 2.0f / w;
	tmp._22 = 2.0f / h;
	tmp._33 = 1.0f / (zf - zn);
	tmp._43 = zn / (zn - zf);
	return tmp;
}

matrix operator * (const matrix& m1, const matrix& m2) {
	matrix tmp;
	tmp._11 = m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31 + m1._14 * m2._41;
	tmp._12 = m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32 + m1._14 * m2._42;
	tmp._13 = m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33 + m1._14 * m2._43;
	tmp._14 = m1._11 * m2._14 + m1._12 * m2._24 + m1._13 * m2._34 + m1._14 * m2._44;

	tmp._21 = m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31 + m1._24 * m2._41;
	tmp._22 = m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32 + m1._24 * m2._42;
	tmp._23 = m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33 + m1._24 * m2._43;
	tmp._24 = m1._21 * m2._14 + m1._22 * m2._24 + m1._23 * m2._34 + m1._24 * m2._44;

	tmp._31 = m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31 + m1._34 * m2._41;
	tmp._32 = m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32 + m1._34 * m2._42;
	tmp._33 = m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33 + m1._34 * m2._43;
	tmp._34 = m1._31 * m2._14 + m1._32 * m2._24 + m1._33 * m2._34 + m1._34 * m2._44;

	tmp._41 = m1._41 * m2._11 + m1._42 * m2._21 + m1._43 * m2._31 + m1._44 * m2._41;
	tmp._42 = m1._41 * m2._12 + m1._42 * m2._22 + m1._43 * m2._32 + m1._44 * m2._42;
	tmp._43 = m1._41 * m2._13 + m1._42 * m2._23 + m1._43 * m2._33 + m1._44 * m2._43;
	tmp._44 = m1._41 * m2._14 + m1._42 * m2._24 + m1._43 * m2._34 + m1._44 * m2._44;

	return tmp;
}

// -------------------------------------------------------
// Scale matrix
// -------------------------------------------------------
matrix mat_Scale(const v3& scale) {
	matrix sm(
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	return sm;
}

// http://www.cprogramming.com/tutorial/3d/rotationMatrices.html
// left hand sided
matrix mat_RotationX(float angle) {
	matrix sm(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(angle), -sin(angle), 0.0f,
		0.0f, sin(angle), cos(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	return sm;
}

matrix mat_RotationY(float angle) {
	matrix sm(
		cos(angle), 0.0f, sin(angle), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(angle), 0.0f, cos(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	return sm;
}
// FIXME: wrong direction!!!!
matrix mat_RotationZ(float angle) {
	matrix sm(
		cos(angle), -sin(angle), 0.0f, 0.0f,
		sin(angle), cos(angle), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	return sm;
}

matrix mat_Rotation(const v3& r) {
	return mat_RotationZ(r.z) * mat_RotationY(r.y) * mat_RotationX(r.x);
}

// -------------------------------------------------------
// Transpose matrix
// -------------------------------------------------------
matrix mat_Transpose(const matrix& m) {
	matrix current = m;
	matrix tmp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp.m[i][j] = current.m[j][i];
		}
	}
	return tmp;
}

// -------------------------------------------------------
// Translation matrix
// -------------------------------------------------------
matrix mat_Translate(const v3& pos) {
	matrix tm(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
	);
	return tm;
}

matrix mat_LookAtLH(const v3& eye, const v3& lookAt, const v3& up) {
	// see msdn.microsoft.com/de-de/library/windows/desktop/bb205342(v=vs.85).aspx
	v3 zAxis = normalize(lookAt - eye);
	v3 xAxis = normalize(cross(up, zAxis));
	v3 yAxis = cross(zAxis, xAxis);
	float dox = -dot(xAxis, eye);
	float doy = -dot(yAxis, eye);
	float doz = -dot(zAxis, eye);
	matrix tmp(
		xAxis.x, yAxis.x, zAxis.x, 0.0f,
		xAxis.y, yAxis.y, zAxis.y, 0.0f,
		xAxis.z, yAxis.z, zAxis.z, 0.0f,
		dox, doy, doz, 1.0f
	);
	return tmp;
}

matrix mat_PerspectiveFovLH(float fovy, float aspect, float zn, float zf) {
	// msdn.microsoft.com/de-de/library/windows/desktop/bb205350(v=vs.85).aspx
	float yScale = 1.0f / tan(fovy / 2.0f);
	float xScale = yScale / aspect;

	matrix tmp(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, zf / (zf - zn), 1.0f,
		0.0f, 0.0f, -zn*zf / (zf - zn), 0.0f
	);
	return tmp;
}

v3 mat_TransformNormal(const v3& v, const matrix& m) {
	v3 result =
		v3(v.x * m._11 + v.y * m._21 + v.z * m._31,
			v.x * m._12 + v.y * m._22 + v.z * m._32,
			v.x * m._13 + v.y * m._23 + v.z * m._33);
	return result;
}

matrix mat_Rotation(const v3& v, float angle) {
	float L = (v.x * v.x + v.y * v.y + v.z * v.z);
	float u2 = v.x * v.x;
	float v2 = v.y * v.y;
	float w2 = v.z * v.z;
	matrix tmp = mat_identity();
	tmp._11 = (u2 + (v2 + w2) * cos(angle)) / L;
	tmp._12 = (v.x * v.y * (1 - cos(angle)) - v.z * sqrt(L) * sin(angle)) / L;
	tmp._13 = (v.x * v.z * (1 - cos(angle)) + v.y * sqrt(L) * sin(angle)) / L;
	tmp._14 = 0.0f;

	tmp._21 = (v.x * v.y * (1 - cos(angle)) + v.z * sqrt(L) * sin(angle)) / L;
	tmp._22 = (v2 + (u2 + w2) * cos(angle)) / L;
	tmp._23 = (v.y * v.z * (1 - cos(angle)) - v.x * sqrt(L) * sin(angle)) / L;
	tmp._24 = 0.0f;

	tmp._31 = (v.x * v.z * (1 - cos(angle)) - v.y * sqrt(L) * sin(angle)) / L;
	tmp._32 = (v.y * v.z * (1 - cos(angle)) + v.x * sqrt(L) * sin(angle)) / L;
	tmp._33 = (w2 + (u2 + v2) * cos(angle)) / L;
	tmp._34 = 0.0f;

	return tmp;
}

v4 operator * (const matrix& m, const v4& v) {
	// column mode
	/*
	Vector4f tmp;
	tmp.x = m._11 * v.x + m._12 * v.y + m._13 * v.z + m._14 * v.w;
	tmp.y = m._21 * v.x + m._22 * v.y + m._23 * v.z + m._24 * v.w;
	tmp.z = m._31 * v.x + m._32 * v.y + m._33 * v.z + m._34 * v.w;
	tmp.w = m._41 * v.x + m._42 * v.y + m._43 * v.z + m._44 * v.w;
	return tmp;
	*/
	// row mode
	v4 tmp;
	tmp.x = m._11 * v.x + m._21 * v.y + m._31 * v.z + m._41 * v.w;
	tmp.y = m._12 * v.x + m._22 * v.y + m._32 * v.z + m._42 * v.w;
	tmp.z = m._13 * v.x + m._23 * v.y + m._33 * v.z + m._43 * v.w;
	tmp.w = m._14 * v.x + m._24 * v.y + m._34 * v.z + m._44 * v.w;
	return tmp;
}

v3 operator * (const matrix& m, const v3& v) {
	v4 nv(v.x, v.y, v.z, 1.0f);
	v4 tmp = m * nv;
	return v3(tmp.x, tmp.y, tmp.z);
}
#endif
