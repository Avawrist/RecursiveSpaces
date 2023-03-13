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
#include "game_window.hpp"
#include "input.hpp"

int platformInitAPIs(GameWindow& game_window);

int platformInitWindow(GameWindow& game_window, uint _width, uint _height, c_char* name);

void platformFreeWindow(GameWindow& game_window);

void platformSwapBuffers(GameWindow& game_window);

void platformGetInputsThisFrame(InputManager &im, GameWindow &game_window);

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
