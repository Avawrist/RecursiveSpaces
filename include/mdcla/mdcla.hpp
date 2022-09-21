// ======================================================================
// Title: mdcla.hpp
// Description: The header file for my personal linear algebra library.
// Last Modified: 9/20/2022 MDC
// ======================================================================

#ifndef MDCLA_H
#define MDCLA_H

#include <cmath>

typedef struct Vec2F {
    float x;
    float y;
    Vec2F();
    Vec2F(float _x, float _y);
    Vec2F        operator /(float d);
    void         operator /=(float d);
    void         operator *=(float s);
    void         operator +=(const Vec2F& vec2a);
    void         operator -=(const Vec2F& vec2s);
    float&       operator [](int i);
    const float& operator [](int i) const;
} Vec2F;
Vec2F operator +(const Vec2F& vec2a, const Vec2F& vec2b);
Vec2F operator -(const Vec2F& vec2a, const Vec2F& vec2b);
Vec2F operator *(const Vec2F& vec2, float s);
Vec2F operator *(float s, const Vec2F& vec2);
float dot2F(const Vec2F& vec2a, const Vec2F& vec2b);
float dot2F(const Vec2F& vec2);
float magnitude2F(const Vec2F& vec2);
Vec2F normalize2F(const Vec2F& vec2);

typedef struct Vec3F {
    float x;
    float y;
    float z;
    Vec3F();
    Vec3F(float _x, float _y, float _z);
    Vec3F        operator /(float d);
    void         operator /=(float d);
    void         operator *=(float s);
    void         operator +=(const Vec3F& vec3a);
    void         operator -=(const Vec3F& vec3s);
    float&       operator [](int i);
    const float& operator [](int i) const;
} Vec3F;
Vec3F operator +(const Vec3F& vec3a, const Vec3F& vec3b);
Vec3F operator -(const Vec3F& vec3a, const Vec3F& vec3b);
Vec3F operator *(const Vec3F& vec3, float s);
Vec3F operator *(float s, const Vec3F& vec3);

typedef struct Vec4F {
    float x;
    float y;
    float z;
    float w;
    Vec4F();
    Vec4F(float _x, float _y, float _z, float _w);
    Vec4F        operator /(float d);
    void         operator /=(float d);
    void         operator *=(float s);
    void         operator +=(const Vec4F& vec4a);
    void         operator -=(const Vec4F& vec4s);
    float&       operator [](int i);
    const float& operator [](int i) const;
} Vec4F;
Vec4F operator +(const Vec4F& vec4a, const Vec4F& vec4b);
Vec4F operator -(const Vec4F& vec4a, const Vec4F& vec4b);
Vec4F operator *(const Vec4F& vec4, float s);
Vec4F operator *(float s, const Vec4F& vec4);

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
