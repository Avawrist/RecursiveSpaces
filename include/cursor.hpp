// ======================================================================
// Title: cursor.hpp
// Description: The header file for cursor objects
// ======================================================================

#ifndef CURSOR_H
#define CURSOR_H

#include <mdcla.hpp>
#include <GLFW/glfw3.h>

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

#endif 
