// ====================================================
// Title: input.hpp
// Description: The header file for input processing
// ====================================================

#ifndef INPUT_H
#define INPUT_H

// Win libs
#include <cstring>

// My libs
#include "utility.hpp"
#include "mdcla.hpp"

///////////////////
// Struct Cursor //
///////////////////

typedef struct Cursor
{
    double x_pos;
    double y_pos;
    float  last_x;
    float  last_y;
    bool   first_mouse;
    Cursor();
} Cursor;
Vec2F cursorGetDistance(Cursor& cursor);

/////////////////////////
// Struct InputManager //
/////////////////////////

typedef enum Key
{
    KEY_ARROW_UP    = 0,
    KEY_ARROW_DOWN  = 1,
    KEY_ARROW_LEFT  = 2,
    KEY_ARROW_RIGHT = 3,
    KEY_SPACE       = 4,
    KEY_ESC         = 5,
    KEY_W           = 6,
    KEY_S           = 7,
    KEY_A           = 8,
    KEY_D           = 9,
    TOTAL_KEYS      = 10
} Key;

typedef enum KeyState
{
    KEY_UP   = 0,
    KEY_DOWN = 1
} KeyState;

typedef enum Frame
{
    FRAME_1_PRIOR = 0,
    FRAME_2_PRIOR = 1,
    FRAME_3_PRIOR = 2,
    FRAME_4_PRIOR = 3,
    FRAME_5_PRIOR = 4,
    FRAME_6_PRIOR = 5,
    FRAME_7_PRIOR = 6,
    FRAME_8_PRIOR = 7,
    TOTAL_FRAMES  = 8
} Frame;

typedef struct InputManager
{
    int inputs_on_frame[TOTAL_FRAMES][TOTAL_KEYS];
    Cursor cursor; 
    InputManager();
} InputManager;

#endif

