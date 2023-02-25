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

InputManager::InputManager()
{
    memset(inputs_on_frame, -1, sizeof(int) * TOTAL_FRAMES * TOTAL_KEYS);
}
  
void inputManagerGetInputsThisFrame(InputManager& im, GameWindow& game_window)
{
    // Copy frame inputs to the prior frame inputs to make room for new inputs
    for(int i = TOTAL_FRAMES; i > 1; i--)
    {
	im.inputs_on_frame[i - 1][KEY_ARROW_UP]    = im.inputs_on_frame[i - 2][KEY_ARROW_UP];
	im.inputs_on_frame[i - 1][KEY_ARROW_DOWN]  = im.inputs_on_frame[i - 2][KEY_ARROW_DOWN];
	im.inputs_on_frame[i - 1][KEY_ARROW_LEFT]  = im.inputs_on_frame[i - 2][KEY_ARROW_LEFT];
	im.inputs_on_frame[i - 1][KEY_ARROW_RIGHT] = im.inputs_on_frame[i - 2][KEY_ARROW_RIGHT];
	im.inputs_on_frame[i - 1][KEY_SPACE]       = im.inputs_on_frame[i - 2][KEY_SPACE];
    }
    
    glfwPollEvents(); // Processes all input events that occurred this cycle
    
    // Populate all inputs received this cycle into inputs_on_frame[1_FRAME_PRIOR]
    // Will be processed next cycle
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_UP]    = glfwGetKey(game_window.window_p, GLFW_KEY_UP);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_DOWN]  = glfwGetKey(game_window.window_p, GLFW_KEY_DOWN);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_LEFT]  = glfwGetKey(game_window.window_p, GLFW_KEY_LEFT);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_RIGHT] = glfwGetKey(game_window.window_p, GLFW_KEY_RIGHT);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_SPACE]       = glfwGetKey(game_window.window_p, GLFW_KEY_SPACE);	
}

