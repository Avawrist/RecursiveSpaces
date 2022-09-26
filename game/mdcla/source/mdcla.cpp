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

void Vec2F::print()
{
    printf("\n(%f, %f)\n", x, y);
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

void Vec3F::print()
{
    printf("\n(%f, %f, %f)\n", x, y, z);
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

void Vec4F::print()
{
    printf("\n(%f, %f, %f, %f)\n", x, y, z, w);
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

Mat3F::Mat3F(float f)
{
    n[0][0] = f;    n[0][1] = 0.0f; n[0][2] = 0.0f; 
    n[1][0] = 0.0f; n[1][1] = f;    n[1][2] = 0.0f;
    n[2][0] = 0.0f; n[2][1] = 0.0f; n[2][2] = f;
}

Mat3F::Mat3F(const Vec3F& a, const Vec3F& b, const Vec3F& c)
{
    n[0][0] = a.x; n[0][1] = b.x; n[0][2] = c.x; 
    n[1][0] = a.y; n[1][1] = b.y; n[1][2] = c.y;
    n[2][0] = a.z; n[2][1] = b.z; n[2][2] = c.z;
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

Vec3F Mat3F::operator [](int j)
{
    return Vec3F(n[0][j], n[1][j], n[2][j]);
}

const Vec3F Mat3F::operator [](int j) const
{
    return Vec3F(n[0][j], n[1][j], n[2][j]);
}

float& Mat3F::operator ()(int i, int j)
{
    return n[i][j];
}

const float& Mat3F::operator ()(int i, int j) const
{
    return n[i][j];
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

void Mat3F::transpose()
{
    Mat3F temp = *this;

    n[0][0] = temp(0, 0); n[0][1] = temp(1, 0); n[0][2] = temp(2, 0);
    n[1][0] = temp(0, 1); n[1][1] = temp(1, 1); n[1][2] = temp(2, 1);
    n[2][0] = temp(0, 2); n[2][1] = temp(1, 2); n[2][2] = temp(2, 2);
}

const float* Mat3F::getPointer()
{
    return &(n[0][0]);
}

// Mat3F Non-Members

Mat3F operator +(const Mat3F& a, const Mat3F& b)
{
    return Mat3F(a(0, 0) + b(0, 0), a(0, 1) + b(0, 1), a(0, 2) + b(0, 2),
	         a(1, 0) + b(1, 0), a(1, 1) + b(1, 1), a(1, 2) + b(1, 2),
	         a(2, 0) + b(2, 0), a(2, 1) + b(2, 1), a(2, 2) + b(2, 2));
}

Mat3F operator -(const Mat3F& a, const Mat3F& b)
{
    return Mat3F(a(0, 0) - b(0, 0), a(0, 1) - b(0, 1), a(0, 2) - b(0, 2),
	         a(1, 0) - b(1, 0), a(1, 1) - b(1, 1), a(1, 2) - b(1, 2),
	         a(2, 0) - b(2, 0), a(2, 1) - b(2, 1), a(2, 2) - b(2, 2));
}

Mat3F operator *(const Mat3F& a, const Mat3F& b)
{
    return Mat3F(a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0) + a(0, 2) * b(2, 0),
	         a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1) + a(0, 2) * b(2, 1),
	         a(0, 0) * b(0, 2) + a(0, 1) * b(1, 2) + a(0, 2) * b(2, 2),
	         a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0) + a(1, 2) * b(2, 0),
		 a(1, 0) * b(0, 1) + a(1, 1) * b(1, 1) + a(1, 2) * b(2, 1),
		 a(1, 0) * b(0, 2) + a(1, 1) * b(1, 2) + a(1, 2) * b(2, 2),
		 a(2, 0) * b(0, 0) + a(2, 1) * b(1, 0) + a(2, 2) * b(2, 0),
		 a(2, 0) * b(0, 1) + a(2, 1) * b(1, 1) + a(2, 2) * b(2, 1),
		 a(2, 0) * b(0, 2) + a(2, 1) * b(1, 2) + a(2, 2) * b(2, 2));

}

Vec3F operator *(const Mat3F& m, const Vec3F& v)
{
    return Vec3F(m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z,
		 m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z,
		 m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z);
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
    n[0][0] = n00; n[0][1] = n01; n[0][2] = n02; n[0][3] = n03;
    n[1][0] = n10; n[1][1] = n11; n[1][2] = n12; n[1][3] = n13;
    n[2][0] = n20; n[2][1] = n21; n[2][2] = n22; n[2][3] = n23;
    n[3][0] = n30; n[3][1] = n31; n[3][2] = n32; n[3][3] = n33;
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
    n[0][0] = a.x; n[0][1] = b.x; n[0][2] = c.x; n[0][3] = d.x;
    n[1][0] = a.y; n[1][1] = b.y; n[1][2] = c.y; n[1][3] = d.y;
    n[2][0] = a.z; n[2][1] = b.z; n[2][2] = c.z; n[2][3] = d.z;
    n[3][0] = a.w; n[3][1] = b.w; n[3][2] = c.w; n[3][3] = d.w;    
}

Mat4F::Mat4F(const Mat3F& m)
{
    n[0][0] = m(0, 0); n[0][1] = m(0, 1); n[0][2] = m(0, 2); n[0][3] = 0.0f;
    n[1][0] = m(1, 0); n[1][1] = m(1, 1); n[1][2] = m(1, 2); n[1][3] = 0.0f;
    n[2][0] = m(2, 0); n[2][1] = m(2, 1); n[2][2] = m(2, 2); n[2][3] = 0.0f;
    n[3][0] = 0.0f;    n[3][1] = 0.0f;    n[3][2] = 0.0f;    n[3][3] = 1.0f;
}

void Mat4F::operator *=(float s)
{
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

Vec4F Mat4F::operator [](int j)
{
    return Vec4F(n[0][j], n[1][j], n[2][j], n[3][j]);
}

const Vec4F Mat4F::operator [](int j) const
{
    return Vec4F(n[0][j], n[1][j], n[2][j], n[3][j]);
}

float& Mat4F::operator ()(int i, int j)
{
    return n[i][j];
}

const float& Mat4F::operator ()(int i, int j) const
{
    return n[i][j];
}

void Mat4F::print()
{
    for(int x = 0; x < 4; x++)
    {
	printf("\n");
	for(int y = 0; y < 4; y++)
	    printf("%f, ", n[x][y]);
    }
    printf("\n");
}

void Mat4F::transpose()
{
    Mat4F temp = *this;

    n[0][0] = temp(0, 0); n[0][1] = temp(1, 0); n[0][2] = temp(2, 0); n[0][3] = temp(3, 0);
    n[1][0] = temp(0, 1); n[1][1] = temp(1, 1); n[1][2] = temp(2, 1); n[1][3] = temp(3, 1);
    n[2][0] = temp(0, 2); n[2][1] = temp(1, 2); n[2][2] = temp(2, 2); n[2][3] = temp(3, 2);
    n[3][0] = temp(0, 3); n[3][1] = temp(1, 3); n[3][2] = temp(2, 3); n[3][3] = temp(3, 3);    
}

const float* Mat4F::getPointer()
{
    return &(n[0][0]);
}

// Mat4F Non-members

// TO-DO: implement all
Mat4F operator +(const Mat4F& a, const Mat4F& b)
{
    Mat4F m;
    return m;
}

Mat4F operator -(const Mat4F& a, const Mat4F& b)
{
    Mat4F m;
    return m;
}

Mat4F operator *(const Mat4F& a, const Mat4F& b)
{
    Mat4F m;
    return m;
}

Vec4F operator *(const Mat4F& m, const Vec4F& v)
{
    Vec4F v;
    return v;
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

Quaternion::Quaternion(float theta, float _x, float _y, float _z)
{
    float sinThetaHalf = sin(theta * 0.5f);
    
    // normalize the input vector 
    Vec3F v(_x, _y, _z);
    v = normalize3F(v);
    
    w = cos(theta * 0.5f);
    x = sinThetaHalf * v.x;
    y = sinThetaHalf * v.y;
    z = sinThetaHalf * v.z;
}

Quaternion::Quaternion(float theta, const Vec3F& v)
{
    float sinThetaHalf = sin(theta * 0.5f);

    // normalize the input vector
    Vec3F n = normalize3F(v);
    
    w = cos(theta * 0.5f);
    x = sinThetaHalf * n.x;
    y = sinThetaHalf * n.y;
    z = sinThetaHalf * n.z;
}

// Quaternion non-members

Quaternion operator *(const Quaternion& q1, const Quaternion& q2)
{
    // TO-DO: Implement function
    Quaternion q;
    return q;
}

///////////////////////////////
// Additional math functions //
///////////////////////////////

Mat3F quatToMat3(const Quaternion& q)
{
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

Mat4F getPerspectiveMat(float vfov, float ar, float n, float f)
{
    float range = n - f;
    float tanHalfFOV = tan(vfov * 0.5f);

    return Mat4F(1.0f/(ar * tanHalfFOV), 0.0f,            0.0f,           0.0f,
	         0.0f,                   1.0f/tanHalfFOV, 0.0f,           0.0f,
	         0.0f,                   0.0f,            (-n - f)/range, (2.0f * f * n)/range,
	         0.0f,                   0.0f,            1.0f,           0.0f);
}

/* TO-DO: Implement
Mat4F getOrthographicMat()
{
    printf("Function not implemented");
}
*/

float degToRads(float d)
{
    return (d * 0.0174532925);
}
