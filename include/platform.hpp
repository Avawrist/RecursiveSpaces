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

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// My libs
#include "utility.hpp"
#include "input.hpp"
#include "asset.hpp"
#include "draw.hpp"

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

int platformInitGLFW();

int platformInitOpenGL();

int platformLoadXAudio2();

int platformInitWindow(GameWindow& game_window, uint _width, uint _height, c_char* name);

void platformFreeWindow(GameWindow& game_window);

void platformSwapBuffers(GameWindow& game_window);

void platformGetInputsThisFrame(InputManager &im, GameWindow &game_window);

void platformRenderShadowMapToScreen(AssetManager& asset_manager,
				     GameWindow& game_window,
				     FrameTexture& depth_framebuffer);

void platformRenderShadowMapToBuffer(const ActiveEntities& active_entities,
				     const FrameTexture& depth_framebuffer,
				     AssetManager& asset_manager,
				     const GameWindow& game_window,
				     const Vec3F& light_pos,
				     const Vec3F& look_at);

void platformRenderEntitiesToBuffer(const ActiveEntities& active_entities,
				    const FrameTexture& framebuffer,
				    const GameWindow& game_window,
				    AssetManager& asset_manager,
				    Vec3F cam_pos,
				    Vec3F cam_target,
				    const DirLight& dir_light);

void platformRenderDebugElementsToBuffer(const GameWindow& game_window,
					 AssetManager& asset_manager,
					 Vec3F cam_pos,
					 Vec3F cam_target,
					 DebugGrid* grid_p);

void platformRenderPP(AssetManager& asset_manager,
		      const FrameTexture& non_msaa_framebuffer);
    
int platformLoadEntityTemplatesFromTxt(ActiveEntities& active_entities, c_char* path);

/////////////////////
// Debug Functions //
/////////////////////

void outputGLFWError();

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                       GLsizei length, const char* message, const void* userParam);


#endif 
