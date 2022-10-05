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
    orientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

Camera::Camera(const Vec3F& _pos, const Quaternion& _orientation)
{
    pos = _pos;
    orientation = _orientation;
}

////////////////////////
// Camera Non-Members //
////////////////////////

void updateRotFromMouse(Camera& cam, const Vec2F& mouseDist)
{
    // **Assumes mouseDist is normalized**

    // radians scaled by distance mouse traveled, halved for quaternion:
    float half_x_degrees = mouseDist.x * cam.rot_speed * 0.5f;; 
    float half_y_degrees = mouseDist.y * cam.rot_speed * 0.5f;;

    // rotations as quaternions
    Quaternion x_rot(cos(half_x_degrees), 0.0f, sin(half_x_degrees), 0.0f);
    Quaternion y_rot(cos(half_y_degrees), sin(half_y_degrees), 0.0f, 0.0f);

    // Apply new rotation to orientation
    cam.orientation = cam.orientation * normalize(x_rot * y_rot); 
}

Mat4F getView(const Camera& cam)
{
    //Quaternion inverseRot = inverse(cam.orientation);
    Mat4F R = Mat4F(quatToMat3(cam.orientation));
    Mat4F T = Mat4F(1.0f,       0.0f,       0.0f,       0.0f,
	            0.0f,       1.0f,       0.0f,       0.0f,
	            0.0f,       0.0f,       1.0f,       0.0f,
	            -cam.pos.x, -cam.pos.y, -cam.pos.z, 1.0f);
    
    return T * R; // The inverse of the rotation matrix
                  // by the inverse of the translation matrix
                  // gives the inverse of the camera's model matrix
}
