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
    float pitch       = 0.0f;
    float yaw         = 0.0f;
    float speed       = 0.2;
    float sensitivity = 3.0f;
    Camera();
    Camera(const Vec3F& _pos, float _n, float _f, float _fov, float _ar);
} Camera;
void  cameraOffsetAngles(Camera& cam, float oYaw, float oPitch);
Mat4F cameraGetOrientation(const Camera& cam);
Mat4F cameraGetPerspective(const Camera& cam);
Mat4F cameraGetTranslation(const Camera& cam);
Mat4F cameraGetTranslationInverse(const Camera& cam);
Mat4F cameraGetView(const Camera& cam);

/*
typedef struct Camera
{
    Vec3F pos;
    Vec3F dir;
    flaot fov;
    float n;
    float f;
    float ar;
    float pitch;
    float yaw;
    float sensitivity;
    float speed;
    Camera();
    Camera(const Vec3F& _pos, float n, float f, float _fov, float _ar);
};
void  cameraOffsetAngles(Camera& cam, float oYaw, float oPitch);
Mat4F cameraGetPerspective(const Camera& cam);
Mat4F cameraGetRotation(const Camera& cam);
Mat4F cameraGetTranslation(const Camera& cam);
Mat4F cameraGetTranslationInverse(const Camera& cam);
*/
#endif
