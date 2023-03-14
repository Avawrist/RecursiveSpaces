// ======================================================================
// Title: camera.cpp
// Description: The source file for camera objects
// ======================================================================

#include "camera.hpp"

///////////////////
// Struct Camera //
///////////////////

Camera::Camera()
{
    speed       = 8.0f;
    z_speed     = 20.0f;
    sensitivity = 1.0f;
    pitch       = -37.0f;
    yaw         = 45.0f;
    pos         = Vec3F(0.0f, 0.0f, 0.0f);
    n           = 0.8f;
    f           = 100.0f;
    fov         = 45.0f;
    ar          = 1.0f;
}

Camera::Camera(Vec3F _pos, float _ar)
{
    speed       = 8.0f;
    z_speed     = 20.0f;
    sensitivity = 1.0f;
    pitch       = -37.0f;
    yaw         = 45.0f;
    pos         = _pos;
    n           = 0.8f;
    f           = 100.0f;
    fov         = 45.0f;
    ar          = _ar;
}

//////////////////////
// Camera Functions //
//////////////////////

void cameraOffsetAngles(Camera& cam, float o_yaw, float o_pitch)
{
    cam.pitch -= o_pitch;
    cam.yaw   -= o_yaw;

    if (cam.yaw > 360.0f) {cam.yaw -= 360.0f;}
    if (cam.yaw < 0.0f) {cam.yaw += 360.0f;}
    cam.pitch = clamp(cam.pitch, -89.0f, 89.0f);
}

Mat4F cameraGetView(const Camera& cam)
{
    // Assumes pitch & yaw are stored in degrees
    // Assumes RH coordinate system
    // Assumes Column major
    
    float pitch_rads = degToRads(cam.pitch);
    float yaw_rads   = degToRads(cam.yaw);
    
    float cos_pitch = cos(pitch_rads);
    float cos_yaw   = cos(yaw_rads);
    float sin_pitch = sin(pitch_rads);
    float sin_yaw   = sin(yaw_rads);

    Vec3F x_axis(cos_yaw, 0.0f, -sin_yaw);
    Vec3F y_axis(sin_yaw * sin_pitch, cos_pitch, cos_yaw * sin_pitch);
    Vec3F z_axis(sin_yaw * cos_pitch, -sin_pitch, cos_pitch * cos_yaw);

    return Mat4F(x_axis.x,   y_axis.x,   z_axis.x,   0.0f,
		 x_axis.y,   y_axis.y,   z_axis.y,   0.0f,
		 x_axis.z,   y_axis.z,   z_axis.z,   0.0f,
		 -dot(x_axis, cam.pos), -dot(y_axis, cam.pos), -dot(z_axis, cam.pos), 1.0f);
}

Mat4F cameraGetPerspective(const Camera& cam)
{
    // Assumes RH coordinate system
    // Assumes Column major
    
    float range        = cam.n - cam.f;
    float tan_half_fov = tan(degToRads(cam.fov) * 0.5f);

    return Mat4F(1.0f/(cam.ar * tan_half_fov), 0.0f, 0.0f, 0.0f,                   // Row 1
	         0.0f, 1.0f/tan_half_fov, 0.0f, 0.0f,                              // Row 2
	         0.0f, 0.0f, (-cam.n - cam.f)/range, (2.0f * cam.f * cam.n)/range, // Row 3
	         0.0f, 0.0f, -1.0f, 0.0f);                                         // Row 4
    
}

Mat4F cameraGetOrthographic(const Camera& cam, int win_width, int win_height)
{
    // Assumes RH coordinate system
    // Assumes Column major

    float scaled_w = win_width / 10000.0f;
    float scaled_h = win_height / 10000.0f;
    
    // needs to match window aspect ratio
    float r = scaled_w / 2.0f;
    float l = -scaled_w / 2.0f;
    float t = scaled_h / 2.0f;
    float b = -scaled_h / 2.0f;
    float n = 0.0f;
    float f = 10.0f;

    return Mat4F(2.0f/(r - l), 0.0f,         0.0f,          -(r + l)/(r - l),
	         0.0f,         2.0f/(t - b), 0.0f,          -(t + b)/(t - b),
	         0.0f,         0.0f,         -2.0f/(f - n), -(f + n)/(f - n),
	         0.0f,         0.0f,          0.0f,          1.0f);
}
