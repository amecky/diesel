#pragma once
#include <math.h>


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
	Vector<Size, T> operator -= (const Vector<Size, T>& u, const Vector<Size, T>& v) {
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

#ifdef DS_MATH_IMPLEMENTATION


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

#endif

