// ======================================================================
// Title: camera.hpp
// Description: The header file for camera objects
// ======================================================================

#ifndef CAMERA_H
#define CAMERA_H

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"
#include "mdcla.hpp"
#include "render.hpp"
#include "input.hpp"

typedef struct Camera
{
    Vec3F pos;
    float speed       = 2.0f;
    float z_speed     = 20.0f;
    float sensitivity = 1.0f;
    float pitch       = 0.0f;
    float yaw         = 0.0f;
    float n;
    float f;
    float fov;
    float ar;
    Camera();
    Camera(Vec3F _pos, float _n, float _f, float _fov, float _ar);
} Camera;
void  cameraUpdate(Camera& cam, GameWindow& game_window, InputManager& input_manager);
void  cameraOffsetAngles(Camera& cam, float o_yaw, float o_pitch);
Mat4F cameraGetView(const Camera& cam);
Mat4F cameraGetPerspective(const Camera& cam);
Mat4F cameraGetOrthographic();

#endif
