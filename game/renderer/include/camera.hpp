// ======================================================================
// Title: camera.hpp
// Description: The header file for camera objects
// ======================================================================

#ifndef CAMERA_H
#define CAMERA_H

#include <mdcla.hpp>

typedef struct Camera
{
    Vec3F pos;
    float pitch          = 0.0f;
    float yaw            = 0.0f;
    float move_speed     = 2.0f;
    float rot_speed_deg  = 2.0f;
    Camera();
    Camera(const Vec3F& _pos);
} Camera;
void  cameraOffsetAngles(Camera& cam, float oYaw, float oPitch);
Mat4F getOrientation(const Camera& cam);
Mat4F getTranslation(const Camera& cam);
Mat4F getView(const Camera& cam);

#endif
