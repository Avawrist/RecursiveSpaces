// ======================================================================
// Title: mdcla.cpp
// Description: The implementation file for my personal linear algebra library.
// ======================================================================

#include "mdcla.hpp"

// Struct Vec2F //

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

// Struct Vec3F //

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

// Struct Vec4F //

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

// Struct Mat3F //

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

// Struct Mat4F //

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

// Struct Quaternion //

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

// Interpolation //

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
