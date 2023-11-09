// ====================================================
// Title: input.cpp
// Description: The source file for input processing
// ====================================================

#include "input.hpp"

// Struct Cursor //

Cursor::Cursor()
{
    x_pos       = 0.0f;
    y_pos       = 0.0f;
    last_x      = 0.0f;
    last_y      = 0.0f;
    first_mouse = true;
}

Vec2F
cursorGetDistance(Cursor& cursor)
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

// Struct InputManager //

InputManager::InputManager()
{
    // Initialize all key input states to 0
    memset(inputs_on_frame, 0, sizeof(int) * TOTAL_FRAMES * TOTAL_KEYS);
}


