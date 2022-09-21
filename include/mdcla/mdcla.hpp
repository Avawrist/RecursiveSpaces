// ======================================================================
// Title: mdcla.hpp
// Description: The header file for my personal linear algebra library.
// Last Modified: 9/20/2022 MDC
// ======================================================================

#ifndef MDCLA_H
#define MDCLA_H

typedef struct Vec2F {
    float x;
    float y;
    Vec2F();
    Vec2F(float _x, float _y);
    float& operator [](int i);
    const float& operator [](int i) const;
} Vec2F;
Vec2F operator *(const Vec2F& vec2, float s);

typedef struct Vec3F {
    float x;
    float y;
    float z;
    Vec3F();
    Vec3F(float _x, float _y, float _z);
    float& operator [](int i);
    const float& operator [](int i) const;
} Vec3F;
Vec3F operator *(const Vec3F& vec3, float s);

typedef struct Vec4F {
    float x;
    float y;
    float z;
    float w;
    Vec4F();
    Vec4F(float _x, float _y, float _z, float _w);
    float& operator [](int i);
    const float& operator [](int i) const;
} Vec4F;
Vec4F operator *(const Vec4F& vec4, float s);

/*
typedef struct Mat33F {
	
} Mat33F;

typedef struct Mat44F {
	
} Mat44F;

float dotVec2();

float dotVec3();
    
float dotVec4();

Vec2 crossVec2();
    
Vec3 crossVec3();

Vec4 crossVec4();
*/

#endif
