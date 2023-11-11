// ======================================================================
// Title: mdcla.hpp
// Description: The header file for my personal linear algebra library.
// ======================================================================

#ifndef MDCLA_H
#define MDCLA_H

// Win libs
#include <cmath>
#include <stdio.h> // TO-DO: Remove from release ver w/ print functions

// My libs
#include "utility.hpp"

// Struct Vec2F //

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
    c_float&     operator [](int i) const;
} Vec2F;

inline bool
operator ==(const Vec2F& a, const Vec2F& b) {return(a.x == b.x && a.y == b.y);}

inline Vec2F
operator +(const Vec2F& a, const Vec2F& b) {return Vec2F(a.x + b.x,a.y + b.y);}

inline Vec2F
operator -(const Vec2F& a, const Vec2F& b) {return Vec2F(a.x - b.x,a.y - b.y);}

inline Vec2F
operator *(const Vec2F& v, float s) {return Vec2F(v.x * s, v.y * s);}

inline Vec2F
operator *(float s, const Vec2F& v) {return Vec2F(v.x * s, v.y * s);}

inline float
dot(const Vec2F& a, const Vec2F& b) {return ((a.x * b.x) + (a.y * b.y));}

inline float
dot(const Vec2F& v) {return ((v.x * v.x) + (v.y * v.y));}

inline float
magnitude(const Vec2F& v) {return sqrt(dot(v));}

inline Vec2F
normalize(const Vec2F& v) {return (v * (1.0f / magnitude(v)));}

inline void
print(const Vec2F& v) {printf("(%f, %f)\n", v.x, v.y);}

// Struct Vec3F //

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
    c_float&     operator [](int i) const;
} Vec3F;

inline bool
operator ==(const Vec3F& a, const Vec3F& b) {return(a.x == b.x && a.y == b.y && a.z == b.z);}

inline Vec3F
operator +(const Vec3F& a, const Vec3F& b) {return Vec3F(a.x + b.x, a.y + b.y, a.z + b.z);}

inline Vec3F
operator -(const Vec3F& a, const Vec3F& b) {return Vec3F(a.x - b.x, a.y - b.y, a.z - b.z);}

inline Vec3F
operator *(const Vec3F& v, float s) {return Vec3F(v.x * s, v.y * s, v.z * s);}

inline Vec3F
operator *(float s, const Vec3F& v) {return Vec3F(v.x * s, v.y * s, v.z * s);}

inline Vec3F
cross(const Vec3F& a, const Vec3F& b) {return Vec3F((a.y * b.z) - (a.z * b.y),
								 (a.z * b.x) - (a.x * b.z),
								 (a.x * b.y) - (a.y * b.x));}

inline float
dot(const Vec3F& a, const Vec3F& b) {return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));}

inline float
dot(const Vec3F& v) {return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z));}

inline float
magnitude(const Vec3F& v) {return (sqrt(dot(v)));}

inline Vec3F
normalize(const Vec3F& v) {return (v * (1.0f / magnitude(v)));}

inline void
print(const Vec3F& v) {printf("(%f, %f, %f)\n", v.x, v.y, v.z);}

// Struct Vec4F //

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
    c_float&     operator [](int i) const;
} Vec4F;

inline bool
operator ==(const Vec4F& a, const Vec4F& b)
{
    return(a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

inline Vec4F
operator +(const Vec4F& a, const Vec4F& b)
{
    return Vec4F(a.x + b.x,
	         a.y + b.y,
	         a.z + b.z,
	         a.w + b.w);
}

inline Vec4F
operator -(const Vec4F& a, const Vec4F& b)
{
    return Vec4F(a.x - b.x,
	         a.y - b.y,
	         a.z - b.z,
	         a.w - b.w);
}

inline Vec4F
operator *(const Vec4F& v, float s)
{
    return Vec4F(v.x * s,
	         v.y * s,
	         v.z * s,
	         v.w * s);
}

inline Vec4F
operator *(float s, const Vec4F& v)
{
    return Vec4F(v.x * s,
	         v.y * s,
	         v.z * s,
	         v.w * s);
}

inline float
dot(const Vec4F& a, const Vec4F& b)
{
    return ((a.x * b.x) +
	    (a.y * b.y) +
	    (a.z * b.z) +
	    (a.w * b.w));
}

inline float
dot(const Vec4F& v)
{
    return ((v.x * v.x) +
	    (v.y * v.y) +
	    (v.z * v.z) +
	    (v.w * v.w));
}

inline float
magnitude(const Vec4F& v) {return(sqrt(dot(v)));}

inline Vec4F
normalize(const Vec4F& v) {return (v * (1.0f / magnitude(v)));}

inline void
print(const Vec4F& v) {printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);}

//  NOTE: All Matrix implementations use Column-Major storage, and all Matrix operations and derivations
//       assume a Right-Handed coordinate system.

// Struct Mat3F //

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
    Mat3F(const Mat3F& m);
    void         operator *=(float s);
    void         operator +=(const Mat3F& m);
    void         operator -=(const Mat3F& m);
    float&       operator ()(int i, int j);
    c_float&     operator ()(int i, int j) const;
    c_float* getPointer();
} Mat3F;

inline Mat3F
operator +(const Mat3F& a, const Mat3F& b)
{
    return Mat3F(a(0, 0) + b(0, 0), a(1, 0) + b(1, 0), a(2, 0) + b(2, 0),
	         a(0, 1) + b(0, 1), a(1, 1) + b(1, 1), a(2, 1) + b(2, 1),
	         a(0, 2) + b(0, 2), a(1, 2) + b(1, 2), a(2, 2) + b(2, 2));
}

inline Mat3F
operator -(const Mat3F& a, const Mat3F& b)
{
    return Mat3F(a(0, 0) - b(0, 0), a(1, 0) - b(1, 0), a(2, 0) - b(2, 0),
	         a(0, 1) - b(0, 1), a(1, 1) - b(1, 1), a(2, 1) - b(2, 1),
	         a(0, 2) - b(0, 2), a(1, 2) - b(1, 2), a(2, 2) - b(2, 2));
}

inline Mat3F
operator *(const Mat3F& a, const Mat3F& b)
{
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

inline Vec3F
operator *(const Mat3F& m, const Vec3F& v)
{
    return Vec3F(m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z,
		 m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z,
		 m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z);
}

inline Mat3F
transpose(const Mat3F& m)
{
    return Mat3F(m(0, 0), m(0, 1), m(0, 2),
		 m(1, 0), m(1, 1), m(1, 2),
		 m(2, 0), m(2, 1), m(2, 2));
}

inline void
print(const Mat3F& m)
{
    for(int x = 0; x < 3; x++)
    {
	printf("\n");
	for(int y = 0; y < 3; y++)
	    printf("%f, ", m(y, x));
    }
    printf("\n");
}

// Struct Mat4F //

typedef struct Mat4F
{
private:
    float n[4][4];
public:
    Mat4F();
    Mat4F(float n00, float n01, float n02, float n03,
	  float n10, float n11, float n12, float n13,
	  float n20, float n21, float n22, float n23,
	  float n30, float n31, float n32, float n33);
    Mat4F(float f);
    Mat4F(const Vec4F& a, const Vec4F& b, const Vec4F& c, const Vec4F& d);
    Mat4F(const Mat4F& m);
    Mat4F(const Mat3F& m);
    void         operator *=(float s);
    void         operator +=(const Mat4F& m);
    void         operator -=(const Mat4F& m);
    float&       operator ()(int i, int j);
    c_float& operator ()(int i, int j) const;
    c_float* getPointer();
} Mat4F;

inline Mat4F
operator +(const Mat4F& a, const Mat4F& b)
{
    return Mat4F(a(0, 0) + b(0, 0), a(1, 0) + b(1, 0), a(2, 0) + b(2, 0), a(3, 0) + b(3, 0),
		 a(0, 1) + b(0, 1), a(1, 1) + b(1, 1), a(2, 1) + b(2, 1), a(3, 1) + b(3, 1),
		 a(0, 2) + b(0, 2), a(1, 2) + b(1, 2), a(2, 2) + b(2, 2), a(3, 2) + b(3, 2),
		 a(0, 3) + b(0, 3), a(1, 3) + b(1, 3), a(2, 3) + b(2, 3), a(3, 3) + b(3, 3));
}

inline Mat4F
operator -(const Mat4F& a, const Mat4F& b)
{
    return Mat4F(a(0, 0) - b(0, 0), a(1, 0) - b(1, 0), a(2, 0) - b(2, 0), a(3, 0) - b(3, 0),
		 a(0, 1) - b(0, 1), a(1, 1) - b(1, 1), a(2, 1) - b(2, 1), a(3, 1) - b(3, 1),
		 a(0, 2) - b(0, 2), a(1, 2) - b(1, 2), a(2, 2) - b(2, 2), a(3, 2) - b(3, 2),
		 a(0, 3) - b(0, 3), a(1, 3) - b(1, 3), a(2, 3) - b(2, 3), a(3, 3) - b(3, 3));
}

inline Mat4F
operator *(const Mat4F& a, const Mat4F& b)
{
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

inline Vec4F
operator *(const Mat4F& m, const Vec4F& v)
{
    return Vec4F(m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z + m(0, 3) * v.w,
		 m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z + m(1, 3) * v.w,
		 m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z + m(2, 3) * v.w,
		 m(3, 0) * v.x + m(3, 1) * v.y + m(3, 2) * v.z + m(3, 3) * v.w);
}

inline Mat4F
transpose(const Mat4F& m)
{
    return Mat4F(m(0, 0), m(0, 1), m(0, 2), m(0, 3),
		 m(1, 0), m(1, 1), m(1, 2), m(1, 3),
		 m(2, 0), m(2, 1), m(2, 2), m(2, 3),
		 m(3, 0), m(3, 1), m(3, 2), m(3, 3));
}

inline void
print(const Mat4F& m)
{
    for(int x = 0; x < 4; x++)
    {
	printf("\n");
	for(int y = 0; y < 4; y++)
	    printf("%f, ", m(y, x));
    }
    printf("\n");
}

// Struct Quaternion //

typedef struct Quaternion {
    float w;
    float x;
    float y;
    float z;
    Quaternion();
    Quaternion(float _w, float _x, float _y, float _z);
    Quaternion(float _W, const Vec3F& v);
    void operator +=(const Quaternion& q);
    void operator -=(const Quaternion& q);
} Quaternion;

inline Quaternion
operator +(const Quaternion& q1, const Quaternion& q2)
{
    return Quaternion(q1.w + q2.w,
	              q1.x + q2.x,
		      q1.y + q2.y,
		      q1.z + q2.z);
}

inline Quaternion
operator -(const Quaternion& q1, const Quaternion& q2)
{
    return Quaternion(q1.w - q2.w,
	              q1.x - q2.x,
		      q1.y - q2.y,
		      q1.z - q2.z);
}

inline Quaternion
operator *(const Quaternion& q1, const Quaternion& q2)
{
    Vec3F v1(q1.x, q1.y, q1.z);
    Vec3F v2(q2.x, q2.y, q2.z);

    float w = (q1.w * q2.w) - dot(v1, v2);
    Vec3F v = (q1.w * v2) + (q2.w * v1) + cross(v1, v2);

    return Quaternion(w, v);
}

inline Quaternion
operator *(const Quaternion& q, float s)
{
    return Quaternion(q.w * s,
	              q.x * s,
	              q.y * s,
	              q.z * s);
}

inline Quaternion
operator *(float s, const Quaternion& q)
{
    return Quaternion(q.w * s,
	              q.x * s,
	              q.y * s,
	              q.z * s);
}

inline float
dot(const Quaternion& q1, const Quaternion& q2)
{
    return (q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);
}

inline float
dot(const Quaternion& q)
{
    return (q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

inline float
magnitude(const Quaternion& q)
{
    return sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

inline Quaternion
normalize(const Quaternion& q)
{
    float lengthRecip = (1.0f / magnitude(q));
    return Quaternion(q.w * lengthRecip,
		      q.x * lengthRecip,
		      q.y * lengthRecip,
		      q.z * lengthRecip);
}

inline Quaternion
inverse(const Quaternion& q)
{
    // Assumes a unit quaternion is given
    return Quaternion(q.w, -q.x, -q.y, -q.z);
}

inline Mat3F
quatToMat3(const Quaternion& q)
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

inline void
print(const Quaternion& q)
{
    printf("(%f, (%f, %f, %f))\n", q.w, q.x, q.y, q.z);
}

// Interpolation //

Quaternion
slerp(const Quaternion& q1, const Quaternion& q2, double t);

// Misc. //

inline float
lerp(float a, float b, float t) {return a + (b - a) * t;}

inline Vec3F
vlerp(const Vec3F& a, const Vec3F& b, float t)
{
    return Vec3F(a.x + (b.x - a.x) * t,
		 a.y + (b.y - a.y) * t,
		 a.z + (b.z - a.z) * t);
}

inline float
clamp(float n, float min, float max)
{
    float t = n < min ? min : n;
    return t > max ? max : t;
}

inline float
degToRads(float d) {return (float)(d * 0.0174532925);}

inline Vec3F
rotate(const Vec3F& v, const Quaternion& q)
{
    float vMult     = 2.0f * (q.x * v.x + q.y * v.y * q.z * v.z);
    float crossMult = 2.0f * q.w;
    float pMult     = crossMult * q.w - 1.0f;

    return Vec3F(pMult * v.x + vMult * q.x + crossMult * (q.y * v.z - q.z * v.y),
	         pMult * v.y + vMult * q.y + crossMult * (q.z * v.x - q.x * v.z),
	         pMult * v.z + vMult * q.z + crossMult * (q.x * v.y - q.y * v.x));
}

inline Mat4F
lookAt(const Vec3F& eye, const Vec3F& at, const Vec3F& up)
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

inline Mat4F
getOrthoProjection(float l, float r, float b, float t, float n, float f)
{
        return Mat4F(2/(r - l), 0.0f, 0.0f,  -(r + l) / (r - l),
		 0.0f, 2/(t - b), 0.0f,  -(t + b) / (t - b),
		 0.0f, 0.0f, -2/(f - n), -(f + n) / (f - n),
		 0.0f, 0.0f, 0.0f, 1.0f);
}

inline Mat4F
getModelMat(const Vec3F& scale, const Vec3F& translation)
{
    Mat4F model = Mat4F(scale.x, 0.0f, 0.0f, translation.x,
	                0.0f, scale.y, 0.0f, translation.y,
	                0.0f, 0.0f, scale.z, translation.z,
	                0.0f, 0.0f, 0.0f, 1.0f);
    return model;
}

#endif
