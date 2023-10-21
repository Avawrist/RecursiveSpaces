// ======================================================================
// Title: mdcla.cpp
// Description: The implementation file for my personal linear algebra library.
// ======================================================================

#include "mdcla.hpp"

//////////////////
// Struct Vec2F //
//////////////////

Vec2F::Vec2F()
{
    x = 0;
    y = 0;
}

Vec2F::Vec2F(float _x, float _y)
{
    x = _x;
    y = _y;
}

Vec2F Vec2F::operator /(float d)
{
    d = 1.0f / d;
    
    return Vec2F(x * d,
	         y * d);
}

void Vec2F::operator /=(float d)
{
    d = 1.0f / d;
    
    x *= d;
    y *= d;
}

void Vec2F::operator *=(float s)
{
    x *= s;
    y *= s;
}

void Vec2F::operator +=(const Vec2F& v)
{
    x += v.x;
    y += v.y;
}

void Vec2F::operator -=(const Vec2F& v)
{
    x -= v.x;
    y -= v.y;
}

float& Vec2F::operator [](int i)
{
    return *(&x + i);
}

c_float& Vec2F::operator [](int i) const
{
    return *(&x + i);
}

// Vec2F Non-members

bool operator ==(const Vec2F& a, const Vec2F& b)
{
    return(a.x == b.x && a.y == b.y);
}

Vec2F operator +(const Vec2F& a, const Vec2F& b)
{
    return Vec2F(a.x + b.x,
	         a.y + b.y);
}

Vec2F operator -(const Vec2F& a, const Vec2F& b)
{
    return Vec2F(a.x - b.x,
	         a.y - b.y);
}

Vec2F operator *(const Vec2F& v, float s)
{
    return Vec2F(v.x * s,
	         v.y * s);
}

Vec2F operator *(float s, const Vec2F& v)
{
    return Vec2F(v.x * s,
	         v.y * s);
}

float dot(const Vec2F& a, const Vec2F& b)
{
    return ((a.x * b.x) +
	    (a.y * b.y));
}

float dot(const Vec2F& v)
{
    return ((v.x * v.x) +
	    (v.y * v.y));
}

float magnitude(const Vec2F& v)
{
    return sqrt(dot(v));
}

Vec2F normalize(const Vec2F& v)
{
    return (v * (1.0f / magnitude(v)));
}

void print(const Vec2F& v)
{
    printf("(%f, %f)\n", v.x, v.y);
}

//////////////////
// Struct Vec3F //
//////////////////

Vec3F::Vec3F()
{
    x = 0;
    y = 0;
    z = 0;
}

Vec3F::Vec3F(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

Vec3F Vec3F::operator /(float d)
{
    d = 1.0f / d;
    
    return Vec3F(x * d,
	         y * d,
		 z * d);
}

void Vec3F::operator /=(float d)
{
    d = 1.0f / d;
    
    x *= d;
    y *= d;
    z *= d;
}

void Vec3F::operator *=(float s)
{
    x *= s;
    y *= s;
    z *= s;
}

void Vec3F::operator +=(const Vec3F& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vec3F::operator -=(const Vec3F& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

float& Vec3F::operator [](int i)
{
    return *(&x + i);
}

c_float& Vec3F::operator [](int i) const
{
    return *(&x + i);
}

// Vec3F Non-Members

bool operator ==(const Vec3F& a, const Vec3F& b)
{
    return(a.x == b.x && a.y == b.y && a.z == b.z);
}

Vec3F operator +(const Vec3F& a, const Vec3F& b)
{
    return Vec3F(a.x + b.x,
	         a.y + b.y,
	         a.z + b.z);
}

Vec3F operator -(const Vec3F& a, const Vec3F& b)
{
    return Vec3F(a.x - b.x,
	         a.y - b.y,
	         a.z - b.z);
}

Vec3F operator *(const Vec3F& v, float s)
{
    return Vec3F(v.x * s,
	         v.y * s,
	         v.z * s);
}

Vec3F operator *(float s, const Vec3F& v)
{
    return Vec3F(v.x * s,
	         v.y * s,
	         v.z * s);
}

Vec3F cross(const Vec3F& a, const Vec3F& b)
{
    return Vec3F((a.y * b.z) - (a.z * b.y),
		 (a.z * b.x) - (a.x * b.z),
		 (a.x * b.y) - (a.y * b.x));
}

float dot(const Vec3F& a, const Vec3F& b)
{
    return ((a.x * b.x) +
	    (a.y * b.y) +
	    (a.z * b.z));
}

float dot(const Vec3F& v)
{
    return ((v.x * v.x) +
	    (v.y * v.y) +
	    (v.z * v.z));
}

float magnitude(const Vec3F& v)
{
    return (sqrt(dot(v)));
}

Vec3F normalize(const Vec3F& v)
{
    return (v * (1.0f / magnitude(v)));
}

void print(const Vec3F& v)
{
    printf("(%f, %f, %f)\n", v.x, v.y, v.z);
}

//////////////////
// Struct Vec4F //
//////////////////

Vec4F::Vec4F()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vec4F::Vec4F(float _x, float _y, float _z, float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

Vec4F Vec4F::operator /(float d)
{
    d = 1.0f / d;
    
    return Vec4F(x * d,
	         y * d,
		 z * d,
	         w * d);
}

void Vec4F::operator /=(float d)
{
    d = 1.0f / d;
    
    x *= d;
    y *= d;
    z *= d;
    w *= d;
}

void Vec4F::operator *=(float s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
}

void Vec4F::operator +=(const Vec4F& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
}

void Vec4F::operator -=(const Vec4F& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}

float& Vec4F::operator [](int i)
{
    return *(&x + i);
}

c_float& Vec4F::operator [](int i) const
{
    return *(&x + i);
}

// Vec4F Non-Members

bool operator ==(const Vec4F& a, const Vec4F& b)
{
    return(a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

Vec4F operator +(const Vec4F& a, const Vec4F& b)
{
    return Vec4F(a.x + b.x,
	         a.y + b.y,
	         a.z + b.z,
	         a.w + b.w);
}

Vec4F operator -(const Vec4F& a, const Vec4F& b)
{
    return Vec4F(a.x - b.x,
	         a.y - b.y,
	         a.z - b.z,
	         a.w - b.w);
}

Vec4F operator *(const Vec4F& v, float s)
{
    return Vec4F(v.x * s,
	         v.y * s,
	         v.z * s,
	         v.w * s);
}

Vec4F operator *(float s, const Vec4F& v)
{
    return Vec4F(v.x * s,
	         v.y * s,
	         v.z * s,
	         v.w * s);
}
 
float dot(const Vec4F& a, const Vec4F& b)
{
    return ((a.x * b.x) +
	    (a.y * b.y) +
	    (a.z * b.z) +
	    (a.w * b.w));
}

float dot(const Vec4F& v)
{
    return ((v.x * v.x) +
	    (v.y * v.y) +
	    (v.z * v.z) +
	    (v.w * v.w));
}

float magnitude(const Vec4F& v)
{
    return(sqrt(dot(v)));
}

Vec4F normalize(const Vec4F& v)
{
    return (v * (1.0f / magnitude(v)));
}

void print(const Vec4F& v)
{
    printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
}

//////////////////
// Struct Mat3F //
//////////////////

Mat3F::Mat3F()
{
    for(int x = 0; x < 3; x++)
    {
	for(int y = 0; y < 3; y++)
	{
	    n[x][y] = 0.0f;
	}
    }    
}

Mat3F::Mat3F(float n00, float n01, float n02,
	     float n10, float n11, float n12,
	     float n20, float n21, float n22)
{
    // Column Major
    n[0][0] = n00; n[0][1] = n10; n[0][2] = n20; 
    n[1][0] = n01; n[1][1] = n11; n[1][2] = n21;
    n[2][0] = n02; n[2][1] = n12; n[2][2] = n22;
}

Mat3F::Mat3F(float f)
{
    n[0][0] = f;    n[0][1] = 0.0f; n[0][2] = 0.0f; 
    n[1][0] = 0.0f; n[1][1] = f;    n[1][2] = 0.0f;
    n[2][0] = 0.0f; n[2][1] = 0.0f; n[2][2] = f;
}

Mat3F::Mat3F(const Vec3F& a, const Vec3F& b, const Vec3F& c)
{
    // Column Major
    n[0][0] = a.x; n[0][1] = b.x; n[0][2] = c.x; 
    n[1][0] = a.y; n[1][1] = b.y; n[1][2] = c.y;
    n[2][0] = a.z; n[2][1] = b.z; n[2][2] = c.z;
}

Mat3F::Mat3F(const Mat3F& m)
{
    // Column Major
    n[0][0] = m(0, 0); n[0][1] = m(0, 1); n[0][2] = m(0, 2);
    n[1][0] = m(1, 0); n[1][1] = m(1, 1); n[1][2] = m(1, 2);
    n[2][0] = m(2, 0); n[2][1] = m(2, 1); n[2][2] = m(2, 2);
}

void Mat3F::operator *=(float s)
{
    n[0][0] *= s; n[0][1] *= s; n[0][2] *= s;
    n[1][0] *= s; n[1][1] *= s; n[1][2] *= s;
    n[2][0] *= s; n[2][1] *= s; n[2][2] *= s;
}

void Mat3F::operator +=(const Mat3F& m)
{
    n[0][0] += m(0, 0); n[0][1] += m(0, 1); n[0][2] += m(0, 2);
    n[1][0] += m(1, 0); n[1][1] += m(1, 1); n[1][2] += m(1, 2);
    n[2][0] += m(2, 0); n[2][1] += m(2, 1); n[2][2] += m(2, 2);
}

void Mat3F::operator -=(const Mat3F& m)
{
    n[0][0] -= m(0, 0); n[0][1] -= m(0, 1); n[0][2] -= m(0, 2);
    n[1][0] -= m(1, 0); n[1][1] -= m(1, 1); n[1][2] -= m(1, 2);
    n[2][0] -= m(2, 0); n[2][1] -= m(2, 1); n[2][2] -= m(2, 2);
}

float& Mat3F::operator ()(int i, int j)
{
    // Column Major
    return n[i][j];
}

c_float& Mat3F::operator ()(int i, int j) const
{
    // Column Major
    return n[i][j];
}

c_float* Mat3F::getPointer()
{
    return &(n[0][0]);
}

// Mat3F Non-Members

Mat3F operator +(const Mat3F& a, const Mat3F& b)
{
    // Column Major
    return Mat3F(a(0, 0) + b(0, 0), a(1, 0) + b(1, 0), a(2, 0) + b(2, 0),
	         a(0, 1) + b(0, 1), a(1, 1) + b(1, 1), a(2, 1) + b(2, 1),
	         a(0, 2) + b(0, 2), a(1, 2) + b(1, 2), a(2, 2) + b(2, 2));
}

Mat3F operator -(const Mat3F& a, const Mat3F& b)
{
    // Column Major
    return Mat3F(a(0, 0) - b(0, 0), a(1, 0) - b(1, 0), a(2, 0) - b(2, 0),
	         a(0, 1) - b(0, 1), a(1, 1) - b(1, 1), a(2, 1) - b(2, 1),
	         a(0, 2) - b(0, 2), a(1, 2) - b(1, 2), a(2, 2) - b(2, 2));
}

Mat3F operator *(const Mat3F& a, const Mat3F& b)
{
    // Column Major
    return Mat3F(a(0, 0) * b(0, 0) + a(1, 0) * b(0, 1) + a(2, 0) * b(0, 2),
	         a(0, 0) * b(1, 0) + a(1, 0) * b(1, 1) + a(2, 0) * b(1, 2),
	         a(0, 0) * b(2, 0) + a(1, 0) * b(2, 1) + a(2, 0) * b(2, 2),
	         a(0, 1) * b(0, 0) + a(1, 1) * b(0, 1) + a(2, 1) * b(0, 2),
		 a(0, 1) * b(1, 0) + a(1, 1) * b(1, 1) + a(2, 1) * b(1, 2),
		 a(0, 1) * b(2, 0) + a(1, 1) * b(2, 1) + a(2, 1) * b(2, 2),
		 a(0, 2) * b(0, 0) + a(1, 2) * b(0, 1) + a(2, 2) * b(0, 2),
		 a(0, 2) * b(1, 0) + a(1, 2) * b(1, 1) + a(2, 2) * b(1, 2),
		 a(0, 2) * b(2, 0) + a(1, 2) * b(2, 1) + a(2, 2) * b(2, 2));
}

Vec3F operator *(const Mat3F& m, const Vec3F& v)
{
    // Column Major
    return Vec3F(m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z,
		 m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z,
		 m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z);
}

Mat3F transpose(const Mat3F& m)
{
    // Column Major
    return Mat3F(m(0, 0), m(0, 1), m(0, 2),
		 m(1, 0), m(1, 1), m(1, 2),
		 m(2, 0), m(2, 1), m(2, 2));
}

void print(const Mat3F& m)
{
    // Column Major
    for(int x = 0; x < 3; x++)
    {
	printf("\n");
	for(int y = 0; y < 3; y++)
	    printf("%f, ", m(y, x));
    }
    printf("\n");
}

//////////////////
// Struct Mat4F //
//////////////////

Mat4F::Mat4F()
{
    n[0][0] = 0.0f; n[0][1] = 0.0f; n[0][2] = 0.0f; n[0][3] = 0.0f;
    n[1][0] = 0.0f; n[1][1] = 0.0f; n[1][2] = 0.0f; n[1][3] = 0.0f;
    n[2][0] = 0.0f; n[2][1] = 0.0f; n[2][2] = 0.0f; n[2][3] = 0.0f;
    n[3][0] = 0.0f; n[3][1] = 0.0f; n[3][2] = 0.0f; n[3][3] = 0.0f;
}

Mat4F::Mat4F(float n00, float n01, float n02, float n03,
	     float n10, float n11, float n12, float n13,
	     float n20, float n21, float n22, float n23,
	     float n30, float n31, float n32, float n33)
{
    // Column Major
    n[0][0] = n00; n[0][1] = n10; n[0][2] = n20; n[0][3] = n30;
    n[1][0] = n01; n[1][1] = n11; n[1][2] = n21; n[1][3] = n31;
    n[2][0] = n02; n[2][1] = n12; n[2][2] = n22; n[2][3] = n32;
    n[3][0] = n03; n[3][1] = n13; n[3][2] = n23; n[3][3] = n33;
}

Mat4F::Mat4F(float f)
{
    n[0][0] = f;    n[0][1] = 0.0f; n[0][2] = 0.0f; n[0][3] = 0.0f;
    n[1][0] = 0.0f; n[1][1] = f;    n[1][2] = 0.0f; n[1][3] = 0.0f;
    n[2][0] = 0.0f; n[2][1] = 0.0f; n[2][2] = f;    n[2][3] = 0.0f;
    n[3][0] = 0.0f; n[3][1] = 0.0f; n[3][2] = 0.0f; n[3][3] = f;
}

Mat4F::Mat4F(const Vec4F& a, const Vec4F& b, const Vec4F& c, const Vec4F& d)
{
    // Column Major
    n[0][0] = a.x; n[0][1] = b.x; n[0][2] = c.x; n[0][3] = d.x;
    n[1][0] = a.y; n[1][1] = b.y; n[1][2] = c.y; n[1][3] = d.y;
    n[2][0] = a.z; n[2][1] = b.z; n[2][2] = c.z; n[2][3] = d.z;
    n[3][0] = a.w; n[3][1] = b.w; n[3][2] = c.w; n[3][3] = d.w;    
}

Mat4F::Mat4F(const Mat4F& m)
{
    // Column Major
    n[0][0] = m(0, 0); n[0][1] = m(0, 1); n[0][2] = m(0, 2); n[0][3] = m(0, 3);
    n[1][0] = m(1, 0); n[1][1] = m(1, 1); n[1][2] = m(1, 2); n[1][3] = m(1, 3);
    n[2][0] = m(2, 0); n[2][1] = m(2, 1); n[2][2] = m(2, 2); n[2][3] = m(2, 3);
    n[3][0] = m(3, 0); n[3][1] = m(3, 1); n[3][2] = m(3, 2); n[3][3] = m(3, 3);
}

Mat4F::Mat4F(const Mat3F& m)
{
    // Column Major
    n[0][0] = m(0, 0); n[0][1] = m(0, 1); n[0][2] = m(0, 2); n[0][3] = 0.0f;
    n[1][0] = m(1, 0); n[1][1] = m(1, 1); n[1][2] = m(1, 2); n[1][3] = 0.0f;
    n[2][0] = m(2, 0); n[2][1] = m(2, 1); n[2][2] = m(2, 2); n[2][3] = 0.0f;
    n[3][0] = 0.0f;    n[3][1] = 0.0f;    n[3][2] = 0.0f;    n[3][3] = 1.0f;
}

void Mat4F::operator *=(float s)
{
    // Column Major
    n[0][0] *= s; n[0][1] *= s; n[0][2] *= s; n[0][3] *= s;
    n[1][0] *= s; n[1][1] *= s; n[1][2] *= s; n[1][3] *= s;
    n[2][0] *= s; n[2][1] *= s; n[2][2] *= s; n[2][3] *= s;
    n[3][0] *= s; n[3][1] *= s; n[3][2] *= s; n[3][3] *= s;
}

void Mat4F::operator +=(const Mat4F& m)
{
    n[0][0] += m(0, 0); n[0][1] += m(0, 1); n[0][2] += m(0, 2); n[0][3] += m(0, 3);
    n[1][0] += m(1, 0); n[1][1] += m(1, 1); n[1][2] += m(1, 2); n[1][3] += m(1, 3);
    n[2][0] += m(2, 0); n[2][1] += m(2, 1); n[2][2] += m(2, 2); n[2][3] += m(2, 3);
    n[3][0] += m(3, 0); n[3][1] += m(3, 1); n[3][2] += m(3, 2); n[3][3] += m(3, 3);
}

void Mat4F::operator -=(const Mat4F& m)
{
    n[0][0] -= m(0, 0); n[0][1] -= m(0, 1); n[0][2] -= m(0, 2); n[0][3] -= m(0, 3);
    n[1][0] -= m(1, 0); n[1][1] -= m(1, 1); n[1][2] -= m(1, 2); n[1][3] -= m(1, 3);
    n[2][0] -= m(2, 0); n[2][1] -= m(2, 1); n[2][2] -= m(2, 2); n[2][3] -= m(2, 3);
    n[3][0] -= m(3, 0); n[3][1] -= m(3, 1); n[3][2] -= m(3, 2); n[3][3] -= m(3, 3);
}

float& Mat4F::operator ()(int i, int j)
{
    return n[i][j];
}

c_float& Mat4F::operator ()(int i, int j) const
{
    return n[i][j];
}

c_float* Mat4F::getPointer()
{
    return &(n[0][0]);
}

// Mat4F Non-members

// TO-DO: implement all

Mat4F operator +(const Mat4F& a, const Mat4F& b)
{
    // Column Major
    return Mat4F(a(0, 0) + b(0, 0), a(1, 0) + b(1, 0), a(2, 0) + b(2, 0), a(3, 0) + b(3, 0),
		 a(0, 1) + b(0, 1), a(1, 1) + b(1, 1), a(2, 1) + b(2, 1), a(3, 1) + b(3, 1),
		 a(0, 2) + b(0, 2), a(1, 2) + b(1, 2), a(2, 2) + b(2, 2), a(3, 2) + b(3, 2),
		 a(0, 3) + b(0, 3), a(1, 3) + b(1, 3), a(2, 3) + b(2, 3), a(3, 3) + b(3, 3));
}

Mat4F operator -(const Mat4F& a, const Mat4F& b)
{
    // Column Major
    return Mat4F(a(0, 0) - b(0, 0), a(1, 0) - b(1, 0), a(2, 0) - b(2, 0), a(3, 0) - b(3, 0),
		 a(0, 1) - b(0, 1), a(1, 1) - b(1, 1), a(2, 1) - b(2, 1), a(3, 1) - b(3, 1),
		 a(0, 2) - b(0, 2), a(1, 2) - b(1, 2), a(2, 2) - b(2, 2), a(3, 2) - b(3, 2),
		 a(0, 3) - b(0, 3), a(1, 3) - b(1, 3), a(2, 3) - b(2, 3), a(3, 3) - b(3, 3));
}

Mat4F operator *(const Mat4F& a, const Mat4F& b)
{
    // Column Major
    return Mat4F(a(0, 0) * b(0, 0) + a(1, 0) * b(0, 1) + a(2, 0) * b(0, 2) + a(3, 0) * b(0, 3), // row 1
	         a(0, 0) * b(1, 0) + a(1, 0) * b(1, 1) + a(2, 0) * b(1, 2) + a(3, 0) * b(1, 3),
		 a(0, 0) * b(2, 0) + a(1, 0) * b(2, 1) + a(2, 0) * b(2, 2) + a(3, 0) * b(2, 3),
		 a(0, 0) * b(3, 0) + a(1, 0) * b(3, 1) + a(2, 0) * b(3, 2) + a(3, 0) * b(3, 3),

		 a(0, 1) * b(0, 0) + a(1, 1) * b(0, 1) + a(2, 1) * b(0, 2) + a(3, 1) * b(0, 3), // row 2
		 a(0, 1) * b(1, 0) + a(1, 1) * b(1, 1) + a(2, 1) * b(1, 2) + a(3, 1) * b(1, 3),
		 a(0, 1) * b(2, 0) + a(1, 1) * b(2, 1) + a(2, 1) * b(2, 2) + a(3, 1) * b(2, 3),
		 a(0, 1) * b(3, 0) + a(1, 1) * b(3, 1) + a(2, 1) * b(3, 2) + a(3, 1) * b(3, 3),

		 a(0, 2) * b(0, 0) + a(1, 2) * b(0, 1) + a(2, 2) * b(0, 2) + a(3, 2) * b(0, 3), // row 3
		 a(0, 2) * b(1, 0) + a(1, 2) * b(1, 1) + a(2, 2) * b(1, 2) + a(3, 2) * b(1, 3),
		 a(0, 2) * b(2, 0) + a(1, 2) * b(2, 1) + a(2, 2) * b(2, 2) + a(3, 2) * b(2, 3),
		 a(0, 2) * b(3, 0) + a(1, 2) * b(3, 1) + a(2, 2) * b(3, 2) + a(3, 2) * b(3, 3),

		 a(0, 3) * b(0, 0) + a(1, 3) * b(0, 1) + a(2, 3) * b(0, 2) + a(3, 3) * b(0, 3), // row 4
		 a(0, 3) * b(1, 0) + a(1, 3) * b(1, 1) + a(2, 3) * b(1, 2) + a(3, 3) * b(1, 3),
		 a(0, 3) * b(2, 0) + a(1, 3) * b(2, 1) + a(2, 3) * b(2, 2) + a(3, 3) * b(2, 3),
		 a(0, 3) * b(3, 0) + a(1, 3) * b(3, 1) + a(2, 3) * b(3, 2) + a(3, 3) * b(3, 3));
}

Vec4F operator *(const Mat4F& m, const Vec4F& v)
{
    // Column Major
    return Vec4F(m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z + m(0, 3) * v.w,
		 m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z + m(1, 3) * v.w,
		 m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z + m(2, 3) * v.w,
		 m(3, 0) * v.x + m(3, 1) * v.y + m(3, 2) * v.z + m(3, 3) * v.w);
}

Mat4F transpose(const Mat4F& m)
{
    // Column Major
    return Mat4F(m(0, 0), m(0, 1), m(0, 2), m(0, 3),
		 m(1, 0), m(1, 1), m(1, 2), m(1, 3),
		 m(2, 0), m(2, 1), m(2, 2), m(2, 3),
		 m(3, 0), m(3, 1), m(3, 2), m(3, 3));
}

void print(const Mat4F& m)
{
    // Column Major
    for(int x = 0; x < 4; x++)
    {
	printf("\n");
	for(int y = 0; y < 4; y++)
	    printf("%f, ", m(y, x));
    }
    printf("\n");
}

///////////////////////
// Struct Quaternion //
///////////////////////

Quaternion::Quaternion()
{
    w = 1.0f;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Quaternion::Quaternion(float _w, float _x, float _y, float _z)
{
    w = _w;
    x = _x;
    y = _y;
    z = _z;
}

Quaternion::Quaternion(float _w, const Vec3F& v)
{
    w = _w;
    x = v.x;
    y = v.y;
    z = v.z;
}

void Quaternion::operator +=(const Quaternion& q)
{
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;
}

void Quaternion::operator -=(const Quaternion& q)
{
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;
}

// Quaternion non-members

Quaternion operator +(const Quaternion& q1, const Quaternion& q2)
{
    return Quaternion(q1.w + q2.w,
	              q1.x + q2.x,
		      q1.y + q2.y,
		      q1.z + q2.z);
}

Quaternion operator -(const Quaternion& q1, const Quaternion& q2)
{
    return Quaternion(q1.w - q2.w,
	              q1.x - q2.x,
		      q1.y - q2.y,
		      q1.z - q2.z);
}

Quaternion operator *(const Quaternion& q1, const Quaternion& q2)
{
    Vec3F v1(q1.x, q1.y, q1.z);
    Vec3F v2(q2.x, q2.y, q2.z);

    float w = (q1.w * q2.w) - dot(v1, v2);
    Vec3F v = (q1.w * v2) + (q2.w * v1) + cross(v1, v2);

    return Quaternion(w, v);
}

Quaternion operator *(const Quaternion& q, float s)
{
    return Quaternion(q.w * s,
	              q.x * s,
	              q.y * s,
	              q.z * s);
}

Quaternion operator *(float s, const Quaternion& q)
{
    return Quaternion(q.w * s,
	              q.x * s,
	              q.y * s,
	              q.z * s);
}

float dot(const Quaternion& q1, const Quaternion& q2)
{
    return (q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);
}

float dot(const Quaternion& q)
{
    return (q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

float magnitude(const Quaternion& q)
{
    return sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

Quaternion normalize(const Quaternion& q)
{
    float lengthRecip = (1.0f / magnitude(q));
    return Quaternion(q.w * lengthRecip,
		      q.x * lengthRecip,
		      q.y * lengthRecip,
		      q.z * lengthRecip);
}

Quaternion inverse(const Quaternion& q)
{
    // Assumes a unit quaternion is given
    return Quaternion(q.w, -q.x, -q.y, -q.z);
}

Mat3F quatToMat3(const Quaternion& q)
{
    // Assumes a unit quaternion is given
    
    float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    s = 2.0f / (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);

    xs = s * q.x; ys = s * q.y; zs = s * q.z;
    wx = q.w * xs; wy = q.w * ys; wz = q.w * zs;
    xx = q.x * xs; xy = q.x * ys; xz = q.x * zs;
    yy = q.y * ys; yz = q.y * zs; zz = q.z * zs;

    return Mat3F(1.0f - (yy + zz), xy + wz,          xz - wy,
	         xy - wz,          1.0f - (xx + zz), yz + wx,
	         xz + wy,          yz - wx,          1.0f - (xx + yy));
}

void print(const Quaternion& q)
{
    printf("(%f, (%f, %f, %f))\n", q.w, q.x, q.y, q.z);
}

///////////////////
// Interpolation //
///////////////////

Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    // An unoptimized slerp implementation from Jonathan Blow,
    // altered from Shoemake
    float cosTheta = clamp(dot(q1, q2), -1, 1);

    // If quaternions are nearly parallel, linearly interpolate instead
    if(cosTheta > 0.99f)
    {
	Quaternion result = q1 + t * (q2 - q1);
	normalize(result);
	return result;
    }

    float theta_0 = acos(cosTheta); // angle between input quaternions
    float theta_1 = t * theta_0; // interpolated angle
    Quaternion q3 = q2 - (q1 * cosTheta);
    q3 = normalize(q3);

    return q1 * cos(theta_1) + q3 * (sin(theta_1));
}

///////////
// Misc. //
///////////

float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float clamp(float n, float min, float max)
{
    float t = n < min ? min : n;
    return t > max ? max : t;
}

float degToRads(float d)
{
    return (float)(d * 0.0174532925);
}

Vec3F rotate(const Vec3F& v, const Quaternion& q)
{
    float vMult     = 2.0f * (q.x * v.x + q.y * v.y * q.z * v.z);
    float crossMult = 2.0f * q.w;
    float pMult     = crossMult * q.w - 1.0f;

    return Vec3F(pMult * v.x + vMult * q.x + crossMult * (q.y * v.z - q.z * v.y),
	         pMult * v.y + vMult * q.y + crossMult * (q.z * v.x - q.x * v.z),
	         pMult * v.z + vMult * q.z + crossMult * (q.x * v.y - q.y * v.x));
}

Mat4F lookAt(const Vec3F& eye, const Vec3F& at, const Vec3F& up)
{
    // Assumes RH coordinate system
    // Assume Column Major
    
    Vec3F zaxis = normalize(at - eye);
    Vec3F xaxis = normalize(cross(zaxis, up));
    Vec3F yaxis = cross(xaxis, zaxis);

    zaxis *= -1;
    
    return Mat4F(xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, eye),
		 yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, eye),
		 zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, eye),
		 0.0f, 0.0f, 0.0f, 1.0f);
    
}

Mat4F getOrthoProjection(float l, float r, float b, float t, float n, float f)
{
    return Mat4F(2/(r - l), 0.0f, 0.0f,  -(r + l) / (r - l),
		 0.0f, 2/(t - b), 0.0f,  -(t + b) / (t - b),
		 0.0f, 0.0f, -2/(f - n), -(f + n) / (f - n),
		 0.0f, 0.0f, 0.0f, 1.0f);
}
