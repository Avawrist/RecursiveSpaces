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
#include "game_window.hpp"
#include "input.hpp"

typedef struct Camera
{
    Vec3F pos;
    float speed;
    float z_speed;
    float sensitivity;
    float pitch;
    float yaw;
    float n;
    float f;
    float fov;
    float ar;
    Camera();
    Camera(Vec3F _pos, float _ar);
} Camera;
void  cameraUpdate(Camera& cam, GameWindow& game_window, InputManager& input_manager);
void  cameraOffsetAngles(Camera& cam, float o_yaw, float o_pitch);
Mat4F cameraGetView(const Camera& cam);
Mat4F cameraGetPerspective(const Camera& cam);
Mat4F cameraGetOrthographic(const Camera& cam, const GameWindow& window);

#endif
