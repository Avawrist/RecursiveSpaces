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
    float speed       = 0.25f;
    float sensitivity = 0.75f;
    float pitch       = 0.0f;
    float yaw         = 0.0f;
    float n;
    float f;
    float fov;
    float ar;
    Camera();
    Camera(Vec3F _pos, float _n, float _f, float _fov, float _ar);
} Camera;
void  cameraOffsetAngles(Camera& cam, float o_pitch, float o_yaw);
Mat4F cameraGetView(const Camera& cam);
Mat4F cameraGetPerspective(const Camera& cam);

#endif
