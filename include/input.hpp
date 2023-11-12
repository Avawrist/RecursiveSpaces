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

// Struct Cursor //

typedef struct Cursor
{
    double x_pos;
    double y_pos;
    float  last_x;
    float  last_y;
    bool   first_mouse;
    Cursor();
} Cursor;

Vec2F
cursorGetDistance(Cursor& cursor);

// Struct InputManager //

typedef enum Key
{
    KEY_ARROW_UP    = 0,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,
    KEY_SPACE,
    KEY_ESC,
    KEY_W,
    KEY_S,
    KEY_A,
    KEY_D,
    KEY_I,
    KEY_O,
    TOTAL_KEYS
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

