// ==========================================================================
// Title: game_window.hpp
// Description: The header file for the window struct, built on the GLFW API
// ==========================================================================

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

// Win libs
#include <windows.h>
#include <stdio.h>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"

///////////////////////
// Struct GameWindow //
///////////////////////

typedef struct GameWindow
{
    GLFWwindow* window_p = NULL;
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
    GameWindow(uint _width, uint _height, c_char* name);
    ~GameWindow();
} GameWindow;
void gameWindowSwapBuffers(GameWindow& game_window);
void gameWindowClose(GameWindow& game_window);

////////////////////
// Init Functions //
////////////////////

int initGLFW();
int initOpenGL();

/////////////////////
// Debug Functions //
/////////////////////

void outputGLFWError();
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                       GLsizei length, const char *message, const void *userParam);
#endif
