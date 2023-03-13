// ====================================================
// Title: input.cpp
// Description: The source file for input processing
// ====================================================

#include "input.hpp"

///////////////////
// Struct Cursor //
///////////////////

Cursor::Cursor()
{
    first_mouse = true;
    x_pos       = 0.0f;
    y_pos       = 0.0f;
    last_x      = 0.0f;
    last_y      = 0.0f;
}

Cursor::Cursor(float _last_x, float _last_y)
{
    first_mouse = true;
    x_pos       = 0.0f;
    y_pos       = 0.0f;
    last_x      = _last_x;
    last_y      = _last_y;
}

Vec2F cursorGetDistance(Cursor& cursor)
{
    float x_dist = (float)(cursor.x_pos - cursor.last_x);
    float y_dist = (float)(cursor.y_pos - cursor.last_y);
    cursor.last_x = (float)cursor.x_pos;
    cursor.last_y = (float)cursor.y_pos;

    if(cursor.first_mouse)
    {
        x_dist = 0.0f;
        y_dist = 0.0f;
	cursor.first_mouse = false;
    }

    return Vec2F(x_dist, y_dist);
}

/////////////////////////
// Struct InputManager //
/////////////////////////

InputManager::InputManager(GameWindow& game_window)
{
    // Initialize all key input states to 0
    memset(inputs_on_frame, 0, sizeof(int) * TOTAL_FRAMES * TOTAL_KEYS);

    // Initialize cursor
    cursor.last_x = game_window.x_center;
    cursor.last_y = game_window.y_center;
}


