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
    pos  = Vec3F(0.0f, 0.0f, 0.0f);
    n    = 1.0f;
    f    = 100.0f;
    fov  = 45.0f;
    ar   = 1.0f;
}

Camera::Camera(const Vec3F& _pos, float _n, float _f, float _fov, float _ar)
{
    pos  = _pos;
    n    = _n;
    f    = _f;
    fov  = _fov;
    ar   = _ar;
}

////////////////////////
// Camera Non-Members //
////////////////////////

void cameraOffsetAngles(Camera& cam, float oYaw, float oPitch)
{
    cam.yaw   += oYaw;
    cam.pitch += oPitch;

    if(cam.yaw > 360.0f) {cam.yaw -= 360.0f;}
    if(cam.yaw < 0.0f) {cam.yaw += 360.0f;}
    cam.pitch = clamp(cam.pitch, -90.0f, 90.0f);
}

Mat4F cameraGetOrientation(const Camera& cam)
{
    float half_yaw_rads   = degToRads(cam.yaw) * 0.5f;
    float half_pitch_rads = degToRads(cam.pitch) * 0.5f;
    Quaternion x_rot(cos(half_yaw_rads), 0.0f, sin(half_yaw_rads), 0.0f);
    Quaternion y_rot(cos(half_pitch_rads), sin(half_pitch_rads), 0.0f, 0.0f);
    Quaternion o = normalize(x_rot * y_rot);
        
    return Mat4F(quatToMat3(o)); // Orthogonal rotation matrix
}

Mat4F cameraGetPerspective(const Camera& cam)
{
    float range        = cam.n - cam.f;
    float tan_half_fov = tan(degToRads(cam.fov) * 0.5f);

    return Mat4F(1.0f/(cam.ar * tan_half_fov), 0.0f, 0.0f, 0.0f,                   // Row 1
	         0.0f, 1.0f/tan_half_fov, 0.0f, 0.0f,                              // Row 2
	         0.0f, 0.0f, (-cam.n - cam.f)/range, (2.0f * cam.f * cam.n)/range, // Row 3
	         0.0f, 0.0f, 1.0f, 0.0f);                                          // Row 4
    
}

Mat4F cameraGetTranslation(const Camera& cam)
{
    return Mat4F(1.0f,      0.0f,      0.0f,      0.0f,
		 0.0f,      1.0f,      0.0f,      0.0f,
		 0.0f,      0.0f,      1.0f,      0.0f,
		 cam.pos.x, cam.pos.y, cam.pos.z, 1.0f);
}

Mat4F cameraGetTranslationInverse(const Camera& cam)
{
    return Mat4F(1.0f,       0.0f,       0.0f,       0.0f,
		 0.0f,       1.0f,       0.0f,       0.0f,
		 0.0f,       0.0f,       1.0f,       0.0f,
		 -cam.pos.x, -cam.pos.y, -cam.pos.z, 1.0f);
}

Mat4F cameraGetView(const Camera& cam)
{
    return (cameraGetTranslationInverse(cam) * cameraGetOrientation(cam));
}

/*
void cameraOffsetAngles(Camera& cam, float oYaw, float oPitch)
{
    cam.yaw   += oYaw;
    cam.pitch += oPitch;

    if(cam.yaw > 360.0f) {cam.yaw -= 360.0f;}
    if(cam.yaw < 0.0f) {cam.yaw += 360.0f;}
    cam.pitch = clamp(cam.pitch, -90.0f, 90.0f);
}

Mat4F cameraGetPerspective(const Camera& cam)
{
    float range        = cam.n - cam.f;
    float tan_half_fov = tan(degToRads(cam.fov) * 0.5f);

    return Mat4F(1.0f/(cam.ar * tan_half_fov), 0.0f, 0.0f, 0.0f,                   // Row 1
	         0.0f, 1.0f/tan_half_fov, 0.0f, 0.0f,                              // Row 2
	         0.0f, 0.0f, (-cam.n - cam.f)/range, (2.0f * cam.f * cam.n)/range, // Row 3
	         0.0f, 0.0f, 1.0f, 0.0f);                                          // Row 4
    
}

Mat4F cameraGetRotation(const Camera& cam)
{
    float yaw_rads   = degToRads(cam.yaw);
    float pitch_rads = degToRads(cam.pitch);
    float cos_yaw    = cos(yaw_rads);
    float cos_pitch  = cos(pitch_rads);
    float sin_yaw    = sin(yaw_rads);
    float sin_pitch  = sin(pitch_rads);
    
    return Mat4F(cos_yaw,  sin_yaw * sin_pitch, sin_yaw * cos_pitch,  0.0f,
	         0.0f,     cos_pitch,           -sin_pitch,           0.0f,
	         -sin_yaw, cos_yaw * sin_pitch, cos_pitch * cos_yaw,  0.0f,
	         0.0f,     0.0f,                0.0f,                 1.0f);
}

Mat4F cameraGetTranslation(const Camera& cam)
{
    return Mat4F(1.0f,      0.0f,      0.0f,      0.0f,
		 0.0f,      1.0f,      0.0f,      0.0f,
		 0.0f,      0.0f,      1.0f,      0.0f,
		 cam.pos.x, cam.pos.y, cam.pos.z, 1.0f);
}

Mat4F cameraGetTranslationInverse(const Camera& cam)
{
    return Mat4F(1.0f,       0.0f,       0.0f,       0.0f,
		 0.0f,       1.0f,       0.0f,       0.0f,
		 0.0f,       0.0f,       1.0f,       0.0f,
		 -cam.pos.x, -cam.pos.y, -cam.pos.z, 1.0f);
}
*/
