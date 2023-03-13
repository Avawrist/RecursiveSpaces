// ==========================================================================
// Title: game_window.hpp
// Description: The header file for the window struct, built on the GLFW API
// ==========================================================================

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

// My libs
#include "utility.hpp"

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

#endif
