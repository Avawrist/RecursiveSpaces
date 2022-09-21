// ======================================================================
// Title: mdcla.cpp
// Description: The implementation file for my personal linear algebra library.
// Last Modified: 9/20/2022 MDC
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
    return Vec2F((*this).x / d,
	         (*this).y / d);
}

void Vec2F::operator /=(float d)
{
    this->x /= d;
    this->y /= d;
}

void Vec2F::operator *=(float s)
{
    this->x *= s;
    this->y *= s;
}

void Vec2F::operator +=(const Vec2F& vec2a)
{
    this->x += vec2a.x;
    this->y += vec2a.y;
}

void Vec2F::operator -=(const Vec2F& vec2s)
{
    this->x -= vec2s.x;
    this->y -= vec2s.y;
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

Vec2F operator +(const Vec2F& vec2a, const Vec2F& vec2b)
{
    return Vec2F(vec2a.x + vec2b.x,
	         vec2a.y + vec2b.y);
}

Vec2F operator -(const Vec2F& vec2a, const Vec2F& vec2b)
{
    return Vec2F(vec2a.x - vec2b.x,
	         vec2a.y - vec2b.y);
}

Vec2F operator *(const Vec2F& vec2, float s)
{
    return Vec2F(vec2.x * s,
	         vec2.y * s);
}

Vec2F operator *(float s, const Vec2F& vec2)
{
    return Vec2F(vec2.x * s,
	         vec2.y * s);
}

float dot2F(const Vec2F& vec2a, const Vec2F& vec2b)
{
    return ((vec2a.x * vec2b.x) +
	    (vec2a.y * vec2b.y));
}

float dot2F(const Vec2F& vec2)
{
    return ((vec2.x * vec2.x) +
	    (vec2.y * vec2.y));
}

float magnitude2F(const Vec2F& vec2)
{
    return sqrt(dot2F(vec2));
}

Vec2F normalize2F(const Vec2F& vec2)
{
    
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
    return Vec3F((*this).x / d,
	         (*this).y / d,
		 (*this).z / d);
}

void Vec3F::operator /=(float d)
{
    this->x /= d;
    this->y /= d;
    this->z /= d;
}

void Vec3F::operator *=(float s)
{
    this->x *= s;
    this->y *= s;
    this->z *= s;
}

void Vec3F::operator +=(const Vec3F& vec3a)
{
    this->x += vec3a.x;
    this->y += vec3a.y;
    this->z += vec3a.z;
}

void Vec3F::operator -=(const Vec3F& vec3s)
{
    this->x -= vec3s.x;
    this->y -= vec3s.y;
    this->z -= vec3s.z;
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

Vec3F operator +(const Vec3F& vec3a, const Vec3F& vec3b)
{
    return Vec3F(vec3a.x + vec3b.x,
	         vec3a.y + vec3b.y,
	         vec3a.z + vec3b.z);
}

Vec3F operator -(const Vec3F& vec3a, const Vec3F& vec3b)
{
    return Vec3F(vec3a.x - vec3b.x,
	         vec3a.y - vec3b.y,
	         vec3a.z - vec3b.z);
}

Vec3F operator *(const Vec3F& vec3, float s)
{
    return Vec3F(vec3.x * s,
	         vec3.y * s,
	         vec3.z * s);
}

Vec3F operator *(float s, const Vec3F& vec3)
{
    return Vec3F(vec3.x * s,
	         vec3.y * s,
	         vec3.z * s);
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
    return Vec4F((*this).x / d,
	         (*this).y / d,
		 (*this).z / d,
	         (*this).w / d);
}

void Vec4F::operator /=(float d)
{
    this->x /= d;
    this->y /= d;
    this->z /= d;
    this->w /= d;
}

void Vec4F::operator *=(float s)
{
    this->x *= s;
    this->y *= s;
    this->z *= s;
    this->w *= s;
}

void Vec4F::operator +=(const Vec4F& vec4a)
{
    this->x += vec4a.x;
    this->y += vec4a.y;
    this->z += vec4a.z;
    this->w += vec4a.w;
}

void Vec4F::operator -=(const Vec4F& vec4s)
{
    this->x -= vec4s.x;
    this->y -= vec4s.y;
    this->z -= vec4s.z;
    this->w -= vec4s.w;
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

Vec4F operator +(const Vec4F& vec4a, const Vec4F& vec4b)
{
    return Vec4F(vec4a.x + vec4b.x,
	         vec4a.y + vec4b.y,
	         vec4a.z + vec4b.z,
	         vec4a.w + vec4b.w);
}

Vec4F operator -(const Vec4F& vec4a, const Vec4F& vec4b)
{
    return Vec4F(vec4a.x - vec4b.x,
	         vec4a.y - vec4b.y,
	         vec4a.z - vec4b.z,
	         vec4a.w - vec4b.w);
}

Vec4F operator *(const Vec4F& vec4, float s)
{
    return Vec4F(vec4.x * s,
	         vec4.y * s,
	         vec4.z * s,
	         vec4.w * s);
}

Vec4F operator *(float s, const Vec4F& vec4)
{
    return Vec4F(vec4.x * s,
	         vec4.y * s,
	         vec4.z * s,
	         vec4.w * s);
}
 
