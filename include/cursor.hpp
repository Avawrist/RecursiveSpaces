// ======================================================================
// Title: cursor.hpp
// Description: The header file for cursor objects
// ======================================================================

#ifndef CURSOR_H
#define CURSOR_H

// 3rd Party libs
#include <GLFW/glfw3.h>

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"
#include "mdcla.hpp"

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
