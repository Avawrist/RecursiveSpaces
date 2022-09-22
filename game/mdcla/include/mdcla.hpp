// ======================================================================
// Title: mdcla.hpp
// Description: The header file for my personal linear algebra library.
// ======================================================================

#ifndef MDCLA_H
#define MDCLA_H

#include <cmath>
#include <stdio.h> // TO-DO: Remove from release ver w/ print functions

typedef struct Vec2F
{
    float x;
    float y;
    Vec2F();
    Vec2F(float _x, float _y);
    Vec2F        operator /(float d);
    void         operator /=(float d);
    void         operator *=(float s);
    void         operator +=(const Vec2F& v);
    void         operator -=(const Vec2F& v);
    float&       operator [](int i);
    const float& operator [](int i) const;
    void print();
} Vec2F;
Vec2F operator +(const Vec2F& a, const Vec2F& b);
Vec2F operator -(const Vec2F& a, const Vec2F& b);
Vec2F operator *(const Vec2F& v, float s);
Vec2F operator *(float s, const Vec2F& v);
float dot2F(const Vec2F& a, const Vec2F& b);
float dot2F(const Vec2F& v);
float magnitude2F(const Vec2F& v);
Vec2F normalize2F(const Vec2F& v);

typedef struct Vec3F
{
    float x;
    float y;
    float z;
    Vec3F();
    Vec3F(float _x, float _y, float _z);
    Vec3F        operator /(float d);
    void         operator /=(float d);
    void         operator *=(float s);
    void         operator +=(const Vec3F& v);
    void         operator -=(const Vec3F& v);
    float&       operator [](int i);
    const float& operator [](int i) const;
    void print();
} Vec3F;
Vec3F operator +(const Vec3F& a, const Vec3F& b);
Vec3F operator -(const Vec3F& a, const Vec3F& b);
Vec3F operator *(const Vec3F& v, float s);
Vec3F operator *(float s, const Vec3F& v);
Vec3F cross3F(const Vec3F& a, const Vec3F& b);
float dot3F(const Vec3F& a, const Vec3F& b);
float dot3F(const Vec3F& v);
float magnitude3F(const Vec3F& v);
Vec3F normalize3F(const Vec3F& v);

typedef struct Vec4F
{
    float x;
    float y;
    float z;
    float w;
    Vec4F();
    Vec4F(float _x, float _y, float _z, float _w);
    Vec4F        operator /(float d);
    void         operator /=(float d);
    void         operator *=(float s);
    void         operator +=(const Vec4F& v);
    void         operator -=(const Vec4F& v);
    float&       operator [](int i);
    const float& operator [](int i) const;
    void print();
} Vec4F;
Vec4F operator +(const Vec4F& a, const Vec4F& b);
Vec4F operator -(const Vec4F& a, const Vec4F& b);
Vec4F operator *(const Vec4F& v, float s);
Vec4F operator *(float s, const Vec4F& v);
float dot4F(const Vec4F& a, const Vec4F& b);
float dot4F(const Vec4F& v);
float magnitude4F(const Vec4F& v);
Vec4F normalize4F(const Vec4F& v);

typedef struct Mat3F
{
private:
    float n[3][3];
public:
    Mat3F();
    Mat3F(float n00, float n01, float n02,
	  float n10, float n11, float n12,
	  float n20, float n21, float n22);
    Mat3F(float f);
    Mat3F(const Vec3F& a, const Vec3F& b, const Vec3F& c);
    void         operator *=(float s);
    void         operator +=(const Mat3F& m);
    void         operator -=(const Mat3F& m);
    Vec3F        operator [](int j);
    const Vec3F  operator [](int j) const;
    float&       operator ()(int i, int j);
    const float& operator ()(int i, int j) const;
    void print();
    void transpose();
} Mat3F;
Mat3F operator +(const Mat3F& a, const Mat3F& b);
Mat3F operator -(const Mat3F& a, const Mat3F& b);
Mat3F operator *(const Mat3F& a, const Mat3F& b);
Vec3F operator *(const Mat3F& m, const Vec3F& v);

typedef struct Mat4F
{
private:
    float n[4][4];
public:
    
} Mat4F;

#endif
