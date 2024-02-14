#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <vector>
#include <iostream>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class t> struct Vec2 {
	union {
		struct { t u, v; };
		struct { t x, y; };
		t raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(t _u, t _v) : u(_u), v(_v) {}
	inline Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(u + V.u, v + V.v); }
	inline Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(u - V.u, v - V.v); }
	inline Vec2<t> operator *(float f)          const { return Vec2<t>(u * f, v * f); }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
	union {
		struct { t x, y, z; };
		struct { t ivert, iuv, inorm; };
		t raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
	inline Vec3<t> operator ^(const Vec3<t>& v) const { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	inline Vec3<t> operator +(const Vec3<t>& v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
	inline Vec3<t> operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
	inline Vec3<t> operator *(float f)          const { return Vec3<t>(x * f, y * f, z * f); }
	inline t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; }
	inline t       operator [](const int i) const 
	{
		if (i == 0) return x;
		if (i == 1) return y;
		if (i == 2) return z;
		return NULL;
	}
	Vec3<t> mul(const Vec3<t>& v) const { return Vec3<t>(x * v.x, y * v.y, z * v.z); }
	float norm() const { return std::sqrt(x * x + y * y + z * z); }
	Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

template <class t> struct Vec4 {
	union {
		struct { t x, y, z, w; };
		struct { t ivert, iuv, inorm; };
		t raw[4];
	};
	Vec4() : x(0), y(0), z(0), w(0) {}
	Vec4(t _x, t _y, t _z, t _w) : x(_x), y(_y), z(_z), w(_w) {}
	inline Vec4<t> operator +(const Vec4<t>& v) const { return Vec4<t>(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline Vec4<t> operator -(const Vec4<t>& v) const { return Vec4<t>(x - v.x, y - v.y, z - v.z, w - v.w); }
	inline Vec4<t> operator *(float f)          const { return Vec4<t>(x * f, y * f, z * f, w * f); }
	inline t       operator *(const Vec4<t>& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
	inline t       operator [](const int i) const
	{
		if (i == 0) return x;
		if (i == 1) return y;
		if (i == 2) return z;
		if (i == 3) return w;
		return NULL;
	}
	float norm() const { return std::sqrt(x * x + y * y + z * z + w * w); }
	Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec4<t>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;
typedef Vec4<float> Vec4f;
typedef Vec4<int>   Vec4i;

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
	s << "(" << v.x << ", " << v.y << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec4<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w <<  ")\n";
	return s;
}

template <class t> Vec2<t> operator*(float f, Vec2<t>& v) { return v * f; }
template <class t> Vec3<t> operator*(float f, Vec3<t>& v) { return v * f; }
template <class t> Vec4<t> operator*(float f, Vec4<t>& v) { return v * f; }

//////////////////////////////////////////////////////////////////////////////////////////////

const int DEFAULT_ALLOC = 4;

class Matrix {
	std::vector<std::vector<float> > m;
	int rows, cols;
public:
	Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
	inline int nrows();
	inline int ncols();

	static Matrix identity(int dimensions);
	std::vector<float>& operator[](const int i);
	Vec3f operator*(const Vec3f v);
	Matrix operator*(const Matrix& a);
	Matrix operator*(const float a);
	Matrix transpose();
	Matrix inverse();

	friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

/////////////////////////////////////////////////////////////////////////////////////////////

float cross(Vec2i a, Vec2i b);

Vec3f cross(const Vec3f& v1, const Vec3f& v2);

template <typename T>
T clamp(T x, T a, T b)
{
	if (x < a)
	{
		return a;
	}

	if (x > b)
	{
		return b;
	}

	return x;
}

bool isInTriangle(Vec2i* pts, Vec2i p);

Vec3f barycentric2D(Vec2i* pts, Vec2i P);

Matrix viewport(int x, int y, int w, int h, int depth);

Matrix lookat(Vec3f eye, Vec3f center, Vec3f up);

#endif //__GEOMETRY_H__