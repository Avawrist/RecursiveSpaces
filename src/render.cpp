// ==========================================================================
// Title: render.cpp
// Description: The source file for the window struct, built on the GLFW API
// ==========================================================================

#include "render.hpp"

///////////////////////
// Struct GameWindow //
///////////////////////

GameWindow::GameWindow(uint _width, uint _height, c_char* name)
{
    /////////////////////////////
    // Set window measurements //
    /////////////////////////////
    win_width   = _width;
    win_height  = _height;
    view_width  = (uint)(win_width * 1.0f);
    view_height = (uint)(win_height * 1.0f);
    x_center    = win_width * 0.5f;
    y_center    = win_height * 0.5f;
    win_ar      = (float)win_width / (float)win_height;

    /////////////////////
    // Set time fields //
    /////////////////////
    target_framerate = 0; 

    // Get monitor struct from GLFW
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if(monitor)
    {
	// Get vidmode struct from GLFW, and read the refresh rate from it
	const GLFWvidmode* vid_mode = glfwGetVideoMode(monitor);
	if(vid_mode)
	{
	    target_framerate = vid_mode->refreshRate;
	    char msg[256];
	    sprintf_s(msg, "Target framerate found: %i\n", target_framerate);
	    OutputDebugStringA(msg);
	}
    }
    if(target_framerate == 0) {target_framerate = 60;} // If framerate not found, set to 60 by default
    target_cycle_length_secs = 1.0f / target_framerate;  
    cycle_start_time_secs    = 0.0f;
    d_time                 = 0;

    ///////////////////////////////////
    // Set sleep granularity to 1 MS //
    ///////////////////////////////////
    sleep_is_granular = (timeBeginPeriod(1) == TIMERR_NOERROR);
    
    /////////////////////////////
    // Create window & context //
    /////////////////////////////
    window_p = glfwCreateWindow(win_width, win_height, name, NULL, NULL);
    outputGLFWError();
    if(window_p)
    {
	// Make new window the current context	
	glfwMakeContextCurrent(window_p);
	outputGLFWError();
	
	// Disable cursor
	glfwSetInputMode(window_p, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	outputGLFWError();
    }
    else
    {
	OutputDebugStringA("ERROR: GLFW failed to create window/context\n");
    }
}

GameWindow::~GameWindow()
{
    glfwDestroyWindow(window_p);
    glfwTerminate(); // This will stop all GLFW functions including timer and input 
}

void gameWindowSwapBuffers(GameWindow& game_window)
{
    double time_elapsed_this_cycle_secs = glfwGetTime() - game_window.cycle_start_time_secs;

    // While the cycle is early
    if(time_elapsed_this_cycle_secs < game_window.target_cycle_length_secs)
    {
	// Wait for the difference of time between the current cycle time and the target time
	if(game_window.sleep_is_granular)
	{
	    DWORD sleep_time_ms = (DWORD)(1000.0f * (game_window.target_cycle_length_secs - time_elapsed_this_cycle_secs));
	    Sleep(sleep_time_ms);
	    time_elapsed_this_cycle_secs = glfwGetTime() - game_window.cycle_start_time_secs;
	}
    }
    // TODO: Handle case where cycle takes LONGER than the target cycle length
    
    // Swap buffers
    glfwSwapBuffers(game_window.window_p);
    outputGLFWError();

    // Update delta time
    game_window.d_time = (float)time_elapsed_this_cycle_secs;

    char msg2[256];
    sprintf_s(msg2, "Delta time from previous frame: %f\n", game_window.d_time);
    OutputDebugStringA(msg2);

    // Reset counters
    game_window.cycle_start_time_secs = glfwGetTime();
}

void gameWindowClose(GameWindow& game_window)
{
    glfwSetWindowShouldClose(game_window.window_p, GLFW_TRUE);
}

//////////////////////
// Render Functions //
//////////////////////

int initGLFW()
{
    // Init GLFW library
    if(!glfwInit())
    {
	OutputDebugStringA("ERROR: Failed to initialize GLFW\n");
	return 0;
    }

    // Set context Hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // TO-DO: Remove on release
    glfwWindowHint(GLFW_SAMPLES, 0); // disable multisampling
    
    return 1;
}

int initOpenGL()
{
    // Load OpenGL procedures through GLAD
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
	OutputDebugStringA("ERROR: Glad failed to load the OpenGL functions/extensions\n");
	return 0;
    }

    // Set initial OpenGL state
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.71f, 0.89f, 1.0f);
    // Cull Faces
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    // Debug - TO-DO: Remove following lines for release build
    glEnable(GL_DEBUG_OUTPUT); 
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    
    return 1;
}

/////////////////////
// Debug Functions //
/////////////////////

void outputGLFWError()
{
    c_char* errorMsg;
    int errorCode = glfwGetError(&errorMsg);
    if(errorMsg)
    {
	char code_buf[256];
	sprintf_s(code_buf, "ERROR CODE: %i", errorCode);
	OutputDebugStringA(code_buf);
        OutputDebugStringA(errorMsg);
	OutputDebugStringA("\n");
    }
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                       GLsizei length, c_char *message, c_void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    OutputDebugStringA("---------------\n");
    OutputDebugStringA("Debug message:\n");

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             OutputDebugStringA("Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   OutputDebugStringA("Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: OutputDebugStringA("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     OutputDebugStringA("Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     OutputDebugStringA("Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           OutputDebugStringA("Source: Other"); break;
    } OutputDebugStringA("\n");

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               OutputDebugStringA("Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: OutputDebugStringA("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  OutputDebugStringA("Type: Undefined Behaviour"); break; 
        case GL_DEBUG_TYPE_PORTABILITY:         OutputDebugStringA("Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         OutputDebugStringA("Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              OutputDebugStringA("Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          OutputDebugStringA("Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           OutputDebugStringA("Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               OutputDebugStringA("Type: Other"); break;
    } OutputDebugStringA("\n");
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         OutputDebugStringA("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       OutputDebugStringA("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          OutputDebugStringA("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: OutputDebugStringA("Severity: notification"); break;
    } OutputDebugStringA("\n");
    OutputDebugStringA("\n");
}
