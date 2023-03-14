// ====================================================================================
// Title: platform.hpp
// Description: The header file for the platform layer, abstracting the
//              platform dependent APIs
// ====================================================================================

#ifndef PLATFORM_H
#define PLATFORM_H

// Win libs
#include <windows.h>
#include <stdio.h>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// My libs
#include "utility.hpp"
#include "input.hpp"
#include "asset.hpp"
#include "camera.hpp"
#include "draw.hpp"
#include "light.hpp"

///////////////////////
// Struct GameWindow //
///////////////////////

typedef struct GameWindow
{
    void*  window_p;
    uint   win_width;
    uint   win_height;
    uint   view_width;
    uint   view_height;
    float  x_center;
    float  y_center;
    float  win_ar;
    uint   target_framerate;
    double target_cycle_length_secs;  
    double cycle_start_time_secs;
    float  d_time;
    bool   sleep_is_granular;
    bool   close;
} GameWindow;

//////////////////////////////////
// Platform Function Prototypes //
//////////////////////////////////

int platformInitAPIs(GameWindow& game_window);

int platformInitWindow(GameWindow& game_window, uint _width, uint _height, c_char* name);

void platformFreeWindow(GameWindow& game_window);

void platformSwapBuffers(GameWindow& game_window);

void platformGetInputsThisFrame(InputManager &im, GameWindow &game_window);

void platformSetRenderStateDefault(GameWindow& game_window, FrameTexture& framebuffer);

void platformPrepShaderDefault(GameWindow& game_window, AssetManager& asset_manager,
			       Camera& camera, DirLight& dir_light);

void platformPrepShaderDebug(GameWindow& game_window, AssetManager& asset_manager, Camera& camera);

void platformRenderEntity(AssetManager& asset_manager, uint entity_type, Mat4F model);

void platformRenderDebug(AssetManager& asset_manager, DebugGrid* grid_p);

void platformRenderPP(AssetManager& asset_manager, GameWindow& game_window, FrameTexture* ftexture_p);

////////////////////
// Init Functions //
////////////////////

int platformInitGLFW();

int platformInitOpenGL();

/////////////////////
// Debug Functions //
/////////////////////

void outputGLFWError();

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                       GLsizei length, const char* message, const void* userParam);


#endif 
