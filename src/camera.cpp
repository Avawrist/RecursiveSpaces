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
    pos   = Vec3F(0.0f, 0.0f, 0.0f);
    n     = 1.0f;
    f     = 100.0f;
    fov   = 45.0f;
    ar    = 1.0f;
}

Camera::Camera(Vec3F _pos, float _n, float _f, float _fov, float _ar)
{
    pos   = _pos;
    n     = _n;
    f     = _f;
    fov   = _fov;
    ar    = _ar;
}

////////////////////////
// Camera Non-Members //
////////////////////////

void cameraUpdate(Camera& cam, GLFWwindow* window, const Vec2F& distance, float d_time)
{
    /////////////////////
    // Camera Strafing //
    /////////////////////

    // polling GLFW for input
    // TODO: Create our own input data structure and poll that instead?
    
    Mat4F V          = cameraGetView(cam);
    float d_time_spd = d_time * cam.speed; 
    // Move forward
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
	cam.pos -= normalize(Vec3F(V(0, 2), V(1, 2), V(2, 2))) * d_time_spd;
    }
    // Move back
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cam.pos += normalize(Vec3F(V(0, 2), V(1, 2), V(2, 2))) * d_time_spd;
    }
    // Move right
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cam.pos += normalize(Vec3F(V(0, 0), V(1, 0), V(2, 0))) * d_time_spd;
    }
    // Move left
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cam.pos -= normalize(Vec3F(V(0, 0), V(1, 0), V(2, 0))) * d_time_spd;
    }

    /////////////////
    // Camera Zoom //
    /////////////////
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cam.fov -= 1.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cam.fov += 1.0f;
    }
    cam.fov = clamp(cam.fov, 45.0f, 115.0f);

    ////////////////////
    // Camera Looking //
    ////////////////////
    cameraOffsetAngles(cam,
		       cam.sensitivity * distance.x,
		       cam.sensitivity * distance.y);
}

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
    // Assumes RH coordinate systems
    // Assumes Column major
    
    float range        = cam.n - cam.f;
    float tan_half_fov = tan(degToRads(cam.fov) * 0.5f);

    return Mat4F(1.0f/(cam.ar * tan_half_fov), 0.0f, 0.0f, 0.0f,                   // Row 1
	         0.0f, 1.0f/tan_half_fov, 0.0f, 0.0f,                              // Row 2
	         0.0f, 0.0f, (-cam.n - cam.f)/range, (2.0f * cam.f * cam.n)/range, // Row 3
	         0.0f, 0.0f, 1.0f, 0.0f);                                          // Row 4
    
}
