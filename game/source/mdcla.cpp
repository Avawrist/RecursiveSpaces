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

Vec2F operator *(const Vec2F& vec2, float s)
{
    return Vec2F(vec2.x * s,
	         vec2.y * s);
}

float& Vec2F::operator [](int i)
{
    return *(&x + i);
}

const float& Vec2F::operator [](int i) const
{
    return *(&x + i);
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

Vec3F operator  *(const Vec3F& vec3, float s)
{
    return Vec3F(vec3.x * s,
	         vec3.y * s,
	         vec3.z * s);
}

float& Vec3F::operator [](int i)
{
    return *(&x + i);
}

const float& Vec3F::operator [](int i) const
{
    return *(&x + i);
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

Vec4F operator *(const Vec4F& vec4, float s)
{
    return Vec4F(vec4.x * s,
	         vec4.y * s,
	         vec4.z * s,
	         vec4.w * s);
}

float& Vec4F::operator [](int i)
{
    return *(&x + i);
}

const float& Vec4F::operator [](int i) const
{
    return *(&x + i);
}
