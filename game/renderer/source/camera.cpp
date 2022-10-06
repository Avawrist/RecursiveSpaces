// ======================================================================
// Title: camera.cpp
// Description: The source file for camera objects
// ======================================================================

#include <camera.hpp>

///////////////////
// Struct Camera //
///////////////////

Camera::Camera()
{
    pos         = Vec3F(0.0f, 0.0f, 0.0f);
}

Camera::Camera(const Vec3F& _pos)
{
    pos = _pos;
}

////////////////////////
// Camera Non-Members //
////////////////////////

void cameraOffsetAngles(Camera& cam, float oYaw, float oPitch)
{
    cam.yaw   += oYaw;
    cam.pitch += oPitch;
}

Mat4F getOrientation(const Camera& cam)
{
    float halfYawRads   = degToRads(cam.yaw) * 0.5f;
    float halfPitchRads = degToRads(cam.pitch) * 0.5f;
    Quaternion x_rot(cos(halfYawRads), 0.0f, sin(halfYawRads), 0.0f);
    Quaternion y_rot(cos(halfPitchRads), sin(halfPitchRads), 0.0f, 0.0f);
    Quaternion o = normalize(y_rot * x_rot);

    return Mat4F(quatToMat3(o));
}

Mat4F getTranslation(const Camera& cam)
{
    return Mat4F(1.0f,      0.0f,      0.0f,      0.0f,
	         0.0f,      1.0f,      0.0f,      0.0f,
	         0.0f,      0.0f,      1.0f,      0.0f,
	         cam.pos.x, cam.pos.y, cam.pos.z, 1.0f);
}

Mat4F getTranslationInverse(const Camera& cam)
{
    return Mat4F(1.0f,       0.0f,       0.0f,       0.0f,
	         0.0f,       1.0f,       0.0f,       0.0f,
	         0.0f,       0.0f,       1.0f,       0.0f,
	         -cam.pos.x, -cam.pos.y, -cam.pos.z, 1.0f);
}

Mat4F getView(const Camera& cam)
{
    return (getTranslationInverse(cam) * getOrientation(cam));
}
