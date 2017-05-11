#pragma once
#include <stdint.h>
#include <stdio.h>

/**
* @file diesel.h
*/
// ------------------------------------------------------------------------------------
// MIT License
// 
// Copyright(c) 2017 Andreas Mecky (amecky@gmail.com)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------
// Change log:
// 
// 0.1 - initial release
// ------------------------------------------------------------------------------------

//#define DS_IMPLEMENTATION

typedef unsigned char BYTE;
typedef unsigned char byte;
// ----------------------------------------------------
// RID - resource identifier
//
// stage: 4 // pipeline stage
// type : 6 // resource type
// slot : 6 // 64 slots
// id : 16 // the internal id / index of the resource
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

namespace ds {

#ifndef DS_VERSION
	#define VERSION "0.1"
#endif

	const uint32_t FNV_Prime = 0x01000193; //   16777619
	const uint32_t FNV_Seed = 0x811C9DC5; // 2166136261

	inline uint32_t fnv1a(const char* text, uint32_t hash = FNV_Seed) {
		const unsigned char* ptr = (const unsigned char*)text;
		while (*ptr) {
			hash = (*ptr++ ^ hash) * FNV_Prime;
		}
		return hash;
	}
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
	typedef Vector<2, float> vec2;
	typedef Vector<3, float> vec3;
	typedef Vector<4, float> vec4;

	const vec2 vec2_RIGHT = vec2(1, 0);
	const vec2 vec2_LEFT = vec2(-1, 0);
	const vec2 vec2_UP = vec2(0, 1);
	const vec2 vec2_DOWN = vec2(0, -1);
	const vec2 vec2_ZERO = vec2(0, 0);
	const vec2 vec2_ONE = vec2(1, 1);

	const vec3 vec3_RIGHT = vec3(1, 0, 0);
	const vec3 vec3_LEFT = vec3(-1, 0, 0);
	const vec3 vec3_UP = vec3(0, 1, 0);
	const vec3 vec3_DOWN = vec3(0, -1, 0);
	const vec3 vec3_FORWARD = vec3(0, 0, -1);
	const vec3 vec3_BACKWARD = vec3(0, 0, 1);
	const vec3 vec3_ZERO = vec3(0, 0, 0);
	const vec3 vec3_ONE = vec3(1, 1, 1);

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
		matrix(const float* other) {
			for (int y = 0; y < 4; ++y) {
				for (int x = 0; x < 4; ++x) {
					m[x][y] = other[x + y * 4];
				}
			}
		}
		operator float *() const { return (float *)&_11; }

		float& operator () (int a, int b) {
			return m[a][b];
		}
	};

	matrix matIdentity();

	matrix matOrthoLH(float w, float h, float zn, float zf);

	matrix matScale(const vec3& scale);

	matrix matRotationX(float angle);

	matrix matRotationY(float angle);

	matrix matRotationZ(float angle);

	matrix matRotation(const vec3& r);

	matrix matTranspose(const matrix& m);

	matrix matTranslate(const vec3& pos);

	matrix matLookAtLH(const vec3& eye, const vec3& lookAt, const vec3& up);

	matrix matPerspectiveFovLH(float fovy, float aspect, float zn, float zf);

	vec3 matTransformNormal(const vec3& v, const matrix& m);

	matrix matRotation(const vec3& v, float angle);

	matrix matInverse(const matrix& m);

	matrix operator * (const matrix& m1, const matrix& m2);

	vec3 operator * (const matrix& m, const vec3& v);

	vec4 operator * (const matrix& m, const vec4& v);

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
	T inline cross(const Vector<2, T>& v1, const Vector<2, T>& vec2) {
		return v1.x * vec2.y - vec2.x * v1.y;
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

	// **********************************************************************
	//
	// The rendering API
	//
	// **********************************************************************


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
		/*
		uint32_t u32() {
			uint32_t u = r * 255.0f;
			u = (u << 8) + g * 255.0f;
			u = (u << 8) + b * 255.0f;
			u = (u << 8) + a * 255.0f;
			return u;
		}
		*/

	} Color;



	enum BufferAttribute {
		POSITION,
		COLOR,
		TEXCOORD,
		NORMAL,
		TANGENT,
		BINORMAL
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

	struct InputLayoutDefinition {
		BufferAttribute attribute;
		BufferAttributeType type;
		uint8_t size;
	};

	struct InstancedInputLayoutDefinition {
		const char* name;
		uint8_t nameIndex;
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
		R8G8B8A8_UNORM,
		R8G8B8A8_UNORM_SRGB
	};

	// ---------------------------------------------------
	// The two valid formats of an index buffer
	// ---------------------------------------------------
	enum IndexType {
		UINT_16,
		UINT_32
	};

	// ---------------------------------------------------
	// cull mode
	// ---------------------------------------------------
	enum CullMode {
		NONE = 1,
		FRONT = 2,
		BACK = 3
	};

	enum BindFlag {
		BF_VERTEX_BUFFER = 0x1L,
		BF_INDEX_BUFFER = 0x2L,
		BF_CONSTANT_BUFFER = 0x4L,
		BF_SHADER_RESOURCE = 0x8L,
		BF_STREAM_OUTPUT = 0x10L,
		BF_RENDER_TARGET = 0x20L,
		BF_DEPTH_STENCIL = 0x40L,
		BF_UNORDERED_ACCESS = 0x80L,
		BF_DECODER = 0x200L,
		BF_VIDEO_ENCODER = 0x400L
	};
	
	// ---------------------------------------------------
	// fill mode
	// ---------------------------------------------------
	enum FillMode {
		WIREFRAME = 2,
		SOLID = 3
	};

	enum DrawType {
		DT_VERTICES,
		DT_INDEXED,
		DT_INSTANCED,
		DT_INDEXED_INSTANCED
	};

	enum ResourceType {
		RT_NONE,
		RT_INPUT_LAYOUT,
		RT_CONSTANT_BUFFER,
		RT_INDEX_BUFFER,
		RT_VERTEX_BUFFER,
		RT_SAMPLER_STATE,
		RT_BLENDSTATE,
		RT_SRV,
		RT_RASTERIZER_STATE,
		RT_INSTANCED_VERTEX_BUFFER,
		RT_VERTEX_SHADER,
		RT_GEOMETRY_SHADER,
		RT_PIXEL_SHADER,
		RT_RENDER_TARGET,
		RT_RENDER_PASS,
		RT_DRAW_ITEM,
		RT_STATE_GROUP,
		RT_TEXTURE_FROM_RT,
		RT_COMPUTE_SHADER,
		RT_STRUCTURED_BUFFER,
		RT_UA_SRV,
		RT_CS_GROUP
	};
	
	// ---------------------------------------------------
	// Render settings 
	// ---------------------------------------------------
	struct RenderSettings  {
		uint16_t width;
		uint16_t height;
		Color clearColor;
		uint8_t multisampling;
		const char* title;
		bool useGPUProfiling;
		bool supportDebug;

		RenderSettings() {
			width = 1024;
			height = 768;
			clearColor = Color(0, 0, 0, 255);
			multisampling = 4;
			title = "No title";
			useGPUProfiling = false;
			supportDebug = true;
		}
	};

	struct Camera {
		matrix viewMatrix;
		matrix projectionMatrix;
		matrix viewProjectionMatrix;
		ds::vec3 position;
		ds::vec3 target;
		ds::vec3 up;
		ds::vec3 right;
		float roll;
		float pitch;
		float yaw;

	};
	
	// ---------------------------------------------------
	// Draw Command
	// ---------------------------------------------------
	struct DrawCommand {
		uint32_t size;
		DrawType drawType;
		PrimitiveTypes topology;
		uint32_t instances;
	};

	class PipelineState;
	enum ResourceType;
	enum PipelineStage;

	// ---------------------------------------------------
	// State group
	// ---------------------------------------------------	
	struct StateGroup {
		RID* items;
		int num;
		RID rid;
	};

	static void apply(PipelineState* state, RID groupID);
	// ---------------------------------------------------
	// State group
	// ---------------------------------------------------	
	class BasicGroupBuilder {

	public:
		BasicGroupBuilder() : _num(0), _total(0), _items(0) {}
		virtual ~BasicGroupBuilder() {
			if (_items != 0) {
				delete[] _items;
			}
		}
	protected:
		void add(uint16_t index, ResourceType type, int stage, int slot = 0);
		void basicBinding(RID rid, ResourceType rt, int stage, int slot = 0);
		int _num;
		int _total;
		RID* _items;
	};

	class StateGroupBuilder : public BasicGroupBuilder {

	public:
		StateGroupBuilder() : BasicGroupBuilder() {}
		virtual ~StateGroupBuilder() {}
		StateGroupBuilder& inputLayout(RID rid);
		StateGroupBuilder& constantBuffer(RID rid, RID shader, int slot = 0);
		StateGroupBuilder& structuredBuffer(RID rid, RID shader, int slot = 0);
		StateGroupBuilder& basicConstantBuffer(RID shader, int slot = 0);
		StateGroupBuilder& blendState(RID rid);
		StateGroupBuilder& samplerState(RID rid, RID shader);
		StateGroupBuilder& vertexBuffer(RID rid);
		StateGroupBuilder& instancedVertexBuffer(RID rid);
		StateGroupBuilder& vertexShader(RID rid);
		StateGroupBuilder& geometryShader(RID rid);
		StateGroupBuilder& pixelShader(RID rid);
		StateGroupBuilder& indexBuffer(RID rid);
		StateGroupBuilder& texture(RID rid, RID shader, int slot);
		StateGroupBuilder& textureFromRenderTarget(RID rtID, RID shader, int slot);
		StateGroupBuilder& rasterizerState(RID rid);
		StateGroupBuilder& noResource(ResourceType type, int stage, int slot);
		RID build(const char* name = "StateGroup");
	private:
		int partition(RID* a, int l, int r);
		void quickSort(RID* a, int l, int r);
	};

	// ---------------------------------------------------
	// ComputeShaderBuilder
	// ---------------------------------------------------	
	struct ComputeShaderGroup {
		RID* items;
		int num;
		RID rid;
	};

	class ComputeShaderBuilder : public BasicGroupBuilder {

	public:
		ComputeShaderBuilder() : BasicGroupBuilder() {}
		virtual ~ComputeShaderBuilder() {}
		ComputeShaderBuilder& shader(RID shaderID);
		ComputeShaderBuilder& input(RID srv, int slot);
		ComputeShaderBuilder& output(RID srv, int slot);
		ComputeShaderBuilder& constantBuffer(RID buffer, int slot);
		RID build(const char* name = "ComputeShader");
	};

	class StaticHash {

	public:
		StaticHash();
		explicit StaticHash(const char* text);
		explicit StaticHash(uint32_t hash);
		~StaticHash() {}
		const uint32_t get() const;
		const bool operator<(const StaticHash &rhs) const;
	private:
		uint32_t _hash;
	};
	
	// ---------------------------------------------------
	// Draw Item
	// ---------------------------------------------------
	struct DrawItem {
		DrawCommand command;
		RID* groups;
		int nameIndex;
		int num;
	};

	struct RenderPass {
		Camera* camera;
		RID rts[4];
		int numRenderTargets;
		DepthBufferState depthState;
	};

	struct RenderPassInfo {
		Camera* camera;
		// FIXME: RID depthBuffer
		DepthBufferState depthBufferState;
		RID* renderTargets;
		int numRenderTargets;
	};
	
	RID createRenderPass(const RenderPassInfo& info, const char* name = "RenderPass");

	RID compile(const DrawCommand cmd, RID* groups, int num, const char* name = "UNKNOWN");
	
	RID compile(const DrawCommand cmd, RID group, const char* name = "UNKNOWN");

	void submit(RID renderPass, RID drawItemID, int numElements = -1);
	
	bool init(const RenderSettings& settings);

	// vertex declaration / buffer input layout
	struct InputLayoutInfo {
		InputLayoutDefinition* declarations;
		uint8_t numDeclarations;
		RID vertexShaderId;
	};
	RID createInputLayout(const InputLayoutInfo& info, const char* name = "InputLayout");

	struct InstancedInputLayoutInfo {
		InputLayoutDefinition* decl;
		uint8_t num;
		InstancedInputLayoutDefinition* instDecl;
		uint8_t instNum;
		RID shaderId;
	};
	RID createInstancedInputLayout(const InstancedInputLayoutInfo& info, const char* name = "InstancedInputLayout");

	RID createConstantBuffer(int byteWidth, void* data = 0, const char* name = "ConstantBuffer");

	struct IndexBufferInfo {
		uint32_t numIndices;
		IndexType indexType;
		BufferType type;
		void* data;
	};
	
	RID createIndexBuffer(const IndexBufferInfo& info, const char* name = "IndexBuffer");
	
	RID createQuadIndexBuffer(int numQuads, const char* name = "QuadIndexBuffer");

	RID createQuadIndexBuffer(int numQuads, int* order, const char* name = "QuadIndexBuffer");

	// vertex buffer
	struct VertexBufferInfo {
		BufferType type;
		uint32_t numVertices;
		uint32_t vertexSize;
		void* data;
	};
	RID createVertexBuffer(const VertexBufferInfo& info, const char* name = "VertexBuffer");
	
	void mapBufferData(RID rid, void* data, uint32_t size);
	
	RID createInstancedBuffer(RID vertexBuffer, RID instanceBuffer, const char* name = "UNKNOWN");

	
	RID createBuffer(int numElements, int byteStride, void* data = 0, const char* name = "UNKNOWN");

	// ---------------------------------------------------
	// StructuredBufferInfo
	// ---------------------------------------------------
	struct StructuredBufferInfo {
		unsigned int numElements;
		int elementSize;
		bool cpuWritable;
		bool gpuWritable;
		void* data;
		RID textureID;
		RID renderTarget;
	};
	RID createStructuredBuffer(const StructuredBufferInfo& info, const char* name = "StructuredBuffer");

	struct SamplerStateInfo {
		TextureAddressModes addressMode;
		TextureFilters filter;
	};
	
	RID createSamplerState(const SamplerStateInfo& info, const char* name = "SamplerState");

	// FIXME: add blendOperation!!! and blendMask!!!
	struct BlendStateInfo {
		BlendStates srcBlend;
		BlendStates srcAlphaBlend;
		BlendStates destBlend;
		BlendStates destAlphaBlend;
		bool alphaEnabled;
	};

	RID createBlendState(const BlendStateInfo& info, const char* name = "BlendState");

	enum ShaderType {
		ST_VERTEX_SHADER,
		ST_PIXEL_SHADER,
		ST_GEOMETRY_SHADER,
		ST_COMPUTE_SHADER
	};

	struct ShaderInfo {
		const char* csoName;
		const void* data;
		int dataSize;
		ShaderType type;
	};

	RID createShader(const ShaderInfo& info, const char* name = "Shader");
	
	// texture
	struct TextureInfo {
		uint16_t width;
		uint16_t height;
		uint8_t channels;
		void* data;
		TextureFormat format;
		uint16_t bindFlags;
	};
	
	RID createTexture(const TextureInfo& info, const char* name = "Texture");

	ds::vec2 getTextureSize(RID rid);

	byte* getBufferData(RID bufferID);

	struct RenderTargetInfo {
		uint16_t width;
		uint16_t height;
		const ds::Color& clearColor;
	};

	RID createRenderTarget(const RenderTargetInfo& info, const char* name = "RenderTarget");

	//void setRenderTarget(RID rtID);

	//void restoreBackBuffer();

	// rasterizer state
	struct RasterizerStateInfo {
		CullMode cullMode;
		FillMode fillMode;
		bool multiSample;
		bool scissor;
		float depthBias;
		float slopeDepthBias;
	};

	RID createRasterizerState(const RasterizerStateInfo& info, const char* name = "RasterizerState");

	RID findResource(const StaticHash& hash, ResourceType type);

	void rebuildCamera(Camera* camera);
	
	// drawing
	
	void begin();
	
	void end();
	
	bool isRunning();
	
	void shutdown();

	//
	// the compute shader functions
	//

	void dispatch(RID computeShaderID, int x, int y, int z);

	uint16_t getScreenWidth();

	uint16_t getScreenHeight();

	float getScreenAspectRatio();

	// input

	bool isKeyPressed(uint8_t key);

	vec2 getMousePosition();

	bool isMouseButtonClicked(int button);

	bool isMouseButtonPressed(int button);	

	// timing

	uint64_t getElapsedTicks();

	double getElapsedSeconds();

	uint64_t getTotalTicks();

	double GetTotalSeconds();

	uint32_t getFrameCount();

	uint32_t getFramesPerSecond();

	// GPU profiling

	namespace gpu {
		
		void init();
		
		void beginFrame();
		
		void endFrame();
		
		void measure(int index);
		
		void waitForData();
		
		float dt(int index);
		
		float dtAvg(int index);
		
		float totalTime();
		
		void shutdown();
	}

	// utils

	float random(float min, float max);

	const char* getLastError();

	enum SpecialKeys {
		DSKEY_Tab,
		DSKEY_LeftArrow,
		DSKEY_RightArrow,
		DSKEY_UpArrow,
		DSKEY_DownArrow,
		DSKEY_PageUp,
		DSKEY_PageDown,
		DSKEY_Home,
		DSKEY_End,
		DSKEY_Delete,
		DSKEY_Backspace,
		DSKEY_Enter,
		DSKEY_F1,
		DSKEY_F2,
		DSKEY_F3,
		DSKEY_F4,
		DSKEY_F5,
		DSKEY_F6,
		DSKEY_F7,
		DSKEY_F8,
		DSKEY_F9,
		DSKEY_F10,
		DSKEY_F11,
		DSKEY_F12,
		DSKEY_UNKNOWN
	};

	enum InputKeyType {
		IKT_SYSTEM,
		IKT_ASCII
	};

	struct InputKey {
		InputKeyType type;
		char value;
	};

	void addInputCharacter(char c);

	void addVirtualKey(uint32_t keyCode);

	int getNumInputKeys();

	const InputKey& getInputKey(int index);

	// ------------------------------------------
	
	void dbgPrint(uint16_t x, uint16_t y, char* format, ...);
	
	void saveResourcesToFile(const char* fileName = "resources.txt");

}

#ifndef SID
#define SID(str) (ds::StaticHash(str))
#endif
#ifndef SID_VAL
#define SID_VAL(str) (fnv1a(str))
#endif

#ifdef DS_IMPLEMENTATION

#include <Windows.h>
#include <crtdbg.h>  
#include <d3d11.h>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

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

	struct StructuredBuffer {
		ID3D11Buffer* buffer;
		ID3D11ShaderResourceView*  srv;
		ID3D11UnorderedAccessView* uav;
		unsigned int numElements;
		int elementSize;
	};

	enum PipelineStage {
		PLS_IA, // input assembler
		PLS_VS, // vertex shader
		PLS_VS_RES, // vertex shader resources
		PLS_GS, // geometry shader
		PLS_GS_RES, // geometry shader resources
		PLS_RS, // rasterizer
		PLS_PS, // pixel shader
		PLS_PS_RES, // pixel shader resources
		PLS_OM, // output merger
		PLS_UNKNOWN
	};

	const char* PIPELINE_STAGE_NAMES[] {
		"IA", // input assembler
		"VS", // vertex shader
		"VS_RES", // vertex shader resources
		"GS", // geometry shader
		"GS_RES", // geometry shader resources
		"RS", // rasterizer
		"PS", // pixel shader
		"PS_RES", // pixel shader resources
		"OM", // output merger
		"UNKNOWN"
	};

	const char* RESOURCE_NAMES[] {
		"NONE",
		"INPUT_LAYOUT",
		"CONSTANT_BUFFER",
		"INDEX_BUFFER",
		"VERTEX_BUFFER",
		"SAMPLER_STATE",
		"BLENDSTATE",
		"SRV",
		"RASTERIZER_STATE",		
		"INSTANCED_VERTEX_BUFFER",
		"VERTEX_SHADER",
		"GEOMETRY_SHADER",
		"PIXEL_SHADER",
		"RENDER_TARGET",
		"RENDER_PASS",
		"DRAW_ITEM",
		"STATE_GROUP",
		"TEXTURE_FROM_RT",
		"COMPUTE_SHADER",
		"STRUCTURED_BUFFER",
		"UA_SRV",
		"CS_GROUP"
	};

	void dbgInit();
	void dbgBegin();
	void dbgFlush();
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
	// RID conversion
	// ------------------------------------------------------
	static uint8_t stage_mask(RID rid) {
		return (rid >> 22) & 15;
	}

	static uint8_t slot_mask(RID rid) {
		return (rid >> 26) & 63;
	}

	static uint8_t type_mask(RID rid) {
		return (rid >> 16) & 63;
	}

	static uint16_t id_mask(RID rid) {
		return rid & 0xffff;
	}

	static RID buildRID(uint16_t id, uint8_t resourceType, uint8_t stage = 0, uint8_t slot = 0) {
		return id + (resourceType << 16) + (stage << 22) + (slot << 26);
	}

	// ******************************************************
	//
	// Internal pipeline state
	//
	// This is used when the StateGroups are applied to keep
	// track which resources have already been binded.
	//
	// ******************************************************
	class PipelineState {

	public:
		PipelineState() : _entries(0), _index(0), _capacity(0) {}
		~PipelineState() {
			if (_entries != 0) {
				delete[] _entries;
			}
		}
		void reset() {
			_index = 0;
		}
		bool isUsed(RID rid) {
			uint16_t mask = convert(rid);
			for (int i = 0; i < _index; ++i) {
				if ( mask == _entries[i]) {
					return true;
				}
			}
			return false;
		}

		void add(RID rid) {
			if (!isUsed(rid)) {
				if ((_index + 1) > _capacity) {
					alloc(_capacity * 2);
				}
				uint16_t mask = convert(rid);
				_entries[_index++] = mask;
			}
		}

		uint16_t size() const {
			return _index;
		}

		uint16_t get(uint16_t idx) const {
			return _entries[idx];
		}

		uint16_t* getPtr() const {
			return &_entries[0];
		}

		int diff(const PipelineState& other, uint16_t* entries, int max) {
			int num = 0;
			for (uint16_t i = 0; i < other.size(); ++i) {
				uint16_t current = other.get(i);
				bool found = false;
				for (uint16_t j = 0; j < _index; ++j) {
					if (!found && _entries[j] == current) {
						found = true;
					}
				}
				if (!found && num < max ) {
					entries[num++] = current;
				}
			}
			return num;
		}
	private:
		uint16_t convert(RID rid) {
			uint8_t type = type_mask(rid);
			if (type == RT_TEXTURE_FROM_RT) {
				type = RT_SRV;
				rid = buildRID(0, type, stage_mask(rid), slot_mask(rid));
			}
			uint16_t mask = (rid >> 16);
			return mask;
		}
		void alloc(uint16_t newCapacity) {
			if (_entries == 0) {
				_capacity = 16;
				_entries = new uint16_t[_capacity];
				_index = 0;
			}
			else {
				uint16_t* tmp = new uint16_t[newCapacity];
				memcpy(tmp, _entries, _index*sizeof(uint16_t));
				delete[] _entries;
				_entries = tmp;
				_capacity = newCapacity;
			}
		}
		uint16_t* _entries;
		uint16_t _capacity;
		uint16_t _index;
	};

	// ------------------------------------------------------
	// Shader
	// ------------------------------------------------------
	struct VertexShader {
		ID3D11VertexShader* vertexShader;
		void* vertexShaderBuffer;
		UINT bufferSize;
	};

	// ------------------------------------------------------
	// internal texture 
	// ------------------------------------------------------
	struct InternalTexture {
		int width;
		int height;
		RID samplerState;
		ID3D11ShaderResourceView* srv;
		ID3D11Texture2D* texture;
	};

	struct RenderTarget {
		ID3D11Texture2D* texture;
		ID3D11RenderTargetView* view;
		ID3D11ShaderResourceView* srv;
		ID3D11Texture2D* depthTexture;
		ID3D11DepthStencilView* depthStencilView;
		ds::Color clearColor;
	};

	// ******************************************************
	//
	// Resource management
	//
	// ******************************************************
	class BaseResource {

	public:
		BaseResource() {}
		virtual ~BaseResource() {}
		virtual void release() = 0;
		virtual const ResourceType getType() const = 0;
		void setRID(RID rid) {
			_rid = rid;
		}
		const RID& getRID() const {
			return _rid;
		}
		const int getNameIndex() const {
			return _nameIndex;
		}
		const StaticHash& getHash() const {
			return _hash;
		}
		void setNameIndex(int idx, const StaticHash& hash) {
			_nameIndex = idx;
			_hash = hash;
		}
	private:
		int _nameIndex;
		RID _rid;
		StaticHash _hash;
	};

	// ------------------------------------------------------
	// Abstract resource
	// ------------------------------------------------------
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

	// ------------------------------------------------------
	// ConstantBufferResource
	// ------------------------------------------------------
	class ConstantBufferResource : public AbstractResource<ID3D11Buffer*> {

	public:
		ConstantBufferResource(ID3D11Buffer* t, int byteWidth, void* bufferPtr) : AbstractResource(t) , _byteWidth(byteWidth) , _bufferPtr(bufferPtr) {}
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
		int getByteWidth() const {
			return _byteWidth;
		}
		void* getBufferPtr() const {
			return _bufferPtr;
		}
	private:
		int _byteWidth;
		void* _bufferPtr;
	};

	// ------------------------------------------------------
	// InputLayoutResource
	// ------------------------------------------------------
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
			return RT_INPUT_LAYOUT;
		}
	private:
		int _size;
	};

	// ------------------------------------------------------
	// VertexBufferResource
	// ------------------------------------------------------
	class VertexBufferResource : public AbstractResource<ID3D11Buffer*> {

	public:
		VertexBufferResource(ID3D11Buffer* t, int size, BufferType type, unsigned int vertexSize) : AbstractResource(t), _size(size), _vertexSize(vertexSize) {}
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
		const ResourceType getType() const {
			return RT_VERTEX_BUFFER;
		}
		BufferType getBufferType() const {
			return _type;
		}
		unsigned int getVertexSize() const {
			return _vertexSize;
		}
	private:
		int _size;
		BufferType _type;
		unsigned int _vertexSize;
	};

	// ------------------------------------------------------
	// VertexBufferResource
	// ------------------------------------------------------
	class BufferResource : public AbstractResource<StructuredBuffer*> {

	public:
		BufferResource(StructuredBuffer* t) : AbstractResource(t) {}
		virtual ~BufferResource() {}

		void release() {
			if (_data != 0) {
				if (_data->buffer != 0) {
					_data->buffer->Release();
				}
				if (_data->srv != 0) {
					_data->srv->Release();
				}
				if (_data->uav != 0) {
					_data->uav->Release();
				}
				delete _data;
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_STRUCTURED_BUFFER;
		}
	};

	// ------------------------------------------------------
	// InstancedVertexBufferResource
	// ------------------------------------------------------
	struct InstancedBindData {
		RID rid;
		RID instanceBuffer;
	};
	
	class InstancedVertexBufferResource : public AbstractResource<InstancedBindData*> {

	public:
		InstancedVertexBufferResource(InstancedBindData* t) : AbstractResource(t) {}
		virtual ~InstancedVertexBufferResource() {}

		void release() {
			if (_data != 0) {
				delete _data;
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_INSTANCED_VERTEX_BUFFER;
		}
	};

	// ------------------------------------------------------
	// IndexBufferResource
	// ------------------------------------------------------
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

	// ------------------------------------------------------
	// ShaderResourceViewResource
	// ------------------------------------------------------
	class ShaderResourceViewResource : public AbstractResource<InternalTexture*> {

	public:
		ShaderResourceViewResource(InternalTexture* t) : AbstractResource(t) {
			_size = ds::vec2(t->width, t->height);
		}
		virtual ~ShaderResourceViewResource() {}
		void release() {
			if (_data->srv != 0) {
				_data->srv->Release();
				delete _data;
				_data = 0;
			}
		}
		const ds::vec2& getSize() const {
			return _size;

		}
		const ResourceType getType() const {
			return RT_SRV;
		}
	private:
		ds::vec2 _size;
	};

	// ------------------------------------------------------
	// ShaderResourceViewResource
	// ------------------------------------------------------
	class UAResourceViewResource : public AbstractResource<ID3D11UnorderedAccessView*> {

	public:
		UAResourceViewResource(ID3D11UnorderedAccessView* t) : AbstractResource(t) {
		}
		virtual ~UAResourceViewResource() {}
		void release() {
			//if (_data != 0) {
				//_data->Release();
				//delete _data;
				//_data = 0;
			//}
		}
		const ResourceType getType() const {
			return RT_UA_SRV;
		}
	};

	// ------------------------------------------------------
	// BlendStateResource
	// ------------------------------------------------------
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

	// ------------------------------------------------------
	// SamplerStateResource
	// ------------------------------------------------------
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

	// ------------------------------------------------------
	// RasterizerStateResource
	// ------------------------------------------------------
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

	// ------------------------------------------------------
	// VertexShaderResource
	// ------------------------------------------------------
	class VertexShaderResource : public AbstractResource<VertexShader*> {

	public:
		VertexShaderResource(VertexShader* t) : AbstractResource(t) {}
		virtual ~VertexShaderResource() {}
		void release() {
			if (_data != 0) {
				if (_data->vertexShader != 0) {
					_data->vertexShader->Release();
					_data->vertexShader = 0;
				}
				if (_data->vertexShaderBuffer != 0) {
					delete[] _data->vertexShaderBuffer;
					_data->vertexShaderBuffer = 0;
				}
				delete _data;
			}
		}
		const ResourceType getType() const {
			return RT_VERTEX_SHADER;
		}
	};

	// ------------------------------------------------------
	// GeometryShaderResource
	// ------------------------------------------------------
	class GeometryShaderResource : public AbstractResource<ID3D11GeometryShader*> {

	public:
		GeometryShaderResource(ID3D11GeometryShader* t) : AbstractResource(t) {}
		virtual ~GeometryShaderResource() {}
		void release() {
			if (_data != 0) {				
				_data->Release();
				_data = 0;				
			}
		}
		const ResourceType getType() const {
			return RT_GEOMETRY_SHADER;
		}
	};

	// ------------------------------------------------------
	// PixelShaderResource
	// ------------------------------------------------------
	class PixelShaderResource : public AbstractResource<ID3D11PixelShader*> {

	public:
		PixelShaderResource(ID3D11PixelShader* t) : AbstractResource(t) {}
		virtual ~PixelShaderResource() {}
		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_PIXEL_SHADER;
		}
	};

	// ------------------------------------------------------
	// ComputeShaderResource
	// ------------------------------------------------------
	class ComputeShaderResource : public AbstractResource<ID3D11ComputeShader*> {

	public:
		ComputeShaderResource(ID3D11ComputeShader* t) : AbstractResource(t) {}
		virtual ~ComputeShaderResource() {}
		void release() {
			if (_data != 0) {
				_data->Release();
				_data = 0;
			}
		}
		const ResourceType getType() const {
			return RT_COMPUTE_SHADER;
		}
	};

	// ------------------------------------------------------
	// RenderTargetResource
	// ------------------------------------------------------
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
	// RenderPassResource
	// ------------------------------------------------------
	class RenderPassResource : public AbstractResource<RenderPass*> {

	public:
		RenderPassResource(RenderPass* t) : AbstractResource(t) {}
		virtual ~RenderPassResource() {}
		void release() {
			if (_data != 0) {				
				delete _data;
			}
		}
		const ResourceType getType() const {
			return RT_RENDER_PASS;
		}
	};

	// ------------------------------------------------------
	// DrawItemResource
	// ------------------------------------------------------
	class DrawItemResource : public AbstractResource<DrawItem*> {

	public:
		DrawItemResource(DrawItem* t) : AbstractResource(t) {}
		virtual ~DrawItemResource() {}
		void release() {
			if (_data != 0) {
				//for (int i = 0; i < _data->num; ++i) {
					//delete _data->groups[i];
				//}
				delete _data;
			}
		}
		const ResourceType getType() const {
			return RT_DRAW_ITEM;
		}
	};

	// ------------------------------------------------------
	// StateGroupResource
	// ------------------------------------------------------
	class StateGroupResource : public AbstractResource<StateGroup*> {

	public:
		StateGroupResource(StateGroup* t) : AbstractResource(t) {}
		virtual ~StateGroupResource() {}
		void release() {
			if (_data != 0) {
				delete _data;
			}
		}
		const ResourceType getType() const {
			return RT_STATE_GROUP;
		}
	};

	// ------------------------------------------------------
	// ComputeShaderResource
	// ------------------------------------------------------
	class ComputeShaderGroupResource : public AbstractResource<ComputeShaderGroup*> {

	public:
		ComputeShaderGroupResource(ComputeShaderGroup* t) : AbstractResource(t) {}
		virtual ~ComputeShaderGroupResource() {}
		void release() {
			if (_data != 0) {
				delete _data;
			}
		}
		const ResourceType getType() const {
			return RT_CS_GROUP;
		}
	};

	class GPUProfiler;

	// ---------------------------------------------------------------
	// the cube constant buffer
	// ---------------------------------------------------------------
	struct BasicConstantBuffer {
		matrix viewMatrix;
		matrix projectionMatrix;
		matrix worldMatrix;
		matrix viewProjectionMatrix;		
	};


	// ******************************************************
	//
	// Internal context
	//
	// ******************************************************
	struct DebugTextVertex {

		ds::vec3 position;
		ds::vec4 texture;
		ds::Color color;

	};

	struct DebugTextConstantBuffer {
		ds::vec4 screenDimension;
		ds::matrix wvp;
	};

	const int MAX_DBG_TXT_VERTICES = 2048;

	struct CharBuffer {

		char* data;
		int size;
		int capacity;
		int num;

		CharBuffer();
		~CharBuffer();

		void* alloc(int sz);
		void resize(int newCap);
		int append(const char* s, int len);
		int append(const char* s);
		int append(char s);
		const char* get(int index) const;

	};

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

		BasicConstantBuffer basicConstantBuffer;
		RID basicConstantBufferID;

		std::vector<BaseResource*> _resources;
		std::vector<StateGroup*> _groups;
		std::vector<ComputeShaderGroup*> _computeShaderGroups;

		bool mouseButtonClicked[2];
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
		
		InputKey inputKeys[256];
		int numInputKeys;

		PipelineState pipelineStates[2];
		int currentDrawCall;
		int lastDrawCall;
		RID drawCalls[2];

		//PipelineState* pipelineState;

		RID defaultStateGroup;

		// Debug
		bool supportDebug;
		RID debugTextureID;
		DebugTextConstantBuffer debugConstantBuffer;
		RID debugDrawItem;
		RID debugOrthoPass;
		Camera orthoCamera;
		DebugTextVertex debugVertices[MAX_DBG_TXT_VERTICES];
		RID debugVertexBufferID;
		int debugItemCount;

		CharBuffer* charBuffer;

	} InternalContext;

	static InternalContext* _ctx;
	
	uint16_t getScreenWidth() {
		return _ctx->screenWidth;
	}

	uint16_t getScreenHeight() {
		return _ctx->screenHeight;
	}

	float getScreenAspectRatio() {
		return static_cast<float>(_ctx->screenWidth) / static_cast<float>(_ctx->screenHeight);
	}

	static void assert_resource(RID rid, ResourceType type) {
		XASSERT(type_mask(rid) == type,"The selected resource %d is not the required type %s", rid, RESOURCE_NAMES[type]);
	}

	static RID addResource(BaseResource* res, ResourceType type, const char* name) {
		XASSERT((_ctx->_resources.size() + 1) < NO_RID, "The maximum number of resources reached");
		_ctx->_resources.push_back(res);
		RID rid = buildRID(static_cast<uint16_t>(_ctx->_resources.size() - 1), type);
		res->setRID(rid);		
		res->setNameIndex(_ctx->charBuffer->append(name), SID(name));
		return rid;
	}

	uint16_t getResourceIndex(RID rid,ResourceType type) {
		uint16_t idx = id_mask(rid);
		if (idx != NO_RID) {			
			XASSERT(idx < _ctx->_resources.size(), "Invalid resource selected - Out of bounds");
			int current = type_mask(rid);
			XASSERT(current == type, "The selected resource %d is not the required type: %s", idx, RESOURCE_NAMES[idx]);
			return idx;
		}
		return NO_RID;
	}

	RID findResource(const StaticHash& hash, ResourceType type) {
		for (size_t i = 0; i < _ctx->_resources.size(); ++i) {
			const BaseResource* res = _ctx->_resources[i];
			if (res->getHash().get() == hash.get() && res->getType() == type) {
				return res->getRID();
			}
		}
		return buildRID(NO_RID, type);
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
			char complete[1600];
			sprintf_s(complete, "%s - %d : %s", file, line, buffer);
			MessageBox(_ctx->hwnd, complete, "ERROR", NULL);
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
		char complete[1600];
		sprintf_s(complete, "%s - %d : %s", file, line, buffer);
		MessageBox(_ctx->hwnd, complete, "ERROR", NULL);
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
		_ctx->supportDebug = settings.supportDebug;
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

//#ifdef DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif

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
		//_ctx->pipelineState = new PipelineState;

		// create default state group
		BlendStateInfo defaultBlendState = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
		RID bs_id = ds::createBlendState( defaultBlendState, "DefaultBlendState");
		SamplerStateInfo defaultSamplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
		RID ssid = ds::createSamplerState(defaultSamplerInfo, "DefaultSamplerState");
		ds::RasterizerStateInfo rasterizerInfo = { ds::CullMode::BACK, ds::FillMode::SOLID, true, false, 0.0f, 0.0f };
		RID rasterizerStateID = ds::createRasterizerState(rasterizerInfo, "DefaultRasterizerState");

		_ctx->currentDrawCall = 0;
		_ctx->lastDrawCall = -1;

		_ctx->basicConstantBufferID = createConstantBuffer(sizeof(BasicConstantBuffer), &_ctx->basicConstantBuffer, "BasicConstantBuffer");
		matrix world = matIdentity();
		_ctx->basicConstantBuffer.worldMatrix = matTranspose(world);
		_ctx->defaultStateGroup = StateGroupBuilder()
			.blendState(bs_id)
			//.samplerState(ssid, ds::ShaderType::PIXEL)
			.noResource(RT_SRV, PLS_VS_RES, 0)
			.noResource(RT_SRV, PLS_GS_RES, 0)
			.noResource(RT_SRV, PLS_PS_RES, 0)
			.vertexShader(NO_RID)
			.geometryShader(NO_RID)
			.pixelShader(NO_RID)
			.indexBuffer(NO_RID)
			.vertexBuffer(NO_RID)
			.rasterizerState(rasterizerStateID)
			.build("DefaultGroup");

		if (settings.useGPUProfiling) {
			gpu::init();
		}
		if (_ctx->supportDebug) {
			dbgInit();
		}
		return true;
	}

	static RenderPass* getRenderPass(RID rid) {
		int ridx = getResourceIndex(rid, RT_RENDER_PASS);
		RenderPassResource* res = (RenderPassResource*)_ctx->_resources[ridx];
		return res->get();
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
	vec2 getMousePosition() {
		vec2 mp(-1.0f,-1.0f);
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

	bool isMouseButtonClicked(int button) {
		return _ctx->mouseButtonClicked[button];
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
			case WM_CHAR: {
				char ascii = wParam;
				ds::addInputCharacter(ascii);
				return 0;
			}
			case WM_KEYDOWN: {
				char ascii = wParam;
				ds::addVirtualKey(wParam);
				_ctx->keyState[ascii] = 80;
				return 0;
			}
			case WM_KEYUP: {
				UINT ascii = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
				_ctx->keyState[ascii] = 0;
				return 0;
			}
			case WM_LBUTTONDOWN:
				_ctx->mouseButtonState[0] = 80;
				return 0;
			case WM_LBUTTONUP:
				_ctx->mouseButtonState[0] = 0;
				_ctx->mouseButtonClicked[0] = true;
				return 0;
			case WM_RBUTTONDOWN:
				_ctx->mouseButtonState[1] = 80;
				return 0;
			case WM_RBUTTONUP:
				_ctx->mouseButtonState[1] = 0;
				_ctx->mouseButtonClicked[1] = true;
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
		_ctx->charBuffer = new CharBuffer;
		_ctx->leftOverTicks = 0;
		_ctx->framesPerSecond = 0;
		_ctx->framesThisSecond = 0;
		_ctx->secondCounter = 0;
		_ctx->numInputKeys = 0;
		_ctx->totalTicks = 0;
		_ctx->maxDelta = _ctx->timerFrequency.QuadPart / 10;
		_ctx->running = true;
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
			//delete _ctx->pipelineState;
			gpu::shutdown();
			for (size_t i = 0; i < _ctx->_resources.size(); ++i) {
				_ctx->_resources[i]->release();
				delete _ctx->_resources[i];
			}			
			for (size_t i = 0; i < _ctx->_groups.size(); ++i) {				
				delete _ctx->_groups[i];
			}
			delete _ctx->charBuffer;
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
	// begin rendering
	// ------------------------------------------------------
	void begin() {
		gpu::beginFrame();
		if (_ctx->supportDebug) {
			dbgBegin();
		}
		_ctx->d3dContext->OMSetRenderTargets(1, &_ctx->backBufferTarget, _ctx->depthStencilView);
		_ctx->d3dContext->ClearRenderTargetView(_ctx->backBufferTarget, _ctx->clearColor);
		_ctx->d3dContext->ClearDepthStencilView(_ctx->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		_ctx->d3dContext->OMSetDepthStencilState(_ctx->depthEnabledStencilState, 1);
		_ctx->depthBufferState = DepthBufferState::ENABLED;
	}

	// ------------------------------------------------------
	// end rendering
	// ------------------------------------------------------
	void end() {
		if (_ctx->supportDebug) {
			dbgFlush();
		}
		_ctx->swapChain->Present(0, 0);
		_ctx->numInputKeys = 0;
		_ctx->mouseButtonClicked[0] = false;
		_ctx->mouseButtonClicked[1] = false;
		gpu::endFrame();
	}

	static const char* DXBufferAttributeNames[] = {
		"POSITION",
		"COLOR",
		"TEXCOORD",
		"NORMAL",
		"TANGENT",
		"BINORMAL"
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
	RID createInputLayout(const InputLayoutInfo& info, const char* name) {
		D3D11_INPUT_ELEMENT_DESC* descriptors = new D3D11_INPUT_ELEMENT_DESC[info.numDeclarations];
		uint32_t index = 0;
		uint32_t counter = 0;
		//char nb[64];
		int si[8] = { 0 };
		for (int i = 0; i < info.numDeclarations; ++i) {
			D3D11_INPUT_ELEMENT_DESC& desc = descriptors[i];
			const InputLayoutDefinition& current = info.declarations[i];
			int fidx = find_format(info.declarations[i].type, info.declarations[i].size);
			if (fidx == -1) {
				return INVALID_RID;
			}
			const DXBufferAttributeType& formatType = DXBufferAttributeTypes[fidx];
			//sprintf(nb, "%s%d", DXBufferAttributeNames[decl[i].attribute], si[decl[i].attribute]);
			//desc.SemanticName = nb;
			desc.SemanticName = DXBufferAttributeNames[info.declarations[i].attribute];
			desc.SemanticIndex = si[info.declarations[i].attribute];
			desc.Format = formatType.format;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = index;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			index += formatType.bytes;
			si[info.declarations[i].attribute] += 1;
		}		
		uint16_t sidx = getResourceIndex(info.vertexShaderId, RT_VERTEX_SHADER);
		VertexShaderResource* sres = (VertexShaderResource*)_ctx->_resources[sidx];
		VertexShader* s = sres->get();
		ID3D11InputLayout* layout = 0;
		ASSERT_RESULT(_ctx->d3dDevice->CreateInputLayout(descriptors, info.numDeclarations, s->vertexShaderBuffer, s->bufferSize, &layout), "Failed to create input layout");
		InputLayoutResource* res = new InputLayoutResource(layout, index);
		return addResource(res, RT_INPUT_LAYOUT, name);
	}

	RID createInstancedInputLayout(const InstancedInputLayoutInfo& info, const char* name) {
		int total = info.num + info.instNum;
		D3D11_INPUT_ELEMENT_DESC* descriptors = new D3D11_INPUT_ELEMENT_DESC[total];
		uint32_t index = 0;
		uint32_t counter = 0;
		int si[8] = { 0 };
		for (int i = 0; i < info.num; ++i) {
			D3D11_INPUT_ELEMENT_DESC& desc = descriptors[counter++];
			const InputLayoutDefinition& current = info.decl[i];
			int fidx = find_format(current.type, current.size);
			if (fidx == -1) {
				return INVALID_RID;
			}
			const DXBufferAttributeType& formatType = DXBufferAttributeTypes[fidx];
			desc.SemanticName = DXBufferAttributeNames[info.decl[i].attribute];
			desc.SemanticIndex = si[info.decl[i].attribute];
			desc.Format = formatType.format;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = index;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			index += formatType.bytes;
			si[info.decl[i].attribute] += 1;
		}
		index = 0;
		for (int i = 0; i < info.instNum; ++i) {
			D3D11_INPUT_ELEMENT_DESC& desc = descriptors[counter++];
			const InstancedInputLayoutDefinition& current = info.instDecl[i];
			int fidx = find_format(current.type, current.size);
			if (fidx == -1) {
				return INVALID_RID;
			}
			const DXBufferAttributeType& formatType = DXBufferAttributeTypes[fidx];
			desc.SemanticName = current.name;
			desc.SemanticIndex = current.nameIndex;
			desc.Format = formatType.format;
			desc.InputSlot = 1;
			desc.AlignedByteOffset = index;
			desc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			desc.InstanceDataStepRate = 1;
			index += formatType.bytes;
		}
		uint16_t sidx = getResourceIndex(info.shaderId, RT_VERTEX_SHADER);
		VertexShaderResource* sres = (VertexShaderResource*)_ctx->_resources[sidx];
		VertexShader* s = sres->get();
		ID3D11InputLayout* layout = 0;
		ASSERT_RESULT(_ctx->d3dDevice->CreateInputLayout(descriptors, total, s->vertexShaderBuffer, s->bufferSize, &layout), "Failed to create input layout");
		InputLayoutResource* res = new InputLayoutResource(layout, index);
		return addResource(res, RT_INPUT_LAYOUT, name);
	}

	// ------------------------------------------------------
	// set vertex declaration
	// ------------------------------------------------------
	static void setVertexDeclaration(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_INPUT_LAYOUT);		
		if (ridx == NO_RID) {
			_ctx->d3dContext->IASetInputLayout(NULL);
		}
		else {				
			InputLayoutResource* res = (InputLayoutResource*)_ctx->_resources[ridx];
			_ctx->d3dContext->IASetInputLayout(res->get());
		}
	}

	// ------------------------------------------------------
	// constant buffer
	// ------------------------------------------------------
	RID createConstantBuffer(int byteWidth, void* data, const char* name) {
		// FIXME: check that byteWidth % 16 == 0
		D3D11_BUFFER_DESC constDesc;
		ZeroMemory(&constDesc, sizeof(constDesc));
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = byteWidth;
		constDesc.Usage = D3D11_USAGE_DYNAMIC;
		constDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ID3D11Buffer* buffer = 0;
		assert_result(_ctx->d3dDevice->CreateBuffer(&constDesc, 0, &buffer), "Failed to create constant buffer");	
		ConstantBufferResource* res = new ConstantBufferResource(buffer, byteWidth, data);
		if (data != 0) {
			D3D11_MAPPED_SUBRESOURCE resource;
			assert_result(_ctx->d3dContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource), "Failed to update constant buffer");
			void* ptr = resource.pData;
			memcpy(ptr, data, byteWidth);
			_ctx->d3dContext->Unmap(buffer, 0);
		}
		return addResource(res, RT_CONSTANT_BUFFER, name);
	}

	// ------------------------------------------------------
	// update constant buffer
	// ------------------------------------------------------
	static void updateConstantBuffer(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_CONSTANT_BUFFER);
		if ( ridx != NO_RID) {
			ConstantBufferResource* cbr = (ConstantBufferResource*)_ctx->_resources[ridx];
			if (cbr->getBufferPtr() != 0) {
				ID3D11Buffer* buffer = cbr->get();
				D3D11_MAPPED_SUBRESOURCE resource;
				assert_result(_ctx->d3dContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource), "Failed to update constant buffer");
				void* ptr = resource.pData;
				memcpy(ptr, cbr->getBufferPtr(), cbr->getByteWidth());
				_ctx->d3dContext->Unmap(buffer, 0);
			}
		}
	}

	// ------------------------------------------------------
	// set vertex shader constant buffer
	// ------------------------------------------------------
	static void setConstantBuffer(RID rid) {
		int stage = stage_mask(rid);
		int slot = slot_mask(rid);
		uint16_t ridx = getResourceIndex(rid, RT_CONSTANT_BUFFER);
		if (ridx != NO_RID) {
			ConstantBufferResource* cbr = (ConstantBufferResource*)_ctx->_resources[ridx];
			if (cbr->getBufferPtr() != 0) {
				updateConstantBuffer(rid);
			}
			ID3D11Buffer* buffer = cbr->get();
			switch (stage) {
				case PLS_VS_RES: _ctx->d3dContext->VSSetConstantBuffers(slot, 1, &buffer); break;
				case PLS_PS_RES: _ctx->d3dContext->PSSetConstantBuffers(slot, 1, &buffer); break;
				case PLS_GS_RES: _ctx->d3dContext->GSSetConstantBuffers(slot, 1, &buffer); break;
			}
		}
		// FIXME: check if there is a way to unbind a CB or if it is ok to just leave it as it is
		//else {
			//switch (stage) {
				//case PLS_VS_RES: _ctx->d3dContext->VSSetConstantBuffers(slot, 1, NULL); break;
				//case PLS_PS_RES: _ctx->d3dContext->PSSetConstantBuffers(slot, 1, NULL); break;
				//case PLS_GS_RES: _ctx->d3dContext->GSSetConstantBuffers(slot, 1, NULL); break;
			//}
		//}
	}

	// ------------------------------------------------------
	// set vertex shader constant buffer
	// ------------------------------------------------------
	static void setStructuredBuffer(RID rid) {
		int stage = stage_mask(rid);
		int slot = slot_mask(rid);
		uint16_t ridx = getResourceIndex(rid, RT_STRUCTURED_BUFFER);
		if (ridx != NO_RID) {
			BufferResource* cbr = (BufferResource*)_ctx->_resources[ridx];
			StructuredBuffer* sb = cbr->get();
			//if (buffer->buffer != 0) {
				//updateConstantBuffer(rid);
			//}
			//ID3D11Buffer* buffer = sb->buffer;
			switch (stage) {
				case PLS_VS_RES: _ctx->d3dContext->VSSetShaderResources(slot, 1, &sb->srv); break;
				//case PLS_PS_RES: _ctx->d3dContext->PSSetConstantBuffers(slot, 1, &buffer); break;
				//case PLS_GS_RES: _ctx->d3dContext->GSSetConstantBuffers(slot, 1, &buffer); break;
			}
		}
		// FIXME: check if there is a way to unbind a CB or if it is ok to just leave it as it is
		//else {
		//switch (stage) {
		//case PLS_VS_RES: _ctx->d3dContext->VSSetConstantBuffers(slot, 1, NULL); break;
		//case PLS_PS_RES: _ctx->d3dContext->PSSetConstantBuffers(slot, 1, NULL); break;
		//case PLS_GS_RES: _ctx->d3dContext->GSSetConstantBuffers(slot, 1, NULL); break;
		//}
		//}
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
	RID createIndexBuffer(const IndexBufferInfo& info, const char* name) {
		D3D11_BUFFER_DESC bufferDesc;
		if (info.type == BufferType::DYNAMIC) {
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else {
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
		}
		bufferDesc.ByteWidth = info.numIndices * INDEX_BUFFER_SIZE[info.indexType];
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = info.data;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		ID3D11Buffer* buffer = 0;
		assert_result(_ctx->d3dDevice->CreateBuffer(&bufferDesc, info.data ? &InitData : NULL, &buffer), "Failed to create index buffer");
		IndexBufferResource* res = new IndexBufferResource(buffer, INDEX_BUFFER_FORMATS[info.indexType], info.numIndices, info.type);
		return addResource(res, RT_INDEX_BUFFER, name);
	}
	// ------------------------------------------------------
	// set index buffer
	// ------------------------------------------------------
	static void setIndexBuffer(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_INDEX_BUFFER);
		if (ridx != NO_RID) {
			IndexBufferResource* res = (IndexBufferResource*)_ctx->_resources[ridx];
			_ctx->d3dContext->IASetIndexBuffer(res->get(), res->getFormat(), 0);
		}
		else {
			_ctx->d3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
		}
	}

	// ------------------------------------------------------
	// create a quad index buffer 0, 1, 2, 2, 3, 0
	// ------------------------------------------------------
	RID createQuadIndexBuffer(int numQuads, const char* name) {		
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
		IndexBufferInfo info = { size, ds::IndexType::UINT_32, ds::BufferType::STATIC, data };
		RID rid = createIndexBuffer(info, name);
		delete[] data;
		return rid;
	}

	// ------------------------------------------------------
	// create a quad index buffer 0, 1, 2, 2, 3, 0
	// ------------------------------------------------------
	RID createQuadIndexBuffer(int numQuads, int* order, const char* name) {
		int size = numQuads * 6;
		uint32_t* data = new uint32_t[size];
		int base = 0;
		int cnt = 0;
		for (int i = 0; i < numQuads; ++i) {
			for (int j = 0; j < 6; ++j) {
				data[base + j] = cnt + order[j];
			}
			base += 6;
			cnt += 4;
		}
		IndexBufferInfo info = { size, ds::IndexType::UINT_32, ds::BufferType::STATIC, data };
		RID rid = createIndexBuffer(info, name);
		delete[] data;
		return rid;
	}

	// ------------------------------------------------------
	// vertex buffer with optional data
	// ------------------------------------------------------
	RID createVertexBuffer(const VertexBufferInfo& info, const char* name) {
		UINT size = info.numVertices * info.vertexSize;
		D3D11_BUFFER_DESC bufferDesciption;
		ZeroMemory(&bufferDesciption, sizeof(bufferDesciption));
		if (info.type == BufferType::DYNAMIC) {
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
		if (info.data != 0) {
			D3D11_SUBRESOURCE_DATA resource;
			resource.pSysMem = info.data;
			resource.SysMemPitch = 0;
			resource.SysMemSlicePitch = 0;
			ASSERT_RESULT(_ctx->d3dDevice->CreateBuffer(&bufferDesciption, &resource, &buffer), "Failed to create vertex buffer");
		}
		else {
			ASSERT_RESULT(_ctx->d3dDevice->CreateBuffer(&bufferDesciption, 0, &buffer), "Failed to create vertex buffer");
		}
		VertexBufferResource* res = new VertexBufferResource(buffer, size, info.type, info.vertexSize);
		return addResource(res, RT_VERTEX_BUFFER, name);
	}

	RID createInstancedBuffer(RID vertexBuffer, RID instanceBuffer, const char* name) {
		InstancedBindData* data = new InstancedBindData;
		data->rid = vertexBuffer;
		data->instanceBuffer = instanceBuffer;
		InstancedVertexBufferResource* res = new InstancedVertexBufferResource(data);
		return addResource(res, RT_INSTANCED_VERTEX_BUFFER, name);
	}

	RID createBuffer(int numElements, int byteStride, void* data, const char* name) {
		// First we create a buffer in GPU memory
		D3D11_BUFFER_DESC descGPUBuffer;
		ZeroMemory(&descGPUBuffer, sizeof(descGPUBuffer));
		descGPUBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		descGPUBuffer.ByteWidth = numElements;
		descGPUBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		descGPUBuffer.StructureByteStride = byteStride;	
		StructuredBuffer* sb = new StructuredBuffer;
		sb->buffer = 0;
		sb->uav = 0;
		sb->numElements = numElements;
		sb->elementSize = byteStride;
		sb->srv = 0;
		if (data != 0) {
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = data;			
			_ctx->d3dDevice->CreateBuffer(&descGPUBuffer, &InitData, &sb->buffer);
		}
		else {
			_ctx->d3dDevice->CreateBuffer(&descGPUBuffer, NULL, &sb->buffer);
		}
		BufferResource* res = new BufferResource(sb);
		return addResource(res, RT_STRUCTURED_BUFFER, name);

	}

	// ------------------------------------------------------
	// createStructuredBuffer
	// ------------------------------------------------------
	// FIXME: do we need to add a paramter to info like SRV/UAV buffer?
	RID createStructuredBuffer(const StructuredBufferInfo& info, const char* name) {
		StructuredBuffer* sb = new StructuredBuffer;
		sb->buffer = 0;
		sb->uav = 0;
		sb->numElements = info.numElements;
		sb->elementSize = info.elementSize;
		sb->srv = 0;
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = info.numElements * info.elementSize;
		bufferDesc.StructureByteStride = info.elementSize;
		if ((!info.cpuWritable) && (!info.gpuWritable)) {
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		}
		else if (info.cpuWritable && (!info.gpuWritable)) {
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		}
		else if ((!info.cpuWritable) && info.gpuWritable) {
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		}
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = info.elementSize;
		if (info.data != 0) {
			D3D11_SUBRESOURCE_DATA bufferInitData;
			ZeroMemory(&bufferInitData, sizeof(bufferInitData));
			bufferInitData.pSysMem = info.data;
			ASSERT_RESULT(_ctx->d3dDevice->CreateBuffer(&bufferDesc, &bufferInitData, &sb->buffer), "Cannot create buffer");
		}
		else {
			ASSERT_RESULT(_ctx->d3dDevice->CreateBuffer(&bufferDesc, NULL, &sb->buffer), "Cannot create buffer");
		}
		// FIXME: correct?
		if (info.cpuWritable) {
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.BufferEx.FirstElement = 0;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			srvDesc.BufferEx.NumElements = info.numElements;
			ASSERT_RESULT(_ctx->d3dDevice->CreateShaderResourceView(sb->buffer, &srvDesc, &sb->srv), "Cannot create shader resource view");
		}
		if (info.gpuWritable) {
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			ZeroMemory((&uavDesc), sizeof(uavDesc));
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.NumElements = info.numElements;
			if (info.textureID != NO_RID) {
				uint16_t ridx = getResourceIndex(info.textureID, RT_SRV);
				ShaderResourceViewResource* bufferRes = (ShaderResourceViewResource*)_ctx->_resources[ridx];
				InternalTexture* tex = bufferRes->get();
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				ASSERT_RESULT(_ctx->d3dDevice->CreateUnorderedAccessView(tex->texture, &uavDesc, &sb->uav), "Cannot create unordered access view");
			}
			else if (info.renderTarget != NO_RID) {

			}
			else {
				ASSERT_RESULT(_ctx->d3dDevice->CreateUnorderedAccessView(sb->buffer, &uavDesc, &sb->uav), "Cannot create unordered access view");
			}
		}
		BufferResource* res = new BufferResource(sb);
		return addResource(res, RT_STRUCTURED_BUFFER, name);

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
	static void setVertexBuffer(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_VERTEX_BUFFER);		
		if (ridx == NO_RID) {
			_ctx->d3dContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
		}
		else {
			VertexBufferResource* res = (VertexBufferResource*)_ctx->_resources[ridx];
			unsigned int stride = res->getVertexSize();
			unsigned int offset = 0;
			ID3D11Buffer* buffer = res->get();
			_ctx->d3dContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
		}
	}

	// ------------------------------------------------------
	// set vertex buffer
	// ------------------------------------------------------
	static void setInstancedVertexBuffer(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_INSTANCED_VERTEX_BUFFER);
		InstancedVertexBufferResource* ibr = (InstancedVertexBufferResource*)_ctx->_resources[ridx];
		InstancedBindData* data = ibr->get();
		VertexBufferResource* fr = (VertexBufferResource*)_ctx->_resources[id_mask(data->rid)];
		VertexBufferResource* sr = (VertexBufferResource*)_ctx->_resources[id_mask(data->instanceBuffer)];
		unsigned int strides[2] = { fr->getVertexSize(),sr->getVertexSize() };
		unsigned int offsets[2] = { 0 };
		ID3D11Buffer* bufferPointers[2];
		bufferPointers[0] = fr->get();
		bufferPointers[1] = sr->get();
		_ctx->d3dContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	}

	// ------------------------------------------------------
	// map data to vertex buffer
	// ------------------------------------------------------
	void mapBufferData(RID rid, void* data, uint32_t size) {
		uint16_t type = type_mask(rid);
		if (type == RT_VERTEX_BUFFER) {
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
		else if (type == RT_STRUCTURED_BUFFER) {
			uint16_t ridx = getResourceIndex(rid, RT_STRUCTURED_BUFFER);
			if (ridx != NO_RID) {
				BufferResource* cbr = (BufferResource*)_ctx->_resources[ridx];
				D3D11_MAPPED_SUBRESOURCE resource;
				StructuredBuffer* buffer = cbr->get();
				assert_result(_ctx->d3dContext->Map(buffer->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource), "Failed to map data");
				void* ptr = resource.pData;
				memcpy(ptr, data, size);
				_ctx->d3dContext->Unmap(buffer->buffer, 0);
			}
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
	RID createSamplerState(const SamplerStateInfo& info, const char* name) {
		D3D11_SAMPLER_DESC colorMapDesc;
		ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));

		colorMapDesc.AddressU = TEXTURE_ADDRESSMODES[info.addressMode];
		colorMapDesc.AddressV = TEXTURE_ADDRESSMODES[info.addressMode];
		colorMapDesc.AddressW = TEXTURE_ADDRESSMODES[info.addressMode];

		colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc.Filter = TEXTURE_FILTERMODES[info.filter];
		colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
		
		ID3D11SamplerState* sampler;
		assert_result(_ctx->d3dDevice->CreateSamplerState(&colorMapDesc, &sampler), "Failed to create SamplerState");
		SamplerStateResource* res = new SamplerStateResource(sampler);
		return addResource(res, RT_SAMPLER_STATE, name);
	}

	// ------------------------------------------------------
	// set sampler state
	// ------------------------------------------------------
	static void setSamplerState(RID rid) {
		int stage = stage_mask(rid);
		uint16_t ridx = getResourceIndex(rid, RT_SAMPLER_STATE);
		if (ridx != NO_RID) {
			SamplerStateResource* res = (SamplerStateResource*)_ctx->_resources[ridx];
			ID3D11SamplerState* state = res->get();
			if (stage == PLS_PS_RES) {
				_ctx->d3dContext->PSSetSamplers(0, 1, &state);
			}
			else if (stage == PLS_VS_RES) {
				_ctx->d3dContext->VSSetSamplers(0, 1, &state);
			}
			else if (stage == PLS_GS_RES) {
				_ctx->d3dContext->GSSetSamplers(0, 1, &state);
			}
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
	// create blend state
	// ------------------------------------------------------
	RID createBlendState(const BlendStateInfo& info, const char* name) {
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		if (info.alphaEnabled) {
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
		}
		else {
			blendDesc.RenderTarget[0].BlendEnable = FALSE;
			//blendDesc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
		}
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = BLEND_STATEMAPPINGS[info.srcBlend];
		blendDesc.RenderTarget[0].DestBlend = BLEND_STATEMAPPINGS[info.destBlend];
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = BLEND_STATEMAPPINGS[info.srcAlphaBlend];
		blendDesc.RenderTarget[0].DestBlendAlpha = BLEND_STATEMAPPINGS[info.destAlphaBlend];
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

		ID3D11BlendState* state;
		assert_result(_ctx->d3dDevice->CreateBlendState(&blendDesc, &state), "Failed to create blendstate");
		BlendStateResource* res = new BlendStateResource(state);
		return addResource(res, RT_BLENDSTATE, name);
	}

	// ------------------------------------------------------
	// set blend state
	// ------------------------------------------------------
	static void setBlendState(RID rid) {
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
	// set depth buffer state
	// ------------------------------------------------------
	static void setDepthBufferState(DepthBufferState state) {
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
	// create vertex shader
	// ------------------------------------------------------
	RID createVertexShader(const void* data, int size, const char* name) {
		VertexShader* s = new VertexShader;
		assert_result(_ctx->d3dDevice->CreateVertexShader(data,size,nullptr,&s->vertexShader), "Failed to create vertex shader");
		s->vertexShaderBuffer = new char[size];
		memcpy(s->vertexShaderBuffer, data, size);
		s->bufferSize = size;
		VertexShaderResource* res = new VertexShaderResource(s);
		return addResource(res, RT_VERTEX_SHADER, name);
	}

	// ------------------------------------------------------
	// create geometry shader
	// ------------------------------------------------------
	RID createGeometryShader(const void* data, int size, const char* name) {
		ID3D11GeometryShader* s;
		assert_result(_ctx->d3dDevice->CreateGeometryShader(data,size,nullptr,&s), "Failed to create geometry shader");
		GeometryShaderResource* res = new GeometryShaderResource(s);
		return addResource(res, RT_GEOMETRY_SHADER, name);
	}

	// ------------------------------------------------------
	// create pixel shader
	// ------------------------------------------------------
	RID createPixelShader(const void* data, int size, const char* name) {
		ID3D11PixelShader* s;
		assert_result(_ctx->d3dDevice->CreatePixelShader(data, size, nullptr, &s), "Failed to create pixel shader");
		PixelShaderResource* res = new PixelShaderResource(s);
		return addResource(res, RT_PIXEL_SHADER, name);
	}

	// ------------------------------------------------------
	// create compute shader
	// ------------------------------------------------------
	RID createComputeShader(const void* data, int size, const char* name) {
		ID3D11ComputeShader* s;
		assert_result(_ctx->d3dDevice->CreateComputeShader(data, size, nullptr, &s), "Failed to create compute shader");
		ComputeShaderResource* res = new ComputeShaderResource(s);
		return addResource(res, RT_COMPUTE_SHADER, name);
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
	RID loadVertexShader(const char* csoName, const char* name) {
		DataFile file = read_data(csoName);
		XASSERT(file.size != -1, "Cannot load vertex shader file: '%s'", csoName);
		VertexShader* s = new VertexShader;
		assert_result(_ctx->d3dDevice->CreateVertexShader(
				file.data,
				file.size,
				nullptr,
				&s->vertexShader), "Failed to create vertex shader");
		s->vertexShaderBuffer = new char[file.size];
		memcpy(s->vertexShaderBuffer, file.data, file.size);
		s->bufferSize = file.size;
		delete[] file.data;
		VertexShaderResource* res = new VertexShaderResource(s);
		return addResource(res, RT_VERTEX_SHADER, name);
	}

	// ------------------------------------------------------
	// load pixel shader
	// ------------------------------------------------------
	RID loadPixelShader(const char* csoName, const char* name) {
		DataFile file = read_data(csoName);
		XASSERT(file.size != -1, "Cannot load pixel shader file: '%s'", csoName);
		ID3D11PixelShader* s;
		assert_result(_ctx->d3dDevice->CreatePixelShader(
			file.data,
			file.size,
			nullptr,
			&s), "Failed to create pixel shader");
		delete[] file.data;
		PixelShaderResource* res = new PixelShaderResource(s);
		return addResource(res, RT_PIXEL_SHADER, name);
	}

	// ------------------------------------------------------
	// load compute shader
	// ------------------------------------------------------
	RID loadComputeShader(const char* csoName, const char* name) {
		DataFile file = read_data(csoName);
		XASSERT(file.size != -1, "Cannot load compute shader file: '%s'", csoName);
		ID3D11ComputeShader* s;
		assert_result(_ctx->d3dDevice->CreateComputeShader(
			file.data,
			file.size,
			nullptr,
			&s), "Failed to create compute shader");
		delete[] file.data;
		ComputeShaderResource* res = new ComputeShaderResource(s);
		return addResource(res, RT_COMPUTE_SHADER, name);
	}

	// ------------------------------------------------------
	// load geometry shader
	// ------------------------------------------------------
	RID loadGeometryShader(const char* csoName, const char* name) {
		DataFile file = read_data(csoName);
		XASSERT(file.size != -1, "Cannot load geometry shader file: '%s'", csoName);
		ID3D11GeometryShader* s;
		assert_result(_ctx->d3dDevice->CreateGeometryShader(
			file.data,
			file.size,
			nullptr,
			&s), "Failed to create pixel shader");
		delete[] file.data;
		GeometryShaderResource* res = new GeometryShaderResource(s);
		return addResource(res, RT_GEOMETRY_SHADER, name);
	}

	RID createShader(const ShaderInfo& info, const char* name) {
		if (info.csoName == 0) {
			if (info.type == ST_VERTEX_SHADER) {
				return createVertexShader(info.data,info.dataSize, name);
			}
			else if (info.type == ST_PIXEL_SHADER) {
				return createPixelShader(info.data, info.dataSize, name);
			}
			else if (info.type == ST_GEOMETRY_SHADER) {
				return createGeometryShader(info.data, info.dataSize, name);
			}
			else if (info.type == ST_COMPUTE_SHADER) {
				return createComputeShader(info.data, info.dataSize, name);
			}
		}
		else {
			if (info.type == ST_VERTEX_SHADER) {
				return loadVertexShader(info.csoName, name);
			}
			else if (info.type == ST_PIXEL_SHADER) {
				return loadPixelShader(info.csoName, name);
			}
			else if (info.type == ST_GEOMETRY_SHADER) {
				return loadGeometryShader(info.csoName, name);
			}
			else if (info.type == ST_COMPUTE_SHADER) {
				return loadComputeShader(info.csoName, name);
			}
		}
		return NO_RID;
	}

	// ------------------------------------------------------
	// set shader
	// ------------------------------------------------------
	static void setPixelShader(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_PIXEL_SHADER);
		if (ridx == NO_RID) {
			_ctx->d3dContext->VSSetShader(NULL, NULL, 0);
		}
		else {
			PixelShaderResource* res = (PixelShaderResource*)_ctx->_resources[ridx];
			if (res->get() != 0) {
				_ctx->d3dContext->PSSetShader(res->get(), 0, 0);
			}
			else {
				_ctx->d3dContext->PSSetShader(NULL, NULL, 0);
			}
		}
	}

	// ------------------------------------------------------
	// set shader
	// ------------------------------------------------------
	static void setGeometryShader(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_GEOMETRY_SHADER);
		if (ridx == NO_RID) {
			_ctx->d3dContext->GSSetShader(NULL, NULL, 0);
		}
		else {
			GeometryShaderResource* res = (GeometryShaderResource*)_ctx->_resources[ridx];
			if (res->get() != 0) {
				_ctx->d3dContext->GSSetShader(res->get(), 0, 0);
			}
			else {
				_ctx->d3dContext->GSSetShader(NULL, NULL, 0);
			}
		}
	}

	// ------------------------------------------------------
	// set shader
	// ------------------------------------------------------
	static void setVertexShader(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_VERTEX_SHADER);
		if (ridx == NO_RID) {
			_ctx->d3dContext->VSSetShader(NULL, NULL, 0);
		}
		else {
			VertexShaderResource* res = (VertexShaderResource*)_ctx->_resources[ridx];
			VertexShader* s = res->get();
			if (s->vertexShader != 0) {
				_ctx->d3dContext->VSSetShader(s->vertexShader, 0, 0);
			}
			else {
				_ctx->d3dContext->VSSetShader(NULL, NULL, 0);
			}
		}
	}

	const static DXGI_FORMAT TEXTURE_FOMATS[] = {
		DXGI_FORMAT_R8_UINT,
		DXGI_FORMAT_R8G8B8A8_UINT,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	};

	// ------------------------------------------------------
	// create texture
	// ------------------------------------------------------	
	RID createTexture(const TextureInfo& info, const char* name) {

		InternalTexture* tex = new InternalTexture;
		tex->width = info.width;
		tex->height = info.height;

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = info.width;
		desc.Height = info.height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = TEXTURE_FOMATS[info.format];
		desc.SampleDesc.Count = 1;// _ctx->multisampling;
		desc.SampleDesc.Quality = 0;		
		desc.BindFlags = info.bindFlags;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		//ID3D11Texture2D *texture2D = 0;
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
			//desc.Usage = D3D11_USAGE_IMMUTABLE;
		//desc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_DYNAMIC;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		if (info.data != 0) {
			D3D11_SUBRESOURCE_DATA subres;
			subres.pSysMem = info.data;
			subres.SysMemPitch = info.width * info.channels;
			subres.SysMemSlicePitch = 0;

			ASSERT_RESULT(_ctx->d3dDevice->CreateTexture2D(&desc, &subres, &tex->texture), "Failed to create Texture2D");
		}
		else {
			ASSERT_RESULT(_ctx->d3dDevice->CreateTexture2D(&desc, NULL, &tex->texture), "Failed to create Texture2D");
		}

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
		srvDesc.Format = TEXTURE_FOMATS[info.format];
		ASSERT_RESULT(_ctx->d3dDevice->CreateShaderResourceView(tex->texture, &srvDesc, &tex->srv), "Failed to create resource view");
		ShaderResourceViewResource* res = new ShaderResourceViewResource(tex);
		return addResource(res, RT_SRV, name);
	}

	ds::vec2 getTextureSize(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_SRV);
		XASSERT(ridx != NO_RID, "Invalid texture selected");
		ShaderResourceViewResource* res = (ShaderResourceViewResource*)_ctx->_resources[ridx];
		return res->getSize();
	}

	RID createShaderResourceView(RID bufferID, const char* name) {
		uint16_t ridx = getResourceIndex(bufferID, RT_STRUCTURED_BUFFER);
		BufferResource* bufferRes = (BufferResource*)_ctx->_resources[ridx];
		StructuredBuffer* sb = bufferRes->get();
		D3D11_BUFFER_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(descBuf));
		sb->buffer->GetDesc(&descBuf);

		D3D11_SHADER_RESOURCE_VIEW_DESC descView;
		ZeroMemory(&descView, sizeof(descView));
		descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		descView.BufferEx.FirstElement = 0;

		descView.Format = DXGI_FORMAT_UNKNOWN;
		descView.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
		ID3D11ShaderResourceView* srv;
		ASSERT_RESULT(_ctx->d3dDevice->CreateShaderResourceView(sb->buffer, &descView, &srv), "Failed to create resource view");
		InternalTexture* tex = new InternalTexture;
		// FIXME: get size!!
		tex->width = 0;
		tex->height = 0;
		tex->srv = srv;
		ShaderResourceViewResource* res = new ShaderResourceViewResource(tex);
		return addResource(res, RT_SRV, name);
	}

	RID createUnorderedAccessView(RID bufferID, const char* name) {
		uint16_t ridx = getResourceIndex(bufferID, RT_STRUCTURED_BUFFER);
		BufferResource* bufferRes = (BufferResource*)_ctx->_resources[ridx];
		StructuredBuffer* sb = bufferRes->get();
		D3D11_BUFFER_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(descBuf));
		sb->buffer->GetDesc(&descBuf);

		D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
		ZeroMemory(&descView, sizeof(descView));
		descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		descView.Buffer.FirstElement = 0;

		descView.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
		descView.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
		ID3D11UnorderedAccessView* srv;
		ASSERT_RESULT(_ctx->d3dDevice->CreateUnorderedAccessView(sb->buffer, &descView, &srv), "Failed to create uav resource view");
		UAResourceViewResource* res = new UAResourceViewResource(srv);
		return addResource(res, RT_UA_SRV, name);
	}

	RID createUnorderedAccessView(RID textureID, int numElements, const char* name) {
		uint16_t ridx = getResourceIndex(textureID, RT_SRV);
		ShaderResourceViewResource* bufferRes = (ShaderResourceViewResource*)_ctx->_resources[ridx];
		InternalTexture* tex = bufferRes->get();
		D3D11_SHADER_RESOURCE_VIEW_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(descBuf));
		tex->srv->GetDesc(&descBuf);
		D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
		ZeroMemory(&descView, sizeof(descView));
		descView.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		descView.Buffer.FirstElement = 0;

		descView.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
		descView.Buffer.NumElements = numElements;
		ID3D11UnorderedAccessView* srv;
		ASSERT_RESULT(_ctx->d3dDevice->CreateUnorderedAccessView(tex->texture, &descView, &srv), "Failed to create uav resource view");
		UAResourceViewResource* res = new UAResourceViewResource(srv);
		return addResource(res, RT_UA_SRV, name);
	}

	// ------------------------------------------------------
	// set texture
	// ------------------------------------------------------
	static void setTexture(RID rid) {
		uint16_t ridx = getResourceIndex(rid, RT_SRV);
		int stage = stage_mask(rid);
		int slot = slot_mask(rid);
		ID3D11ShaderResourceView* srv = 0;
		if (ridx != NO_RID) {
			ShaderResourceViewResource* res = (ShaderResourceViewResource*)_ctx->_resources[ridx];
			srv = res->get()->srv;
		}
		if (stage == PLS_PS_RES) {
			_ctx->d3dContext->PSSetShaderResources(slot, 1, &srv);
		}
		else if (stage == PLS_VS_RES) {
			_ctx->d3dContext->VSSetShaderResources(slot, 1, &srv);
		}
		else if (stage == PLS_GS_RES) {
			_ctx->d3dContext->GSSetShaderResources(slot, 1, &srv);
		}
	}

	void setTextureFromRenderTarget(RID rtID) {
		uint16_t ridx = getResourceIndex(rtID, RT_TEXTURE_FROM_RT);
		RenderTargetResource* res = (RenderTargetResource*)_ctx->_resources[ridx];
		int stage = stage_mask(rtID);
		int slot = slot_mask(rtID);
		if (stage == PLS_PS_RES) {
			_ctx->d3dContext->PSSetShaderResources(slot, 1, &res->get()->srv);
		}
		else if (stage == PLS_VS_RES) {
			_ctx->d3dContext->VSSetShaderResources(slot, 1, &res->get()->srv);
		}
		else if (stage == PLS_GS_RES) {
			_ctx->d3dContext->GSSetShaderResources(slot, 1, &res->get()->srv);
		}
	}

	// ------------------------------------------------------
	// create rasterizer state
	// ------------------------------------------------------
	RID createRasterizerState(const RasterizerStateInfo& info, const char* name) {
		D3D11_RASTERIZER_DESC desc;
		desc.CullMode = (D3D11_CULL_MODE)info.cullMode;
		desc.FillMode = (D3D11_FILL_MODE)info.fillMode;
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = (INT)info.depthBias;
		desc.DepthBiasClamp = 0.0f;
		desc.SlopeScaledDepthBias = info.slopeDepthBias;
		desc.AntialiasedLineEnable = FALSE;
		desc.DepthClipEnable = TRUE;
		desc.MultisampleEnable = (BOOL)info.multiSample;
		desc.ScissorEnable = (BOOL)info.scissor;
		ID3D11RasterizerState* state = 0;
		assert_result(_ctx->d3dDevice->CreateRasterizerState(&desc, &state), "Failed to create rasterizer state");
		RasterizerStateResource* res = new RasterizerStateResource(state);
		return addResource(res, RT_RASTERIZER_STATE, name);
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
	RID createRenderTarget(const RenderTargetInfo& info, const char* name) {
		RenderTarget* rt = new RenderTarget;
		rt->clearColor = info.clearColor;
		// Initialize the render target texture description.
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = info.width;
		textureDesc.Height = info.height;
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
		depthTexDesc.Width = info.width;
		depthTexDesc.Height = info.height;
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
		return addResource(res,RT_RENDER_TARGET, name);
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
			_ctx->d3dContext->ClearRenderTargetView(rt->view, rt->clearColor);
			// Clear the depth buffer.
			_ctx->d3dContext->ClearDepthStencilView(rt->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
	}

	void restoreBackBuffer() {
		_ctx->d3dContext->OMSetRenderTargets(1, &_ctx->backBufferTarget, _ctx->depthStencilView);
	}

	void rebuildCamera(Camera* camera) {
		vec3 R = camera->right;
		vec3 U = camera->up;
		vec3 L = camera->target;
		vec3 P = camera->position;

		L = normalize(L);
		U = normalize(cross(L, R));
		R = cross(U, L);

		float x = -dot(P, R);
		float y = -dot(P, U);
		float z = -dot(P, L);

		camera->right = R;
		camera->up = U;
		camera->target = L;

		camera->viewMatrix(0, 0) = camera->right.x;
		camera->viewMatrix(1, 0) = camera->right.y;
		camera->viewMatrix(2, 0) = camera->right.z;
		camera->viewMatrix(3, 0) = x;

		camera->viewMatrix(0, 1) = camera->up.x;
		camera->viewMatrix(1, 1) = camera->up.y;
		camera->viewMatrix(2, 1) = camera->up.z;
		camera->viewMatrix(3, 1) = y;

		camera->viewMatrix(0, 2) = camera->target.x;
		camera->viewMatrix(1, 2) = camera->target.y;
		camera->viewMatrix(2, 2) = camera->target.z;
		camera->viewMatrix(3, 2) = z;

		camera->viewMatrix(0, 3) = 0.0f;
		camera->viewMatrix(1, 3) = 0.0f;
		camera->viewMatrix(2, 3) = 0.0f;
		camera->viewMatrix(3, 3) = 1.0f;

		camera->viewProjectionMatrix = camera->viewMatrix * camera->projectionMatrix;
	}

	// -----------------------------------------------------------------
	// extract PipelineStage from shader RID
	// -----------------------------------------------------------------
	static PipelineStage extractFromShader(RID shader) {
		int shaderType = type_mask(shader);
		PipelineStage stage = PLS_UNKNOWN;
		switch (shaderType) {
			case RT_VERTEX_SHADER: stage = PLS_VS_RES; break;
			case RT_GEOMETRY_SHADER: stage = PLS_GS_RES; break;
			case RT_PIXEL_SHADER: stage = PLS_PS_RES; break;
		}
		return stage;
	}
	// -----------------------------------------------------------------
	// compile with array of StateGroups
	// -----------------------------------------------------------------
	RID compile(const DrawCommand cmd, RID* groups, int num, const char* name) {
		DrawItem* item = new DrawItem;
		item->command = cmd;
		item->groups = new RID[num + 1];
		for (int i = 0; i < num; ++i) {
			item->groups[i] = groups[i];
		}
		item->groups[num] = _ctx->defaultStateGroup;
		item->num = num + 1;
		item->nameIndex = _ctx->charBuffer->append(name);
		return addResource(new DrawItemResource(item), RT_DRAW_ITEM, name);
	}

	// -----------------------------------------------------------------
	// compile with only one StateGroup
	// -----------------------------------------------------------------
	RID compile(const DrawCommand cmd, RID group, const char* name) {
		DrawItem* item = new DrawItem;
		item->command = cmd;
		item->groups = new RID[2];
		item->groups[0] = group;
		item->groups[1] = _ctx->defaultStateGroup;
		item->num = 2;
		item->nameIndex = _ctx->charBuffer->append(name);
		return addResource(new DrawItemResource(item), RT_DRAW_ITEM, name);
	}
	
	void assertResourceType(RID rid, ResourceType type) {
		int id = id_mask(rid);
		if (id != NO_RID) {
			int rt = type_mask(rid);
			XASSERT(rt == type, "The selected resource %d is not the required type: %s", id, RESOURCE_NAMES[rt]);
		}
	}

	// ******************************************************
	//
	// StateGroupBuilder
	//
	// ******************************************************
	void BasicGroupBuilder::add(uint16_t index, ResourceType type, int stage, int slot) {
		if ((_num + 1 ) > _total) {
			if (_items == 0) {
				_items = new RID[16];
				_total = 16;
			}
			else {
				_total += 16;
				RID* tt = new RID[_total];
				memcpy(tt, _items, _num * sizeof(RID));
				delete[] _items;
				_items = tt;
			}
		}
		_items[_num++] = buildRID(index, type, stage, slot);
	}

	void BasicGroupBuilder::basicBinding(RID rid, ResourceType type, int stage, int slot) {
		uint16_t id = id_mask(rid);
		if (id == NO_RID) {
			rid = buildRID(NO_RID, type, stage, 0);
		}
		assertResourceType(rid, type);
		add(id, type, stage, slot);
	}

	StateGroupBuilder& StateGroupBuilder::inputLayout(RID rid) {
		basicBinding(rid, ResourceType::RT_INPUT_LAYOUT, PLS_IA);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::constantBuffer(RID rid, RID shader, int slot) {
		int stage = extractFromShader(shader);
		basicBinding(rid, ResourceType::RT_CONSTANT_BUFFER, stage, slot);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::structuredBuffer(RID rid, RID shader, int slot) {
		int stage = extractFromShader(shader);
		basicBinding(rid, ResourceType::RT_STRUCTURED_BUFFER, stage, slot);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::basicConstantBuffer(RID shader, int slot) {
		int stage = extractFromShader(shader);
		basicBinding(_ctx->basicConstantBufferID, ResourceType::RT_CONSTANT_BUFFER, stage, slot);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::blendState(RID rid) {
		basicBinding(rid, RT_BLENDSTATE, PLS_OM);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::samplerState(RID rid, RID shader) {
		int stage = extractFromShader(shader);
		basicBinding(rid, ResourceType::RT_SAMPLER_STATE, stage, 0);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::vertexBuffer(RID rid) {
		basicBinding(rid, RT_VERTEX_BUFFER, PLS_IA);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::instancedVertexBuffer(RID rid) {
		basicBinding(rid, RT_INSTANCED_VERTEX_BUFFER, PLS_IA);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::vertexShader(RID rid) {
		basicBinding(rid, RT_VERTEX_SHADER, PLS_PS);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::geometryShader(RID rid) {
		basicBinding(rid, RT_GEOMETRY_SHADER, PLS_GS);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::pixelShader(RID rid) {
		basicBinding(rid, RT_PIXEL_SHADER, PLS_PS);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::indexBuffer(RID rid) {
		basicBinding(rid, ResourceType::RT_INDEX_BUFFER, PLS_IA);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::texture(RID rid, RID shader, int slot) {
		int stage = extractFromShader(shader);
		basicBinding(rid, ResourceType::RT_SRV, stage, slot);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::textureFromRenderTarget(RID rtID, RID shader, int slot) {
		int shaderType = type_mask(shader);
		int stage = extractFromShader(shader);
		RID r = buildRID(id_mask(rtID), RT_TEXTURE_FROM_RT, stage, slot);
		basicBinding(r, ResourceType::RT_TEXTURE_FROM_RT, stage, slot);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::noResource(ResourceType type, int stage, int slot) {
		basicBinding(NO_RID, type, stage, slot);
		return *this;
	}

	StateGroupBuilder& StateGroupBuilder::rasterizerState(RID rid) {
		basicBinding(rid, ResourceType::RT_RASTERIZER_STATE, PLS_RS);
		return *this;
	}

	int StateGroupBuilder::partition(RID* a, int l, int r) {
		RID pivot;
		int i, j;
		RID t;
		pivot = a[l];
		i = l; j = r + 1;
		while (1) {
			do ++i; while (a[i] <= pivot && i <= r);
			do --j; while (a[j] > pivot);
			if (i >= j) break;
			t = a[i]; a[i] = a[j]; a[j] = t;
		}
		t = a[l]; a[l] = a[j]; a[j] = t;
		return j;
	}

	void StateGroupBuilder::quickSort(RID* a, int l, int r)	{
		if (l < r)	{
			int j = partition(a, l, r);
			quickSort(a, l, j - 1);
			quickSort(a, j + 1, r);
		}
	}

	RID StateGroupBuilder::build(const char* name) {
		StateGroup* group = new StateGroup();
		quickSort(_items, 0, _num - 1);
		group->num = _num;
		group->items = new RID[_num];
		memcpy(group->items, _items, _num * sizeof(RID));
		RID rid = addResource(new StateGroupResource(group),RT_STATE_GROUP, name);
		group->rid = rid;
		return rid;
	}

	static void applyResource(RID current, int type) {
		if (type == ResourceType::RT_INPUT_LAYOUT) {
			setVertexDeclaration(current);
		}
		else if (type == ResourceType::RT_SAMPLER_STATE) {
			setSamplerState(current);
		}
		else if (type == ResourceType::RT_BLENDSTATE) {
			setBlendState(current);
		}
		else if (type == ResourceType::RT_RASTERIZER_STATE) {
			setRasterizerState(current);
		}
		else if (type == ResourceType::RT_VERTEX_BUFFER) {
			setVertexBuffer(current);
		}
		else if (type == ResourceType::RT_INSTANCED_VERTEX_BUFFER) {
			setInstancedVertexBuffer(current);
		}
		else if (type == ResourceType::RT_INDEX_BUFFER) {
			setIndexBuffer(current);
		}
		else if (type == ResourceType::RT_VERTEX_SHADER) {
			setVertexShader(current);
		}
		else if (type == ResourceType::RT_GEOMETRY_SHADER) {
			setGeometryShader(current);
		}
		else if (type == ResourceType::RT_PIXEL_SHADER) {
			setPixelShader(current);
		}
		else if (type == ResourceType::RT_SRV) {
			setTexture(current);
		}
		else if (type == ResourceType::RT_TEXTURE_FROM_RT) {
			setTextureFromRenderTarget(current);
		}
		else if (type == ResourceType::RT_CONSTANT_BUFFER) {
			setConstantBuffer(current);
		}
		else if (type == ResourceType::RT_STRUCTURED_BUFFER) {
			setStructuredBuffer(current);
		}
	}

	// ------------------------------------------------------
	// ComputeShaderBuilder
	// ------------------------------------------------------
	ComputeShaderBuilder& ComputeShaderBuilder::shader(RID shaderID) {
		basicBinding(shaderID, RT_COMPUTE_SHADER, 0, 0);
		return *this;
	}

	ComputeShaderBuilder& ComputeShaderBuilder::input(RID srv, int slot) {
		basicBinding(srv, RT_STRUCTURED_BUFFER, 0, slot);
		return *this;
	}

	ComputeShaderBuilder& ComputeShaderBuilder::output(RID srv, int slot) {
		basicBinding(srv, RT_STRUCTURED_BUFFER, 0, slot);
		return *this;
	}

	ComputeShaderBuilder& ComputeShaderBuilder::constantBuffer(RID buffer, int slot) {
		basicBinding(buffer, RT_CONSTANT_BUFFER, 0, slot);
		return *this;
	}

	RID ComputeShaderBuilder::build(const char* name) {
		ComputeShaderGroup* csg = new ComputeShaderGroup();
		csg->num = _num;
		csg->items = new RID[_num];
		memcpy(csg->items, _items, _num * sizeof(RID));
		RID rid = addResource(new ComputeShaderGroupResource(csg), RT_CS_GROUP, name);
		csg->rid = rid;
		return rid;
	}
	
	// ------------------------------------------------------
	// dispatch compute shader
	// ------------------------------------------------------
	void dispatch(RID computeShaderID, int x,int y, int z) {
		uint16_t ridx = getResourceIndex(computeShaderID, RT_CS_GROUP);
		ComputeShaderGroupResource* res = (ComputeShaderGroupResource*)_ctx->_resources[ridx];
		ComputeShaderGroup* group = res->get();
		for (int i = 0; i < group->num; ++i) {
			RID current = group->items[i];
			int type = type_mask(current);
			if (type == RT_COMPUTE_SHADER) {
				uint16_t sidx = getResourceIndex(current, RT_COMPUTE_SHADER);
				ComputeShaderResource* sres = (ComputeShaderResource*)_ctx->_resources[sidx];
				_ctx->d3dContext->CSSetShader(sres->get(), NULL, 0);
			}
			else if (type == RT_STRUCTURED_BUFFER) {
				uint16_t sridx = getResourceIndex(current, RT_STRUCTURED_BUFFER);
				BufferResource* bufferRes = (BufferResource*)_ctx->_resources[sridx];				
				StructuredBuffer* sb = bufferRes->get();
				int slot = slot_mask(current);
				if (sb->srv != 0) {
					_ctx->d3dContext->CSSetShaderResources(slot, 1, &sb->srv);
				}
				if (sb->uav != 0) {
					_ctx->d3dContext->CSSetUnorderedAccessViews(slot, 1, &sb->uav, NULL);
				}
			}
			else if (type == RT_SRV) {
				uint16_t sridx = getResourceIndex(current, RT_SRV);
				ShaderResourceViewResource* srvRes = (ShaderResourceViewResource*)_ctx->_resources[sridx];
				InternalTexture* tex = srvRes->get();
				int slot = slot_mask(current);
				_ctx->d3dContext->CSSetShaderResources(slot, 1, &tex->srv);
			}
			else if (type == RT_UA_SRV) {
				uint16_t uaidx = getResourceIndex(current, RT_UA_SRV);
				UAResourceViewResource* uar = (UAResourceViewResource*)_ctx->_resources[uaidx];
				int slot = slot_mask(current);
				ID3D11UnorderedAccessView* view = uar->get();
				_ctx->d3dContext->CSSetUnorderedAccessViews(slot, 1, &view, NULL);
			}
			else if (type == RT_CONSTANT_BUFFER) {
				uint16_t cbidx = getResourceIndex(current, RT_CONSTANT_BUFFER);
				ConstantBufferResource* res = (ConstantBufferResource*)_ctx->_resources[cbidx];
				int slot = slot_mask(current);
				ID3D11Buffer* buffer = res->get();
				_ctx->d3dContext->CSSetConstantBuffers(slot, 1, &buffer);
			}
		}
		_ctx->d3dContext->Dispatch(x, y, z);
		// FIXME: clean up - there might be more than 1 srv or uav
		_ctx->d3dContext->CSSetShader(NULL, NULL, 0);
		ID3D11ShaderResourceView* nullSRV[] = { NULL };
		_ctx->d3dContext->CSSetShaderResources(0, 1, nullSRV);

		// Unbind output from compute shader
		ID3D11UnorderedAccessView* nullUAV[] = { NULL };
		_ctx->d3dContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	}
	// ------------------------------------------------------
	// clear leaking states
	// ------------------------------------------------------
	static void clearStates(uint16_t* differences, int num) {
		for (int i = 0; i < num; ++i) {
			uint32_t real = (differences[i] << 16);
			applyResource(real + NO_RID, type_mask(real));
		}
	}


	byte* getBufferData(RID bufferID) {
		ID3D11Buffer* debugbuf = NULL;

		uint16_t ridx = getResourceIndex(bufferID, RT_STRUCTURED_BUFFER);
		BufferResource* bufferRes = (BufferResource*)_ctx->_resources[ridx];
		StructuredBuffer* sb = bufferRes->get();
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		sb->buffer->GetDesc(&desc);

		UINT byteSize = desc.ByteWidth;

		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.MiscFlags = 0;

		HRESULT r = _ctx->d3dDevice->CreateBuffer(&desc, NULL, &debugbuf);
		if ( r == S_OK) {
			_ctx->d3dContext->CopyResource(debugbuf, sb->buffer);

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			if (_ctx->d3dContext->Map(debugbuf, 0, D3D11_MAP_READ, 0, &mappedResource) != S_OK)
				return 0;

			byte* outBuff = new byte[byteSize];
			memcpy(outBuff, mappedResource.pData, byteSize);

			_ctx->d3dContext->Unmap(debugbuf, 0);

			debugbuf->Release();

			return outBuff;
		}
		return 0;
	}

	// ------------------------------------------------------
	// submit draw command
	// ------------------------------------------------------
	void submit(RID renderPass, RID drawItemID, int numElements) {
		uint16_t pidx = getResourceIndex(renderPass, RT_RENDER_PASS);
		RenderPassResource* rpRes = (RenderPassResource*)_ctx->_resources[pidx];
		RenderPass* pass = rpRes->get();
		if (pass->numRenderTargets > 0) {
			for (int i = 0; i < pass->numRenderTargets; ++i) {
				setRenderTarget(pass->rts[i]);
			}
		}
		else {
			//_ctx->d3dContext->OMSetRenderTargets(1, &rt->view, rt->depthStencilView);
		}
		Camera* camera = pass->camera;
		_ctx->basicConstantBuffer.viewMatrix = matTranspose(camera->viewMatrix);
		_ctx->basicConstantBuffer.projectionMatrix = matTranspose(camera->projectionMatrix);
		_ctx->basicConstantBuffer.viewProjectionMatrix = matTranspose(camera->viewProjectionMatrix);
		// FIXME: how to handle world matrix???
		setDepthBufferState(pass->depthState);
		uint16_t ridx = getResourceIndex(drawItemID, RT_DRAW_ITEM);
		DrawItemResource* res = (DrawItemResource*)_ctx->_resources[ridx];
		const DrawItem* item = res->get();
		_ctx->pipelineStates[_ctx->currentDrawCall].reset();
		_ctx->drawCalls[_ctx->currentDrawCall] = drawItemID;
		for (int i = 0; i < item->num; ++i) {
			apply(&_ctx->pipelineStates[_ctx->currentDrawCall], item->groups[i]);
		}
		if (_ctx->lastDrawCall >= 0) {
			uint16_t differences[64];
			// build diff between draw calls
			int num = _ctx->pipelineStates[_ctx->currentDrawCall].diff(_ctx->pipelineStates[_ctx->lastDrawCall], differences, 64);
			if (num > 0) {
				int sidx = id_mask(drawItemID);
				clearStates(differences, num);
			}
		}
		// toggle draw call
		_ctx->lastDrawCall = (_ctx->lastDrawCall + 1) & 1;
		_ctx->currentDrawCall = (_ctx->currentDrawCall + 1) & 1;
		const DrawCommand& cmd = item->command;
		int num = cmd.size;
		if (numElements != -1) {
			num = numElements;
		}
		_ctx->d3dContext->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGIES[cmd.topology]);
		switch (cmd.drawType) {
			case DT_VERTICES: _ctx->d3dContext->Draw(num, 0); break;
			case DT_INDEXED: _ctx->d3dContext->DrawIndexed(num, 0, 0); break;
			case DT_INSTANCED: _ctx->d3dContext->DrawInstanced(num, cmd.instances, 0, 0); break;
		}
		// FIXME: is this correct? At least it removes the warnings when using render targets as textures
		ID3D11ShaderResourceView *const pSRV[2] = { NULL, NULL };
		_ctx->d3dContext->PSSetShaderResources(0, 2, pSRV);
		// FIXME: this is wrong since there might be several submit which would like to use the same rt
		if (pass->numRenderTargets > 0) {			
			restoreBackBuffer();
		}
	}

	static void apply(PipelineState* pipelineState, RID groupID) {
		StateGroupResource* res = (StateGroupResource*)_ctx->_resources[id_mask(groupID)];
		StateGroup* group = res->get();
		for (int i = 0; i < group->num; ++i) {
			RID current = group->items[i];
			int type = type_mask(current);
			if (!pipelineState->isUsed(current)) {
				applyResource(current, type);
			}
			pipelineState->add(current);
		}
	}

	// ******************************************************
	// Render pass
	// ******************************************************
	RID createRenderPass(const RenderPassInfo& info, const char* name) {
		RenderPass* rp = new RenderPass();
		rp->camera = info.camera;
		int nr = info.numRenderTargets;
		if (nr > 4) {
			nr = 4;
		}
		rp->numRenderTargets = nr;
		for (int i = 0; i < nr; ++i) {
			rp->rts[i] = info.renderTargets[i];
		}
		rp->depthState = info.depthBufferState;
		RenderPassResource* res = new RenderPassResource(rp);
		return addResource(res, RT_RENDER_PASS, name);
	}


	// ******************************************************
	//
	// Math
	//
	// ******************************************************
	matrix matIdentity() {
		matrix m(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return m;
	}

	matrix matOrthoLH(float w, float h, float zn, float zf) {
		// msdn.microsoft.com/de-de/library/windows/desktop/bb204940(v=vs.85).aspx
		matrix tmp = matIdentity();
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
	matrix matScale(const vec3& scale) {
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
	matrix matRotationX(float angle) {
		matrix sm(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cos(angle), -sin(angle), 0.0f,
			0.0f, sin(angle), cos(angle), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return sm;
	}

	matrix matRotationY(float angle) {
		matrix sm(
			cos(angle), 0.0f, sin(angle), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sin(angle), 0.0f, cos(angle), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return sm;
	}
	// FIXME: wrong direction!!!!
	matrix matRotationZ(float angle) {
		matrix sm(
			cos(angle), -sin(angle), 0.0f, 0.0f,
			sin(angle), cos(angle), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return sm;
	}

	matrix matRotation(const vec3& r) {
		return matRotationZ(r.z) * matRotationY(r.y) * matRotationX(r.x);
	}

	// -------------------------------------------------------
	// Transpose matrix
	// -------------------------------------------------------
	matrix matTranspose(const matrix& m) {
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
	matrix matTranslate(const vec3& pos) {
		matrix tm(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			pos.x, pos.y, pos.z, 1.0f
		);
		return tm;
	}

	matrix matLookAtLH(const vec3& eye, const vec3& lookAt, const vec3& up) {
		// see msdn.microsoft.com/de-de/library/windows/desktop/bb205342(v=vs.85).aspx
		vec3 zAxis = normalize(lookAt - eye);
		vec3 xAxis = normalize(cross(up, zAxis));
		vec3 yAxis = cross(zAxis, xAxis);
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

	matrix matPerspectiveFovLH(float fovy, float aspect, float zn, float zf) {
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

	vec3 matTransformNormal(const vec3& v, const matrix& m) {
		vec3 result =
			vec3(v.x * m._11 + v.y * m._21 + v.z * m._31,
				v.x * m._12 + v.y * m._22 + v.z * m._32,
				v.x * m._13 + v.y * m._23 + v.z * m._33);
		return result;
	}

	matrix matRotation(const vec3& v, float angle) {
		float L = (v.x * v.x + v.y * v.y + v.z * v.z);
		float u2 = v.x * v.x;
		float vec2 = v.y * v.y;
		float w2 = v.z * v.z;
		matrix tmp = matIdentity();
		tmp._11 = (u2 + (vec2 + w2) * cos(angle)) / L;
		tmp._12 = (v.x * v.y * (1 - cos(angle)) - v.z * sqrt(L) * sin(angle)) / L;
		tmp._13 = (v.x * v.z * (1 - cos(angle)) + v.y * sqrt(L) * sin(angle)) / L;
		tmp._14 = 0.0f;

		tmp._21 = (v.x * v.y * (1 - cos(angle)) + v.z * sqrt(L) * sin(angle)) / L;
		tmp._22 = (vec2 + (u2 + w2) * cos(angle)) / L;
		tmp._23 = (v.y * v.z * (1 - cos(angle)) - v.x * sqrt(L) * sin(angle)) / L;
		tmp._24 = 0.0f;

		tmp._31 = (v.x * v.z * (1 - cos(angle)) - v.y * sqrt(L) * sin(angle)) / L;
		tmp._32 = (v.y * v.z * (1 - cos(angle)) + v.x * sqrt(L) * sin(angle)) / L;
		tmp._33 = (w2 + (u2 + vec2) * cos(angle)) / L;
		tmp._34 = 0.0f;

		return tmp;
	}

	matrix matInverse(const matrix& m) {
		matrix ret;
		float tmp[12]; /* temp array for pairs */
		float src[16]; /* array of transpose source matrix */
		float det; /* determinant */
		float* dst = ret;
		float* mat = m;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				src[i * 4 + j] = m.m[i][j];
			}
		}
		/* transpose matrix */
		for (int i = 0; i < 4; i++) {
			src[i] = mat[i * 4];
			src[i + 4] = mat[i * 4 + 1];
			src[i + 8] = mat[i * 4 + 2];
			src[i + 12] = mat[i * 4 + 3];
		}
		/* calculate pairs for first 8 elements (cofactors) */
		tmp[0] = src[10] * src[15];
		tmp[1] = src[11] * src[14];
		tmp[2] = src[9] * src[15];
		tmp[3] = src[11] * src[13];
		tmp[4] = src[9] * src[14];
		tmp[5] = src[10] * src[13];
		tmp[6] = src[8] * src[15];
		tmp[7] = src[11] * src[12];
		tmp[8] = src[8] * src[14];
		tmp[9] = src[10] * src[12];
		tmp[10] = src[8] * src[13];
		tmp[11] = src[9] * src[12];
		/* calculate first 8 elements (cofactors) */
		dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
		dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
		dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
		dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
		dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
		dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
		dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
		dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
		dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
		dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
		dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
		dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
		dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
		dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
		dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
		dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
		/* calculate pairs for second 8 elements (cofactors) */
		tmp[0] = src[2] * src[7];
		tmp[1] = src[3] * src[6];
		tmp[2] = src[1] * src[7];
		tmp[3] = src[3] * src[5];
		tmp[4] = src[1] * src[6];
		tmp[5] = src[2] * src[5];
		tmp[6] = src[0] * src[7];
		tmp[7] = src[3] * src[4];
		tmp[8] = src[0] * src[6];
		tmp[9] = src[2] * src[4];
		tmp[10] = src[0] * src[5];
		tmp[11] = src[1] * src[4];
		/* calculate second 8 elements (cofactors) */
		dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
		dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
		dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
		dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
		dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
		dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
		dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
		dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
		dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
		dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
		dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
		dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
		dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
		dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
		dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
		dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
		/* calculate determinant */
		det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];
		/* calculate matrix inverse */
		det = 1 / det;
		for (int j = 0; j < 16; j++) {
			dst[j] *= det;
		}
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				ret.m[i][j] = dst[i * 4 + j];
			}
		}
		return ret;
	}

	vec4 operator * (const matrix& m, const vec4& v) {
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
		vec4 tmp;
		tmp.x = m._11 * v.x + m._21 * v.y + m._31 * v.z + m._41 * v.w;
		tmp.y = m._12 * v.x + m._22 * v.y + m._32 * v.z + m._42 * v.w;
		tmp.z = m._13 * v.x + m._23 * v.y + m._33 * v.z + m._43 * v.w;
		tmp.w = m._14 * v.x + m._24 * v.y + m._34 * v.z + m._44 * v.w;
		return tmp;
	}

	vec3 operator * (const matrix& m, const vec3& v) {
		vec4 nv(v.x, v.y, v.z, 1.0f);
		vec4 tmp = m * nv;
		return vec3(tmp.x, tmp.y, tmp.z);
	}

	// ******************************************************
	//
	// Input handling
	//
	// ******************************************************
	void addInputCharacter(char c) {
		if (c >= 32) {
			InputKey& k = _ctx->inputKeys[_ctx->numInputKeys++];
			k.type = IKT_ASCII;
			k.value = c;
		}
	}

	void addVirtualKey(uint32_t keyCode) {
		int value = SpecialKeys::DSKEY_UNKNOWN;
		switch (keyCode) {
			case VK_TAB: value = SpecialKeys::DSKEY_Tab; break;
			case VK_BACK: value = SpecialKeys::DSKEY_Backspace; break;					
			case VK_LEFT: value = SpecialKeys::DSKEY_LeftArrow; break;
			case VK_UP: value = SpecialKeys::DSKEY_UpArrow; break;
			case VK_RIGHT: value = SpecialKeys::DSKEY_RightArrow; break;
			case VK_DOWN: value = SpecialKeys::DSKEY_DownArrow; break;
			case VK_HOME: value = SpecialKeys::DSKEY_Home; break;
			case VK_END: value = SpecialKeys::DSKEY_End; break;
			case VK_DELETE: value = SpecialKeys::DSKEY_Delete; break;
			case VK_RETURN: value = SpecialKeys::DSKEY_Enter; break;
			case VK_F1: value = SpecialKeys::DSKEY_F1; break;
			case VK_F2: value = SpecialKeys::DSKEY_F2; break;
			case VK_F3: value = SpecialKeys::DSKEY_F3; break;
			case VK_F4: value = SpecialKeys::DSKEY_F4; break;
			case VK_F5: value = SpecialKeys::DSKEY_F5; break;
			case VK_F6: value = SpecialKeys::DSKEY_F6; break;
			case VK_F7: value = SpecialKeys::DSKEY_F7; break;
			case VK_F8: value = SpecialKeys::DSKEY_F8; break;
			case VK_F9: value = SpecialKeys::DSKEY_F9; break;
			case VK_F10: value = SpecialKeys::DSKEY_F10; break;
			case VK_F11: value = SpecialKeys::DSKEY_F11; break;
			case VK_F12: value = SpecialKeys::DSKEY_F12; break;
		}
		if (value != SpecialKeys::DSKEY_UNKNOWN) {
			InputKey& k = _ctx->inputKeys[_ctx->numInputKeys++];
			k.type = IKT_SYSTEM;
			k.value = value;
		}
		//else {
			//printf("unknown: %d\n", keyCode);
		//}
	}

	int getNumInputKeys() {
		return _ctx->numInputKeys;
	}

	const InputKey& getInputKey(int index) {
		return _ctx->inputKeys[index];
	}

	// ******************************************************
	//
	// DEBUG - print resources
	//
	// ******************************************************
	void saveResourcesToFile(const char* fileName) {
		FILE* fp = fopen(fileName, "w");
		fprintf(fp, " index | resource type       | Name\n");
		fprintf(fp, "--------------------------------------------------------------\n");
		for (size_t i = 0; i < _ctx->_resources.size(); ++i) {
			const BaseResource* res = _ctx->_resources[i];
			RID rid = res->getRID();			
			fprintf(fp," %3d  | %-20s | %s\n", id_mask(rid), RESOURCE_NAMES[type_mask(rid)], _ctx->charBuffer->get(res->getNameIndex()));
		}
		fprintf(fp, "\n");
		for (size_t i = 0; i < _ctx->_resources.size(); ++i) {
			const BaseResource* br = _ctx->_resources[i];
			if (br->getType() == RT_DRAW_ITEM) {				
				const DrawItemResource* dir = (DrawItemResource*)_ctx->_resources[i];
				const DrawItem* item = dir->get();
				fprintf(fp,"\nDrawItem %d (%s) - groups: %d\n", id_mask(br->getRID()),_ctx->charBuffer->get(item->nameIndex),item->num);
				for (int j = 0; j < item->num; ++j) {
					RID groupID = item->groups[j];					
					StateGroupResource* res = (StateGroupResource*)_ctx->_resources[id_mask(groupID)];
					StateGroup* group = res->get();
					fprintf(fp, "Group: %d (%s)\n", id_mask(group->rid), _ctx->charBuffer->get(res->getNameIndex()));
					fprintf(fp, "resource type        | id    | stage    | slot | Name\n");
					fprintf(fp, "------------------------------------------------------------------------------------------\n");
					for (int k = 0; k < group->num; ++k) {
						RID current = group->items[k];
						if (id_mask(current) != NO_RID) {
							BaseResource* res = _ctx->_resources[id_mask(current)];
							fprintf(fp, "%-20s | %5d | %-8s | %2d   | %s\n", RESOURCE_NAMES[type_mask(current)], id_mask(current), PIPELINE_STAGE_NAMES[stage_mask(current)], slot_mask(current),_ctx->charBuffer->get(res->getNameIndex()));
						}
						else {
							fprintf(fp, "%-20s | %5d | %-8s | %2d   | NO_RID\n", RESOURCE_NAMES[type_mask(current)], id_mask(current), PIPELINE_STAGE_NAMES[stage_mask(current)], slot_mask(current));
						}
					}
				}
			}
		}
		fclose(fp);
	}

	// ******************************************************
	//
	// GPU profiling
	//
	// ******************************************************
	namespace gpu {

		const int GTS_MAX = 64;

		struct GPUProfilingContext {
			int currFrame;
			int lastFrame;
			int currentMax;
			ID3D11Query* disjointQuery[2];
			ID3D11Query* timestampQuery[GTS_MAX][2];
			float adT[GTS_MAX];
			float adTAvg[GTS_MAX];
			float adTTotalAvg[GTS_MAX];
			int frameCountAvg;
			float tBeginAvg;
		};

		static GPUProfilingContext* _gpCtx = 0;

		void init() {
			_gpCtx = new GPUProfilingContext;
			_gpCtx->lastFrame = -1;
			_gpCtx->frameCountAvg = 0;
			_gpCtx->tBeginAvg = 0.0f;
			_gpCtx->currentMax = 1;
			_gpCtx->currFrame = 0;
			memset(_gpCtx->adT, 0, sizeof(_gpCtx->adT));
			memset(_gpCtx->adTAvg, 0, sizeof(_gpCtx->adT));
			memset(_gpCtx->adTTotalAvg, 0, sizeof(_gpCtx->adT));
			D3D11_QUERY_DESC queryDesc = { D3D11_QUERY_TIMESTAMP_DISJOINT, 0 };
			for (int i = 0; i < 2; ++i) {
				_ctx->d3dDevice->CreateQuery(&queryDesc, &_gpCtx->disjointQuery[i]);
			}
			queryDesc.Query = D3D11_QUERY_TIMESTAMP;
			for (int i = 0; i < 2; ++i) {
				for (int gts = 0; gts < GTS_MAX; ++gts) {
					_ctx->d3dDevice->CreateQuery(&queryDesc, &_gpCtx->timestampQuery[gts][i]);
				}
			}
		}

		void beginFrame() {
			if (_gpCtx != 0) {
				_gpCtx->currentMax = 0;
				_ctx->d3dContext->Begin(_gpCtx->disjointQuery[_gpCtx->currFrame]);
				measure(0);
			}
		}

		void measure(int gts) {
			if (_gpCtx != 0) {
				if ((gts + 1) > _gpCtx->currentMax) {
					_gpCtx->currentMax = gts + 1;
				}
				_ctx->d3dContext->End(_gpCtx->timestampQuery[gts][_gpCtx->currFrame]);
			}
		}

		void endFrame() {
			if (_gpCtx != 0) {
				measure(_gpCtx->currentMax);
				_ctx->d3dContext->End(_gpCtx->disjointQuery[_gpCtx->currFrame]);
				++_gpCtx->currFrame &= 1;
			}
		}

		// Wait on GPU for last frame's data (not this frame's) to be available
		void waitForData() {
			if (_gpCtx != 0) {
				if (_gpCtx->lastFrame < 0) {
					// Haven't run enough frames yet to have data
					_gpCtx->lastFrame = 0;
					return;
				}

				// Wait for data
				while (_ctx->d3dContext->GetData(_gpCtx->disjointQuery[_gpCtx->lastFrame], NULL, 0, 0) == S_FALSE) {
					//Sleep(1);
					std::this_thread::sleep_for(std::chrono::microseconds(200));
				}

				int iFrame = _gpCtx->lastFrame;
				++_gpCtx->lastFrame &= 1;

				D3D11_QUERY_DATA_TIMESTAMP_DISJOINT timestampDisjoint;
				HRESULT hr = _ctx->d3dContext->GetData(_gpCtx->disjointQuery[iFrame], &timestampDisjoint, sizeof(timestampDisjoint), 0);
				if (hr != S_OK) {
					//reportLastError("-- Couldn't retrieve timestamp disjoint query data", hr);
					printf("-- Couldn't retrieve timestamp disjoint query data\n");
					return;
				}

				if (timestampDisjoint.Disjoint) {
					// Throw out this frame's data
					printf("Timestamps disjoint\n");
					return;
				}

				UINT64 timestampPrev;
				if (_ctx->d3dContext->GetData(_gpCtx->timestampQuery[0][iFrame], &timestampPrev, sizeof(UINT64), 0) != S_OK) {
					printf("Couldn't retrieve timestamp query data for GTS 0");
					return;
				}

				for (int gts = 1; gts < _gpCtx->currentMax; ++gts) {
					UINT64 timestamp;
					if (_ctx->d3dContext->GetData(_gpCtx->timestampQuery[gts][iFrame], &timestamp, sizeof(UINT64), 0) != S_OK) {
						printf("Couldn't retrieve timestamp query data for GTS 6\n");
						return;
					}

					_gpCtx->adT[gts] = float(timestamp - timestampPrev) / float(timestampDisjoint.Frequency);
					timestampPrev = timestamp;

					_gpCtx->adTTotalAvg[gts] += _gpCtx->adT[gts];
				}

				++_gpCtx->frameCountAvg;
				if (GetTotalSeconds() > _gpCtx->tBeginAvg + 0.5f) {
					for (int gts = 0; gts < _gpCtx->currentMax; ++gts) {
						_gpCtx->adTAvg[gts] = _gpCtx->adTTotalAvg[gts] / _gpCtx->frameCountAvg;
						_gpCtx->adTTotalAvg[gts] = 0.0f;
					}
					_gpCtx->frameCountAvg = 0;
					_gpCtx->tBeginAvg = GetTotalSeconds();
				}
			}
		}

		float dt(int gts) {
			if (_gpCtx != 0) {
				return _gpCtx->adT[gts];
			}
			return 0.0f;
		}

		float dtAvg(int gts) {
			if (_gpCtx != 0) {
				return _gpCtx->adTAvg[gts];
			}
			return 0.0f;
		}

		float totalTime() {
			float dTDrawTotal = 0.0f;
			if (_gpCtx != 0) {
				for (int gts = 0; gts < _gpCtx->currentMax; ++gts) {
					dTDrawTotal += dtAvg(gts);
				}
			}
			return dTDrawTotal;
		}
		
		void shutdown() {
			if (_gpCtx != 0) {
				for (int i = 0; i < 2; ++i) {
					if (_gpCtx->disjointQuery[i]) {
						_gpCtx->disjointQuery[i]->Release();
					}
					for (int gts = 0; gts < GTS_MAX; ++gts) {
						if (_gpCtx->timestampQuery[gts][i]) {
							_gpCtx->timestampQuery[gts][i]->Release();
						}
					}
				}
			}
		}
	}

	// -----------------------------------------------------
	// CharBuffer
	// -----------------------------------------------------
	CharBuffer::CharBuffer() : data(nullptr), size(0), capacity(0), num(0) {}
	
	CharBuffer::~CharBuffer() {
		if (data != nullptr) {
			delete[] data;
		}
	}

	void* CharBuffer::alloc(int sz) {
		if (size + sz > capacity) {
			int d = capacity * 2 + 8;
			if (d < sz) {
				d = sz * 2 + 8;
			}
			resize(d);
		}
		auto res = data + size;
		size += sz;
		int d = sz / 4;
		if (d == 0) {
			d = 1;
		}
		num += d;
		return res;
	}

	void CharBuffer::resize(int newCap) {
		if (newCap > capacity) {
			char* tmp = new char[newCap];
			if (data != nullptr) {
				memcpy(tmp, data, size);
				delete[] data;
			}
			capacity = newCap;
			data = tmp;
		}
	}

	const char* CharBuffer::get(int index) const {
		return data + index;
	}

	int CharBuffer::append(const char* s, int len) {
		if (size + len + 1 > capacity) {
			resize(capacity + len + 1 + 8);
		}
		const char* t = s;
		int ret = size;
		for (int i = 0; i < len; ++i) {
			data[size++] = *t;
			++t;
		}
		data[size++] = '\0';
		return ret;
	}

	int CharBuffer::append(const char* s) {
		int len = strlen(s);
		return append(s, len);
	}

	int CharBuffer::append(char s) {
		if (size + 1 > capacity) {
			resize(capacity + 9);
		}
		int ret = size;
		data[size++] = s;
		data[size++] = '\0';
		return ret;
	}

	// -----------------------------------------------------
	// Monochrome font - Fixedsys size 20
	// -----------------------------------------------------
	const unsigned char font[128][16] = {
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x60, 0x60, 0x06, 0x1B, 0x3C, 0x6C, 0xC0, 0x01, 0x06, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0xF0, 0x60, 0x06, 0x1B, 0x66, 0x6C, 0x61, 0x03, 0x06, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0xF0, 0x60, 0x86, 0x3F, 0x06, 0xB8, 0x61, 0x03, 0x06, 0x18, 0x60, 0xC0, 0x06, 0x06, 0x00 },
		{ 0x00, 0xF0, 0x00, 0x00, 0x1B, 0x0C, 0xC0, 0xC0, 0x01, 0x00, 0x0C, 0xC0, 0x80, 0x03, 0x06, 0x00 },
		{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x18, 0x60, 0x60, 0x00, 0x00, 0x0C, 0xC0, 0xE0, 0x8F, 0x1F, 0x00 },
		{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x30, 0x30, 0x60, 0x0F, 0x00, 0x0C, 0xC0, 0x80, 0x03, 0x06, 0x00 },
		{ 0x00, 0x00, 0x00, 0x80, 0x3F, 0x60, 0xD8, 0x61, 0x06, 0x00, 0x0C, 0xC0, 0xC0, 0x06, 0x06, 0x00 },
		{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x66, 0x68, 0x63, 0x06, 0x00, 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x3C, 0x60, 0xC3, 0x0D, 0x00, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xC0, 0x01, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x18, 0x78, 0xC0, 0xC0, 0x03, 0x0F, 0x0C, 0xF8, 0x81, 0x83, 0x1F, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x18, 0xCC, 0xE0, 0x60, 0x86, 0x19, 0x0C, 0x18, 0x80, 0x01, 0x18, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0xEC, 0xF8, 0x60, 0x86, 0x19, 0x6C, 0x18, 0xC0, 0x00, 0x0C, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0xEC, 0xC0, 0x00, 0x06, 0x18, 0x6C, 0x18, 0xE0, 0x03, 0x0C, 0x00 },
		{ 0x00, 0xF8, 0x01, 0x00, 0x06, 0xCC, 0xC0, 0x00, 0x03, 0x0E, 0x6C, 0xF8, 0x60, 0x06, 0x06, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x06, 0xDC, 0xC0, 0x80, 0x01, 0x18, 0x66, 0x80, 0x61, 0x06, 0x06, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x03, 0xDC, 0xC0, 0xC0, 0x80, 0x19, 0xFE, 0x80, 0x61, 0x06, 0x03, 0x00 },
		{ 0x38, 0x00, 0x80, 0x03, 0x03, 0xCC, 0xC0, 0x60, 0x80, 0x19, 0x60, 0xC0, 0x60, 0x06, 0x03, 0x00 },
		{ 0x38, 0x00, 0x80, 0x83, 0x01, 0x78, 0xC0, 0xE0, 0x07, 0x0F, 0x60, 0x78, 0xC0, 0x03, 0x03, 0x00 },
		{ 0x30, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x3C, 0xF0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x0F, 0x7E, 0x60, 0xE0, 0x03, 0x0F, 0x00 },
		{ 0x66, 0x98, 0x01, 0x00, 0x00, 0x30, 0x00, 0xC0, 0x80, 0x19, 0xC3, 0xF0, 0x60, 0x86, 0x19, 0x00 },
		{ 0x66, 0x98, 0x81, 0x03, 0x0E, 0x18, 0x00, 0x80, 0x81, 0x19, 0xC3, 0x98, 0x61, 0x86, 0x19, 0x00 },
		{ 0x6E, 0x98, 0x81, 0x03, 0x0E, 0x0C, 0xF8, 0x01, 0x03, 0x0C, 0xF3, 0x98, 0x61, 0x86, 0x01, 0x00 },
		{ 0x3C, 0x98, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x06, 0xDB, 0x98, 0xE1, 0x83, 0x01, 0x00 },
		{ 0x76, 0xF0, 0x01, 0x00, 0x00, 0x0C, 0xF8, 0x01, 0x03, 0x06, 0xDB, 0xF8, 0x61, 0x86, 0x01, 0x00 },
		{ 0x66, 0xC0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x80, 0x01, 0x00, 0xF3, 0x98, 0x61, 0x86, 0x19, 0x00 },
		{ 0x66, 0x60, 0x80, 0x03, 0x0E, 0x30, 0x00, 0xC0, 0x00, 0x06, 0x03, 0x98, 0x61, 0x86, 0x19, 0x00 },
		{ 0x3C, 0x70, 0x80, 0x03, 0x0E, 0x60, 0x00, 0x60, 0x00, 0x06, 0xFE, 0x98, 0xE1, 0x03, 0x0F, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x1E, 0xF8, 0xE1, 0x07, 0x0F, 0x66, 0xF0, 0x00, 0x86, 0x19, 0x06, 0x18, 0x63, 0x0C, 0x0F, 0x00 },
		{ 0x36, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x00, 0x86, 0x19, 0x06, 0x18, 0x63, 0x8C, 0x19, 0x00 },
		{ 0x66, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x00, 0x86, 0x0D, 0x06, 0xB8, 0xE3, 0x8C, 0x19, 0x00 },
		{ 0x66, 0x18, 0x60, 0x80, 0x01, 0x66, 0x60, 0x00, 0x86, 0x0D, 0x06, 0x58, 0xE3, 0x8D, 0x19, 0x00 },
		{ 0x66, 0xF8, 0xE0, 0x83, 0x01, 0x7E, 0x60, 0x00, 0x86, 0x07, 0x06, 0x58, 0x63, 0x8F, 0x19, 0x00 },
		{ 0x66, 0x18, 0x60, 0x80, 0x1D, 0x66, 0x60, 0x00, 0x86, 0x0D, 0x06, 0x58, 0x63, 0x8E, 0x19, 0x00 },
		{ 0x66, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x60, 0x86, 0x0D, 0x06, 0x18, 0x63, 0x8C, 0x19, 0x00 },
		{ 0x36, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x60, 0x86, 0x19, 0x06, 0x18, 0x63, 0x8C, 0x19, 0x00 },
		{ 0x1E, 0xF8, 0x61, 0x00, 0x1F, 0x66, 0xF0, 0xC0, 0x83, 0x19, 0x7E, 0x18, 0x63, 0x0C, 0x0F, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x3E, 0xF0, 0xE0, 0x03, 0x0F, 0x7E, 0x98, 0x61, 0x86, 0x31, 0x66, 0x98, 0xE1, 0x07, 0x0F, 0x00 },
		{ 0x66, 0x98, 0x61, 0x86, 0x19, 0x18, 0x98, 0x61, 0x86, 0x31, 0x66, 0x98, 0x01, 0x06, 0x03, 0x00 },
		{ 0x66, 0x98, 0x61, 0x86, 0x01, 0x18, 0x98, 0x61, 0x86, 0x31, 0x2C, 0x98, 0x01, 0x06, 0x03, 0x00 },
		{ 0x66, 0x98, 0x61, 0x06, 0x03, 0x18, 0x98, 0x61, 0x86, 0x35, 0x18, 0x98, 0x01, 0x03, 0x03, 0x00 },
		{ 0x3E, 0x98, 0xE1, 0x03, 0x06, 0x18, 0x98, 0x61, 0x86, 0x35, 0x18, 0xF0, 0x80, 0x01, 0x03, 0x00 },
		{ 0x06, 0x98, 0x61, 0x03, 0x0C, 0x18, 0x98, 0x61, 0x86, 0x35, 0x34, 0x60, 0xC0, 0x00, 0x03, 0x00 },
		{ 0x06, 0x98, 0x61, 0x06, 0x18, 0x18, 0x98, 0x61, 0x06, 0x1B, 0x66, 0x60, 0x60, 0x00, 0x03, 0x00 },
		{ 0x06, 0x98, 0x61, 0x86, 0x19, 0x18, 0x98, 0xC1, 0x03, 0x1B, 0x66, 0x60, 0x60, 0x00, 0x03, 0x00 },
		{ 0x06, 0xF0, 0x60, 0x06, 0x0F, 0x18, 0xF0, 0x80, 0x01, 0x1B, 0x66, 0x60, 0xE0, 0x07, 0x03, 0x00 },
		{ 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00 },
		{ 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x80, 0x01, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0xC0, 0x03, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x06, 0xF0, 0x60, 0x06, 0x00, 0x30, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x80, 0x07, 0x00, 0x00 },
		{ 0x06, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0xC0, 0x00, 0x00, 0x00 },
		{ 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE0, 0x03, 0x0F, 0x7C, 0xF0, 0xC0, 0x00, 0x1F, 0x00 },
		{ 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x61, 0x86, 0x19, 0x66, 0x98, 0xC1, 0x80, 0x19, 0x00 },
		{ 0x18, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x61, 0x86, 0x01, 0x66, 0x98, 0xE1, 0x87, 0x19, 0x00 },
		{ 0x18, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x61, 0x86, 0x01, 0x66, 0xF8, 0xC1, 0x80, 0x19, 0x00 },
		{ 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x98, 0x61, 0x86, 0x01, 0x66, 0x18, 0xC0, 0x80, 0x19, 0x00 },
		{ 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x98, 0x61, 0x86, 0x19, 0x66, 0x18, 0xC0, 0x80, 0x19, 0x00 },
		{ 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE1, 0x03, 0x0F, 0x7C, 0xF0, 0xC0, 0x00, 0x1F, 0x00 },
		{ 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00 },
		{ 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00 },
		{ 0x00, 0xF0, 0x00, 0xC0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x06, 0x60, 0x00, 0x83, 0x01, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x06, 0x00, 0x00, 0x80, 0x01, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x3E, 0x78, 0xC0, 0x83, 0x19, 0x18, 0xF8, 0xE1, 0x03, 0x0F, 0x3E, 0xF0, 0x61, 0x06, 0x1F, 0x00 },
		{ 0x66, 0x60, 0x00, 0x83, 0x19, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x87, 0x01, 0x00 },
		{ 0x66, 0x60, 0x00, 0x83, 0x0D, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0xE1, 0x80, 0x01, 0x00 },
		{ 0x66, 0x60, 0x00, 0x83, 0x07, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x00, 0x0F, 0x00 },
		{ 0x66, 0x60, 0x00, 0x83, 0x0D, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x00, 0x18, 0x00 },
		{ 0x66, 0x60, 0x00, 0x83, 0x19, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x00, 0x18, 0x00 },
		{ 0x66, 0xF8, 0x01, 0x83, 0x19, 0x7E, 0x18, 0x63, 0x06, 0x0F, 0x3E, 0xF0, 0x61, 0x80, 0x0F, 0x00 },
		{ 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x18, 0x30, 0xE0, 0x88, 0x1F, 0x00 },
		{ 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x18, 0x60, 0xB0, 0x8D, 0x1F, 0x00 },
		{ 0x7E, 0x98, 0x61, 0x86, 0x31, 0x66, 0x98, 0xE1, 0x07, 0x06, 0x18, 0x60, 0x10, 0x87, 0x1F, 0x00 },
		{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x66, 0x98, 0x01, 0x06, 0x06, 0x18, 0x60, 0x00, 0x80, 0x1F, 0x00 },
		{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x3C, 0x98, 0x01, 0x03, 0x03, 0x18, 0xC0, 0x00, 0x80, 0x1F, 0x00 },
		{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x18, 0x98, 0x81, 0x81, 0x01, 0x18, 0x80, 0x01, 0x80, 0x1F, 0x00 },
		{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x3C, 0x98, 0xC1, 0x00, 0x03, 0x18, 0xC0, 0x00, 0x80, 0x1F, 0x00 },
		{ 0x0C, 0x98, 0xC1, 0x03, 0x1B, 0x66, 0x98, 0x61, 0x00, 0x06, 0x18, 0x60, 0x00, 0x80, 0x1F, 0x00 },
		{ 0x78, 0xF0, 0x81, 0x01, 0x1B, 0x66, 0xF0, 0xE0, 0x07, 0x06, 0x18, 0x60, 0x00, 0x80, 0x1F, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x06, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	};

	// -----------------------------------------------------
	// gemoetry shader
	// -----------------------------------------------------
	const BYTE DebugText_GS_Main[] =
	{
		68,  88,  66,  67, 202,  42,
		118, 107, 229,  53, 147, 185,
		2, 218, 254, 203, 198, 198,
		202, 235,   1,   0,   0,   0,
		92,   6,   0,   0,   3,   0,
		0,   0,  44,   0,   0,   0,
		152,   0,   0,   0,  12,   1,
		0,   0,  73,  83,  71,  78,
		100,   0,   0,   0,   3,   0,
		0,   0,   8,   0,   0,   0,
		80,   0,   0,   0,   0,   0,
		0,   0,   1,   0,   0,   0,
		3,   0,   0,   0,   0,   0,
		0,   0,  15,   3,   0,   0,
		92,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		3,   0,   0,   0,   1,   0,
		0,   0,  15,  15,   0,   0,
		92,   0,   0,   0,   1,   0,
		0,   0,   0,   0,   0,   0,
		3,   0,   0,   0,   2,   0,
		0,   0,  15,  15,   0,   0,
		83,  86,  95,  80,  79,  83,
		73,  84,  73,  79,  78,   0,
		67,  79,  76,  79,  82,   0,
		171, 171,  79,  83,  71,  78,
		108,   0,   0,   0,   3,   0,
		0,   0,   8,   0,   0,   0,
		80,   0,   0,   0,   0,   0,
		0,   0,   1,   0,   0,   0,
		3,   0,   0,   0,   0,   0,
		0,   0,  15,   0,   0,   0,
		92,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		3,   0,   0,   0,   1,   0,
		0,   0,   3,  12,   0,   0,
		101,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		3,   0,   0,   0,   2,   0,
		0,   0,  15,   0,   0,   0,
		83,  86,  95,  80,  79,  83,
		73,  84,  73,  79,  78,   0,
		84,  69,  88,  67,  79,  79,
		82,  68,   0,  67,  79,  76,
		79,  82,   0, 171,  83,  72,
		68,  82,  72,   5,   0,   0,
		64,   0,   2,   0,  82,   1,
		0,   0,  89,   0,   0,   4,
		70, 142,  32,   0,   0,   0,
		0,   0,   5,   0,   0,   0,
		97,   0,   0,   5, 242,  16,
		32,   0,   1,   0,   0,   0,
		0,   0,   0,   0,   1,   0,
		0,   0,  95,   0,   0,   4,
		242,  16,  32,   0,   1,   0,
		0,   0,   1,   0,   0,   0,
		95,   0,   0,   4, 242,  16,
		32,   0,   1,   0,   0,   0,
		2,   0,   0,   0, 104,   0,
		0,   2,   3,   0,   0,   0,
		93,   8,   0,   1,  92,  40,
		0,   1, 103,   0,   0,   4,
		242,  32,  16,   0,   0,   0,
		0,   0,   1,   0,   0,   0,
		101,   0,   0,   3,  50,  32,
		16,   0,   1,   0,   0,   0,
		101,   0,   0,   3, 242,  32,
		16,   0,   2,   0,   0,   0,
		94,   0,   0,   2,   4,   0,
		0,   0,   9,   0,   0,   1,
		50,   0,   0,  15, 242,   0,
		16,   0,   0,   0,   0,   0,
		70, 132,  32, 128,  65,   0,
		0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   2,  64,
		0,   0,   0,   0,   0,  63,
		0,   0,   0,  63,   0,   0,
		0,  63,   0,   0,   0,  63,
		70,  20,  32,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		50,   0,   0,  13, 242,   0,
		16,   0,   0,   0,   0,   0,
		230,  30,  32,   0,   0,   0,
		0,   0,   1,   0,   0,   0,
		2,  64,   0,   0,   0,   0,
		0,  63,   0,   0,   0,  63,
		0,   0,   0, 191,   0,   0,
		0, 191, 102,  12,  16,   0,
		0,   0,   0,   0,  54,   0,
		0,   5, 178,   0,  16,   0,
		1,   0,   0,   0, 102,  14,
		16,   0,   0,   0,   0,   0,
		54,   0,   0,   5,  66,   0,
		16,   0,   1,   0,   0,   0,
		1,  64,   0,   0,   0,   0,
		128,  63,  17,   0,   0,   8,
		18,   0,  16,   0,   2,   0,
		0,   0,  70,  10,  16,   0,
		1,   0,   0,   0,  70, 142,
		32,   0,   0,   0,   0,   0,
		1,   0,   0,   0,  54,   0,
		0,   5,  18,  32,  16,   0,
		0,   0,   0,   0,  10,   0,
		16,   0,   2,   0,   0,   0,
		17,   0,   0,   8,  18,   0,
		16,   0,   2,   0,   0,   0,
		70,  10,  16,   0,   1,   0,
		0,   0,  70, 142,  32,   0,
		0,   0,   0,   0,   2,   0,
		0,   0,  54,   0,   0,   5,
		34,  32,  16,   0,   0,   0,
		0,   0,  10,   0,  16,   0,
		2,   0,   0,   0,  54,   0,
		0,   5,  66,  32,  16,   0,
		0,   0,   0,   0,   1,  64,
		0,   0,   0,   0, 128,  63,
		17,   0,   0,   8,  34,   0,
		16,   0,   1,   0,   0,   0,
		70,  10,  16,   0,   1,   0,
		0,   0,  70, 142,  32,   0,
		0,   0,   0,   0,   4,   0,
		0,   0,  54,   0,   0,   5,
		130,  32,  16,   0,   0,   0,
		0,   0,  26,   0,  16,   0,
		1,   0,   0,   0,  56,   0,
		0,  11, 242,   0,  16,   0,
		2,   0,   0,   0,   2,  64,
		0,   0,   0,   0,   0,  60,
		0,   0,   0,  60,   0,   0,
		0,  60,   0,   0,   0,  60,
		70,  30,  32,   0,   0,   0,
		0,   0,   1,   0,   0,   0,
		54,   0,   0,   5,  50,  32,
		16,   0,   1,   0,   0,   0,
		70,   0,  16,   0,   2,   0,
		0,   0,  54,   0,   0,   6,
		242,  32,  16,   0,   2,   0,
		0,   0,  70,  30,  32,   0,
		0,   0,   0,   0,   2,   0,
		0,   0,  19,   0,   0,   1,
		54,   0,   0,   5,  66,   0,
		16,   0,   0,   0,   0,   0,
		1,  64,   0,   0,   0,   0,
		128,  63,  17,   0,   0,   8,
		34,   0,  16,   0,   1,   0,
		0,   0,  70,  10,  16,   0,
		0,   0,   0,   0,  70, 142,
		32,   0,   0,   0,   0,   0,
		1,   0,   0,   0,  54,   0,
		0,   5,  18,  32,  16,   0,
		0,   0,   0,   0,  26,   0,
		16,   0,   1,   0,   0,   0,
		17,   0,   0,   8,  34,   0,
		16,   0,   1,   0,   0,   0,
		70,  10,  16,   0,   0,   0,
		0,   0,  70, 142,  32,   0,
		0,   0,   0,   0,   2,   0,
		0,   0,  54,   0,   0,   5,
		34,  32,  16,   0,   0,   0,
		0,   0,  26,   0,  16,   0,
		1,   0,   0,   0,  54,   0,
		0,   5,  66,  32,  16,   0,
		0,   0,   0,   0,   1,  64,
		0,   0,   0,   0, 128,  63,
		17,   0,   0,   8,  34,   0,
		16,   0,   0,   0,   0,   0,
		70,  10,  16,   0,   0,   0,
		0,   0,  70, 142,  32,   0,
		0,   0,   0,   0,   4,   0,
		0,   0,  54,   0,   0,   5,
		130,  32,  16,   0,   0,   0,
		0,   0,  26,   0,  16,   0,
		0,   0,   0,   0,   0,   0,
		0,   7, 194,   0,  16,   0,
		2,   0,   0,   0, 166,  14,
		16,   0,   2,   0,   0,   0,
		6,   4,  16,   0,   2,   0,
		0,   0,  54,   0,   0,   5,
		18,  32,  16,   0,   1,   0,
		0,   0,  42,   0,  16,   0,
		2,   0,   0,   0,  54,   0,
		0,   5,  34,  32,  16,   0,
		1,   0,   0,   0,  26,   0,
		16,   0,   2,   0,   0,   0,
		54,   0,   0,   6, 242,  32,
		16,   0,   2,   0,   0,   0,
		70,  30,  32,   0,   0,   0,
		0,   0,   2,   0,   0,   0,
		19,   0,   0,   1,  17,   0,
		0,   8,  34,   0,  16,   0,
		0,   0,   0,   0, 198,  10,
		16,   0,   1,   0,   0,   0,
		70, 142,  32,   0,   0,   0,
		0,   0,   1,   0,   0,   0,
		54,   0,   0,   5,  18,  32,
		16,   0,   0,   0,   0,   0,
		26,   0,  16,   0,   0,   0,
		0,   0,  17,   0,   0,   8,
		34,   0,  16,   0,   0,   0,
		0,   0, 198,  10,  16,   0,
		1,   0,   0,   0,  70, 142,
		32,   0,   0,   0,   0,   0,
		2,   0,   0,   0,  17,   0,
		0,   8,  18,   0,  16,   0,
		1,   0,   0,   0, 198,  10,
		16,   0,   1,   0,   0,   0,
		70, 142,  32,   0,   0,   0,
		0,   0,   4,   0,   0,   0,
		54,   0,   0,   5, 130,   0,
		16,   0,   0,   0,   0,   0,
		58,   0,  16,   0,   1,   0,
		0,   0,  54,   0,   0,   5,
		34,  32,  16,   0,   0,   0,
		0,   0,  26,   0,  16,   0,
		0,   0,   0,   0,  54,   0,
		0,   5,  66,  32,  16,   0,
		0,   0,   0,   0,   1,  64,
		0,   0,   0,   0, 128,  63,
		54,   0,   0,   5, 130,  32,
		16,   0,   0,   0,   0,   0,
		10,   0,  16,   0,   1,   0,
		0,   0,  54,   0,   0,   5,
		18,  32,  16,   0,   1,   0,
		0,   0,  10,   0,  16,   0,
		2,   0,   0,   0,  54,   0,
		0,   5,  34,  32,  16,   0,
		1,   0,   0,   0,  58,   0,
		16,   0,   2,   0,   0,   0,
		54,   0,   0,   6, 242,  32,
		16,   0,   2,   0,   0,   0,
		70,  30,  32,   0,   0,   0,
		0,   0,   2,   0,   0,   0,
		19,   0,   0,   1,  17,   0,
		0,   8,  34,   0,  16,   0,
		0,   0,   0,   0, 198,  10,
		16,   0,   0,   0,   0,   0,
		70, 142,  32,   0,   0,   0,
		0,   0,   1,   0,   0,   0,
		54,   0,   0,   5,  18,  32,
		16,   0,   0,   0,   0,   0,
		26,   0,  16,   0,   0,   0,
		0,   0,  17,   0,   0,   8,
		34,   0,  16,   0,   0,   0,
		0,   0, 198,  10,  16,   0,
		0,   0,   0,   0,  70, 142,
		32,   0,   0,   0,   0,   0,
		2,   0,   0,   0,  17,   0,
		0,   8,  18,   0,  16,   0,
		0,   0,   0,   0, 198,  10,
		16,   0,   0,   0,   0,   0,
		70, 142,  32,   0,   0,   0,
		0,   0,   4,   0,   0,   0,
		54,   0,   0,   5,  34,  32,
		16,   0,   0,   0,   0,   0,
		26,   0,  16,   0,   0,   0,
		0,   0,  54,   0,   0,   5,
		66,  32,  16,   0,   0,   0,
		0,   0,   1,  64,   0,   0,
		0,   0, 128,  63,  54,   0,
		0,   5, 130,  32,  16,   0,
		0,   0,   0,   0,  10,   0,
		16,   0,   0,   0,   0,   0,
		54,   0,   0,   5,  50,  32,
		16,   0,   1,   0,   0,   0,
		230,  10,  16,   0,   2,   0,
		0,   0,  54,   0,   0,   6,
		242,  32,  16,   0,   2,   0,
		0,   0,  70,  30,  32,   0,
		0,   0,   0,   0,   2,   0,
		0,   0,  19,   0,   0,   1,
		9,   0,   0,   1,  62,   0,
		0,   1
	};

	// -----------------------------------------------------
	// pixel shader
	// -----------------------------------------------------
	const BYTE DebugText_PS_Main[] =
	{
		68,  88,  66,  67, 133, 123,
		244, 109,  36, 101, 150, 228,
		91, 135, 209, 221,  54, 143,
		33,  28,   1,   0,   0,   0,
		112,   1,   0,   0,   3,   0,
		0,   0,  44,   0,   0,   0,
		160,   0,   0,   0, 212,   0,
		0,   0,  73,  83,  71,  78,
		108,   0,   0,   0,   3,   0,
		0,   0,   8,   0,   0,   0,
		80,   0,   0,   0,   0,   0,
		0,   0,   1,   0,   0,   0,
		3,   0,   0,   0,   0,   0,
		0,   0,  15,   0,   0,   0,
		92,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		3,   0,   0,   0,   1,   0,
		0,   0,   3,   3,   0,   0,
		101,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		3,   0,   0,   0,   2,   0,
		0,   0,  15,  15,   0,   0,
		83,  86,  95,  80,  79,  83,
		73,  84,  73,  79,  78,   0,
		84,  69,  88,  67,  79,  79,
		82,  68,   0,  67,  79,  76,
		79,  82,   0, 171,  79,  83,
		71,  78,  44,   0,   0,   0,
		1,   0,   0,   0,   8,   0,
		0,   0,  32,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		0,   0,   3,   0,   0,   0,
		0,   0,   0,   0,  15,   0,
		0,   0,  83,  86,  95,  84,
		65,  82,  71,  69,  84,   0,
		171, 171,  83,  72,  68,  82,
		148,   0,   0,   0,  64,   0,
		0,   0,  37,   0,   0,   0,
		90,   0,   0,   3,   0,  96,
		16,   0,   0,   0,   0,   0,
		88,  24,   0,   4,   0, 112,
		16,   0,   0,   0,   0,   0,
		85,  85,   0,   0,  98,  16,
		0,   3,  50,  16,  16,   0,
		1,   0,   0,   0,  98,  16,
		0,   3, 242,  16,  16,   0,
		2,   0,   0,   0, 101,   0,
		0,   3, 242,  32,  16,   0,
		0,   0,   0,   0, 104,   0,
		0,   2,   1,   0,   0,   0,
		69,   0,   0,   9, 242,   0,
		16,   0,   0,   0,   0,   0,
		70,  16,  16,   0,   1,   0,
		0,   0,  70, 126,  16,   0,
		0,   0,   0,   0,   0,  96,
		16,   0,   0,   0,   0,   0,
		56,   0,   0,   7, 242,  32,
		16,   0,   0,   0,   0,   0,
		70,  14,  16,   0,   0,   0,
		0,   0,  70,  30,  16,   0,
		2,   0,   0,   0,  62,   0,
		0,   1
	};

	// -----------------------------------------------------
	// vertex shader
	// -----------------------------------------------------
	const BYTE DebugText_VS_Main[] =
	{
		68,  88,  66,  67, 190,  97,
		0,  55,  77, 152, 190,  30,
		20, 134, 117, 232, 251, 207,
		230, 229,   1,   0,   0,   0,
		176,   1,   0,   0,   3,   0,
		0,   0,  44,   0,   0,   0,
		148,   0,   0,   0,   0,   1,
		0,   0,  73,  83,  71,  78,
		96,   0,   0,   0,   3,   0,
		0,   0,   8,   0,   0,   0,
		80,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		3,   0,   0,   0,   0,   0,
		0,   0,   7,   7,   0,   0,
		89,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		3,   0,   0,   0,   1,   0,
		0,   0,  15,  15,   0,   0,
		89,   0,   0,   0,   1,   0,
		0,   0,   0,   0,   0,   0,
		3,   0,   0,   0,   2,   0,
		0,   0,  15,  15,   0,   0,
		80,  79,  83,  73,  84,  73,
		79,  78,   0,  67,  79,  76,
		79,  82,   0, 171,  79,  83,
		71,  78, 100,   0,   0,   0,
		3,   0,   0,   0,   8,   0,
		0,   0,  80,   0,   0,   0,
		0,   0,   0,   0,   1,   0,
		0,   0,   3,   0,   0,   0,
		0,   0,   0,   0,  15,   0,
		0,   0,  92,   0,   0,   0,
		0,   0,   0,   0,   0,   0,
		0,   0,   3,   0,   0,   0,
		1,   0,   0,   0,  15,   0,
		0,   0,  92,   0,   0,   0,
		1,   0,   0,   0,   0,   0,
		0,   0,   3,   0,   0,   0,
		2,   0,   0,   0,  15,   0,
		0,   0,  83,  86,  95,  80,
		79,  83,  73,  84,  73,  79,
		78,   0,  67,  79,  76,  79,
		82,   0, 171, 171,  83,  72,
		68,  82, 168,   0,   0,   0,
		64,   0,   1,   0,  42,   0,
		0,   0,  95,   0,   0,   3,
		114,  16,  16,   0,   0,   0,
		0,   0,  95,   0,   0,   3,
		242,  16,  16,   0,   1,   0,
		0,   0,  95,   0,   0,   3,
		242,  16,  16,   0,   2,   0,
		0,   0, 103,   0,   0,   4,
		242,  32,  16,   0,   0,   0,
		0,   0,   1,   0,   0,   0,
		101,   0,   0,   3, 242,  32,
		16,   0,   1,   0,   0,   0,
		101,   0,   0,   3, 242,  32,
		16,   0,   2,   0,   0,   0,
		54,   0,   0,   5, 114,  32,
		16,   0,   0,   0,   0,   0,
		70,  18,  16,   0,   0,   0,
		0,   0,  54,   0,   0,   5,
		130,  32,  16,   0,   0,   0,
		0,   0,   1,  64,   0,   0,
		0,   0, 128,  63,  54,   0,
		0,   5, 242,  32,  16,   0,
		1,   0,   0,   0,  70,  30,
		16,   0,   1,   0,   0,   0,
		54,   0,   0,   5, 242,  32,
		16,   0,   2,   0,   0,   0,
		70,  30,  16,   0,   2,   0,
		0,   0,  62,   0,   0,   1
	};

	

	// -----------------------------------------------------
	// init
	// -----------------------------------------------------
	static void dbgInit() {
		_ctx->debugItemCount = 0;
		//
		// build texture data
		//
		uint8_t* data = new uint8_t[128 * 128 * 4];
		int index = 0;
		for (int y = 0; y < 128; ++y) {
			for (int x = 0; x < 16; ++x) {
				uint8_t t = font[y][x];
				for (int z = 0; z < 8; ++z) {
					if ((t >> z) & 1) {
						data[index++] = 255;
						data[index++] = 255;
						data[index++] = 255;
						data[index++] = 255;
					}
					else {
						data[index++] = 0;
						data[index++] = 0;
						data[index++] = 0;
						data[index++] = 0;
					}
				}
			}
		}
		//
		// create resources
		//
		TextureInfo info = { 128,128,4,data,TextureFormat::R8G8B8A8_UNORM ,BindFlag::BF_SHADER_RESOURCE };
		_ctx->debugTextureID = createTexture(info, "DebugTextFont");
		delete[] data; // we do not need this anymore
		RID vertexShader = createVertexShader(DebugText_VS_Main, sizeof(DebugText_VS_Main), "DebugVS");
		RID pixelShader = createPixelShader(DebugText_PS_Main, sizeof(DebugText_PS_Main), "DebugPS");
		RID geoShader = createGeometryShader(DebugText_GS_Main, sizeof(DebugText_GS_Main), "DebugGS");
		ds::InputLayoutDefinition decl[] = {
			{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 },
			{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
		};
		InputLayoutInfo layoutInfo = { decl, 3, vertexShader };
		RID vertexDeclId = createInputLayout( layoutInfo, "PCC_Layout");
		RID cbid = createConstantBuffer(sizeof(DebugTextConstantBuffer), &_ctx->debugConstantBuffer, "DebugTextConstantBuffer");
		ds::VertexBufferInfo vbInfo = { BufferType::DYNAMIC, MAX_DBG_TXT_VERTICES, sizeof(DebugTextVertex), 0 };
		_ctx->debugVertexBufferID = createVertexBuffer(vbInfo, "DebugTextVertexBuffer");
		SamplerStateInfo samplerInfo = { TextureAddressModes::CLAMP, TextureFilters::POINT };
		RID ssid = createSamplerState(samplerInfo);

		//
		// create state group
		//
		RID debugTextStateGroup = StateGroupBuilder()
			.inputLayout(vertexDeclId)
			.vertexBuffer(_ctx->debugVertexBufferID)
			.indexBuffer(NO_RID)
			.vertexShader(vertexShader)
			.geometryShader(geoShader)
			.pixelShader(pixelShader)
			.constantBuffer(cbid, geoShader, 0)
			.texture(_ctx->debugTextureID, pixelShader, 0)
			.samplerState(ssid, pixelShader)
			.build();

		vec2 textureSize = ds::getTextureSize(_ctx->debugTextureID);
		_ctx->debugConstantBuffer.screenDimension = vec4(ds::getScreenWidth(), ds::getScreenHeight(), textureSize.x, textureSize.y);
		//
		// create draw command
		//
		DrawCommand drawCmd = { 100, DrawType::DT_VERTICES, PrimitiveTypes::POINT_LIST, 0 };
		_ctx->debugDrawItem = compile(drawCmd, debugTextStateGroup, "DebugText");
		//
		// build ortho view and render pass
		//
		matrix orthoView = matIdentity();
		matrix orthoProjection = matOrthoLH(getScreenWidth(), getScreenHeight(), 0.1f, 1.0f);
		_ctx->orthoCamera = {
			orthoView,
			orthoProjection,
			orthoView * orthoProjection,
			ds::vec3(0,0,0),
			ds::vec3(0,0,0),
			ds::vec3(0,1,0),
			ds::vec3(1,0,0),
			0.0f,
			0.0f,
			0.0f
		};
		RenderPassInfo orthoRP = { &_ctx->orthoCamera,DepthBufferState::DISABLED, 0, 0 };
		_ctx->debugOrthoPass = createRenderPass(orthoRP, "DebugTextOrthoPass");
		_ctx->debugConstantBuffer.wvp = matTranspose(orthoView * orthoProjection);
	}

	// -----------------------------------------------------
	// begin
	// -----------------------------------------------------
	void dbgBegin() {
		_ctx->debugItemCount = 0;
	}

	// -----------------------------------------------------
	// internal add
	// -----------------------------------------------------
	static void add(uint16_t x, uint16_t y, const ds::vec4& texture, const ds::Color color) {
		if ((_ctx->debugItemCount + 1) >= MAX_DBG_TXT_VERTICES) {
			dbgFlush();
		}
		_ctx->debugVertices[_ctx->debugItemCount++] = { vec3(x,y,0),texture,color };
	}

	// -----------------------------------------------------
	// add
	// -----------------------------------------------------
	static void add(uint16_t xp, uint16_t yp, const char* message) {
		xp *= 10;
		xp += 10;
		yp = getScreenHeight() - 20 - yp * 20;
		int l = strlen(message);
		for (int i = 0; i < l; ++i) {
			int c = message[i] - 32;
			int t = c / 12;
			int y = t * 16;
			int x = (c - t * 12) * 10;
			add(xp + 10 * i, yp, vec4(x, y, 8, 16), Color(255, 255, 255, 255));
		}
	}

	// -----------------------------------------------------
	// format
	// -----------------------------------------------------
	void dbgPrint(uint16_t x, uint16_t y, char* format, ...) {
		if (_ctx->supportDebug) {
			va_list args;
			va_start(args, format);
			char buffer[1024];
			memset(buffer, 0, sizeof(buffer));
			int written = vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, args);
			add(x, y, buffer);
			va_end(args);
		}
	}

	// -----------------------------------------------------
	// flush
	// -----------------------------------------------------
	void dbgFlush() {
		if (_ctx->debugItemCount > 0) {
			mapBufferData(_ctx->debugVertexBufferID, _ctx->debugVertices, _ctx->debugItemCount * sizeof(DebugTextVertex));
			submit(_ctx->debugOrthoPass, _ctx->debugDrawItem, _ctx->debugItemCount);
			_ctx->debugItemCount = 0;
		}
	}

	// ----------------------------------------------------
	// Static Hash 
	// ----------------------------------------------------
	StaticHash::StaticHash() {
		_hash = 0;
	}
	StaticHash::StaticHash(const char* text) {
		_hash = fnv1a(text);
	}
	StaticHash::StaticHash(uint32_t hash) {
		_hash = hash;
	}
	const uint32_t StaticHash::get() const {
		return _hash;
	}
	const bool StaticHash::operator<(const StaticHash &rhs) const {
		return _hash < rhs.get();
	}

	const StaticHash INVALID_HASH = StaticHash(static_cast<unsigned int>(0));

}
#endif
	