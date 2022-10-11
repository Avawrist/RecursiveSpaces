// ======================================================================
// Title: cursor.cpp
// Description: The source file for cursor objects
// ======================================================================

#include <cursor.hpp>

Cursor::Cursor()
{
    first_mouse = true;
    x_pos        = 0.0f;
    y_pos        = 0.0f;
    last_x      = 0.0f;
    last_y      = 0.0f;
}

Cursor::Cursor(float _last_x, float _last_y)
{
    first_mouse = true;
    x_pos        = 0.0f;
    y_pos       = 0.0f;
    last_x      = _last_x;
    last_y      = _last_y;
}

void cursorUpdate(Cursor& cursor, GLFWwindow* window)
{
    glfwGetCursorPos(window, &cursor.x_pos, &cursor.y_pos);
}

Vec2F cursorGetDistance(Cursor& cursor)
{
    float x_dist = cursor.x_pos - cursor.last_x;
    float y_dist = cursor.y_pos - cursor.last_y;
    cursor.last_x = cursor.x_pos;
    cursor.last_y = cursor.y_pos;

    if(cursor.first_mouse)
    {
        x_dist = 0.0f;
        y_dist = 0.0f;
	cursor.first_mouse = false;
    }

    return Vec2F(x_dist, y_dist);
}
