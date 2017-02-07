#pragma once
//-------------------------------------------------------------------------
//
// File:		Vector.h
// Desc:		A template vector implementation
// Author:		amecky <amecky@gmail.com>
// License:     BSD - do whatever you want
//
//-------------------------------------------------------------------------
#pragma once
#include <cmath>

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

template<int Size, class T>
bool operator == (const Vector<Size, T>& u, const Vector<Size, T>& v);

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

matrix operator * (const matrix& m1, const matrix& m2);

#ifdef MATH_IMPLEMENTATION
//! The == operator
/*! The compound == operator will return true if both vectors are equals
\param u first vector
\param v second vector
\return true if equals
*/
template<int Size, class T>
bool operator == (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	for (int i = 0; i < Size; ++i) {
		if (u.data[i] != v.data[i]) {
			return false;
		}
	}
	return true;
}

//! The != operator
/*! The compound != operator will return true if both vectors are not equals
\param u first vector
\param v second vector
\return true if not equals
*/
template<int Size, class T>
bool operator != (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	for (int i = 0; i < Size; ++i) {
		if (u.data[i] != v.data[i]) {
			return true;
		}
	}
	return false;
}

/*! Unary operator - which will change the sign on every member
\param u the first vector
\return a vector with the result
*/
template<int Size, class T>
Vector<Size, T> operator - (const Vector<Size, T>& v) {
	Vector<Size, T> ret;
	for (int i = 0; i < Size; ++i) {
		ret.data[i] = -v.data[i];
	}
	return ret;
}


/*! Compound operator += which will add the second to the first vector
\param u the first vector
\param v the vector that will be added to the first one
\return a vector with the result
*/
template<int Size, class T>
Vector<Size, T> operator += (Vector<Size, T>& u, const Vector<Size, T>& v) {
	for (int i = 0; i < Size; ++i) {
		u.data[i] += v.data[i];
	}
	return u;
}

/*! Compound operator += which will add the second to the first vector
\param u the first vector
\param v the vector that will be added to the first one
\return a vector with the result
*/
template<int Size, class T>
Vector<Size, T> operator += (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> r;
	for (int i = 0; i < Size; ++i) {
		r.data[i] = u.data[i] + v.data[i];
	}
	return r;
}

// -------------------------------------------------------
// operator *=
// -------------------------------------------------------
template<int Size, class T>
Vector<Size, T> operator *= (Vector<Size, T>& u, T other) {
	for (int i = 0; i < Size; ++i) {
		u.data[i] *= other;
	}
	return u;
}

// -------------------------------------------------------
// operator /=
// -------------------------------------------------------
template<int Size, class T>
Vector<Size, T>& operator /= (Vector<Size, T>& u, T other) {
	for (int i = 0; i < Size; ++i) {
		u.data[i] /= other;
	}
	return u;
}

/*! Compound operator -= which will subtract the second to the first vector
\param u the first vector
\param v the vector that will be subtracted to the first one
\return a vector with the result
*/
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
// -------------------------------------------------------
// operator +
// -------------------------------------------------------
template<int Size, class T>
Vector<Size, T> operator + (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> ret = u;
	return ret += v;
}

// -------------------------------------------------------
// operator -
// -------------------------------------------------------
template<int Size, class T>
Vector<Size, T> operator - (const Vector<Size, T>& u, const Vector<Size, T>& v) {
	Vector<Size, T> ret = u;
	return ret -= v;
}

/*! Multiplies a vector by a scalar
\param u the vector
\param v the scalar
\return a vector with the result
*/
template<int Size, class T>
Vector<Size, T> operator * (const Vector<Size, T>& u, const T& v) {
	Vector<Size, T> ret = u;
	return ret *= v;
}

/*! Multiplies a vector by a scalar
\param u the vector
\param v the scalar
\return a vector with the result
*/
template<int Size, class T>
Vector<Size, T> operator * (const T& v, const Vector<Size, T>& u) {
	Vector<Size, T> ret = u;
	return ret *= v;
}

/*! Divides a vector by a scalar
\param u the vector
\param v the scalar
\return a vector with the result
*/
template<int Size, class T>
Vector<Size, T> operator / (const Vector<Size, T>& u, const T& v) {
	Vector<Size, T> ret = u;
	return ret /= v;
}

/*! Dot product of two vectors
\param u first vector
\param v second vector
\return the dot product as scalar
*/
template<int Size, class T>
T dot(const Vector<Size, T>& v, const Vector<Size, T>& u) {
	T t(0);
	for (int i = 0; i < Size; ++i) {
		t += v.data[i] * u.data[i];
	}
	return t;
}


/*! Calculates the length of a vector
\param v the vector
\return the length of a vector as scalar
*/
template<int Size, class T>
T length(const Vector<Size, T>& v) {
	T t = dot(v, v);
	float tmp = std::sqrt(static_cast<float>(t));
	return static_cast<T>(tmp);
}

/*! Calculates the squared length of a vector. Sometimes
it can be handy to avoid the sqrt of the regular length
method and the sqr_length will also work.
\param v The vector
\return the square length
*/
template<int Size, class T>
T sqr_length(const Vector<Size, T>& v) {
	return dot(v, v);
}

/*! Calculates a normalized vector .
\param u The vector that will be normalized
\return A normalized vector
*/
template<int Size, class T>
Vector<Size, T> normalize(const Vector<Size, T>& u) {
	T len = length(u);
	if (len == 0.0f) {
		return Vector<Size, T>();
	}
	return u / len;
}

/*! Calculates a normalized vector .
\param u The vector that will be normalized
\param ret a pointer to a Vector that will contain the normalized values
\return A pointer to a normalized vector
*/
template<int Size, class T>
Vector<Size, T>* normalize(const Vector<Size, T>& u, Vector<Size, T>* ret) {
	T len = length(u);
	for (int i = 0; i < Size; ++i) {
		ret->data[i] /= len;
	}
	return ret;
}

/*! Calculates the distance between to vectors
\param u first vector
\param v second vector
\return the distance
*/
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

/*! Cross product of two vectors
\param u first vector
\param v second vector
\return a vector containing the cross product
*/
template<class T>
Vector<3, T> cross(const Vector<3, T>& u, const Vector<3, T>& v) {
	T x = u.y * v.z - u.z * v.y;
	T y = u.z * v.x - u.x * v.z;
	T z = u.x * v.y - u.y * v.x;
	return Vector<3, T>(x, y, z);
}

/*! Cross product of two vectors
\param u first vector
\param v second vector
\param ret a pointer to a vector which will contain the cross product
\return a pointer to a vector
*/
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

/*! Linear interpolation of two vectors of the type float.
\param u first vector
\param v second vector
\param t the normalized time. It will be clamped to 0 to 1.
\param ret a pointer to a vector that will be updated
\return a pointer to vector containing the interpolation
*/
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

/*! Linear interpolation of two vectors of the type float.
\param u first vector
\param v second vector
\param t the normalized time. It will be clamped to 0 to 1.
\return a vector containing the interpolation
*/
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
	//mat4 tmp = m4identity();
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

#endif
