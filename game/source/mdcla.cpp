// ======================================================================
// Title: mdcla.cpp
// Description: The implementation file for my personal linear algebra library.
// ======================================================================

#include <mdcla.hpp>

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

const float& Vec2F::operator [](int i) const
{
    return *(&x + i);
}

// Vec2F Non-members

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

float dot2F(const Vec2F& a, const Vec2F& b)
{
    return ((a.x * b.x) +
	    (a.y * b.y));
}

float dot2F(const Vec2F& v)
{
    return ((v.x * v.x) +
	    (v.y * v.y));
}

float magnitude2F(const Vec2F& v)
{
    return sqrt(dot2F(v));
}

Vec2F normalize2F(const Vec2F& v)
{
    return (v * (1.0f / magnitude2F(v)));
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

const float& Vec3F::operator [](int i) const
{
    return *(&x + i);
}

// Vec3F Non-Members

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

Vec3F cross3F(const Vec3F& a, const Vec3F& b)
{
    return Vec3F((a.y * b.z) - (a.z * b.y),
		 (a.z * b.x) - (a.x * b.z),
		 (a.x * b.y) - (a.y * b.x));
}

float dot3F(const Vec3F& a, const Vec3F& b)
{
    return ((a.x * b.x) +
	    (a.y * b.y) +
	    (a.z * b.z));
}

float dot3F(const Vec3F& v)
{
    return ((v.x * v.x) +
	    (v.y * v.y) +
	    (v.z * v.z));
}

float magnitude3F(const Vec3F& v)
{
    return (sqrt(dot3F(v)));
}

Vec3F normalize3F(const Vec3F& v)
{
    return (v * (1.0f / magnitude3F(v)));
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

const float& Vec4F::operator [](int i) const
{
    return *(&x + i);
}

// Vec4F Non-Members

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
 
float dot4F(const Vec4F& a, const Vec4F& b)
{
    return ((a.x * b.x) +
	    (a.y * b.y) +
	    (a.z * b.z) +
	    (a.w * b.w));
}

float dot4F(const Vec4F& v)
{
    return ((v.x * v.x) +
	    (v.y * v.y) +
	    (v.z * v.z) +
	    (v.w * v.w));
}

float magnitude4F(const Vec4F& v)
{
    return(sqrt(dot4F(v)));
}

Vec4F normalize4F(const Vec4F& v)
{
    return (v * (1.0f / magnitude4F(v)));
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
    n[0][0] = n00; n[0][1] = n01; n[0][2] = n02; 
    n[1][0] = n10; n[1][1] = n11; n[1][2] = n12;
    n[2][0] = n20; n[2][1] = n21; n[2][2] = n22;
}

Mat3F::Mat3F(const Vec3F& a, const Vec3F& b, const Vec3F& c)
{
    n[0][0] = a.x; n[0][1] = b.x; n[0][2] = c.x; 
    n[1][0] = a.y; n[1][1] = b.y; n[1][2] = c.y;
    n[2][0] = a.z; n[2][1] = b.z; n[2][2] = c.z;
}

Vec3F Mat3F::operator [](int j)
{
    return Vec3F(n[0][j], n[1][j], n[2][j]);
}

float& Mat3F::operator ()(int i, int j)
{
    return n[j][i];
}

const float& Mat3F::operator ()(int i, int j) const
{
    return n[j][i];
}

void Mat3F::print()
{
    for(int x = 0; x < 3; x++)
    {
	printf("\n");
	for(int y = 0; y < 3; y++)
	    printf("%f, ", n[x][y]);
    }
    printf("\n");
}
