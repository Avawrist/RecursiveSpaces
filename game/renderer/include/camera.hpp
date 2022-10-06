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
    Quaternion orientation;
    float move_speed = 2.0f;
    float rot_speed  = degToRads(2.0f);
    Camera();
    Camera(const Vec3F& _pos, const Quaternion& _orientation);
} Camera;
void updateRotFromMouse(Camera& cam, const Vec2F& mouseDist);
void updateCamPos(Camera& cam);
Mat4F getView(const Camera& cam);

#endif
