// ====================================================
// Title: input.hpp
// Description: The header file for input processing
// ====================================================

#ifndef INPUT_H
#define INPUT_H

// Win libs
#include <cstring>

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"
#include "render.hpp"
#include "mdcla.hpp"

///////////////////
// Struct Cursor //
///////////////////

typedef struct Cursor
{
    double x_pos;
    double y_pos;
    float last_x;
    float last_y;
    bool  first_mouse;
    Cursor();
    Cursor(float _last_x, float _last_y);
} Cursor;
void cursorUpdate(Cursor& cursor, GLFWwindow* window);
Vec2F cursorGetDistance(Cursor& cursor);

/////////////////////////
// Struct InputManager //
/////////////////////////

#define KEY_ARROW_UP    0
#define KEY_ARROW_DOWN  1
#define KEY_ARROW_LEFT  2
#define KEY_ARROW_RIGHT 3
#define KEY_SPACE 4
#define TOTAL_KEYS 5

#define KEY_UP   0
#define KEY_DOWN 1

#define FRAME_1_PRIOR 0
#define FRAME_2_PRIOR 1
#define FRAME_3_PRIOR 2
#define FRAME_4_PRIOR 3
#define FRAME_5_PRIOR 4
#define FRAME_6_PRIOR 5
#define FRAME_7_PRIOR 6
#define FRAME_8_PRIOR 7
#define TOTAL_FRAMES 8

typedef struct InputManager
{
    int inputs_on_frame[TOTAL_FRAMES][TOTAL_KEYS];
    //Cursor* cursor_p = NULL; 
    InputManager();
    //~InputManager();
} InputManager;
void inputManagerUpdate(InputManager& im, GameWindow& game_window);
void inputManagerGetInputsThisFrame(InputManager& im, GameWindow& game_window);

#endif

