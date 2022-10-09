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
    float fov;
    float n;
    float f;
    float ar;
    float pitch         = 0.0f;
    float yaw           = 0.0f;
    float move_speed    = 0.2;
    float rot_speed_deg = 2.0f;
    Camera();
    Camera(const Vec3F& _pos, float _n, float _f, float _fov, float _ar);
} Camera;
void  cameraOffsetAngles(Camera& cam, float oYaw, float oPitch);
Mat4F cameraGetOrientation(const Camera& cam);
Mat4F cameraGetPerspective(const Camera& cam);
Mat4F cameraGetTranslation(const Camera& cam);
Mat4F cameraGetTranslationInverse(const Camera& cam);
Mat4F cameraGetView(const Camera& cam);

#endif
