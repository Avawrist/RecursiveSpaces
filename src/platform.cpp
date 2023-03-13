// ====================================================================================
// Title: platform.cpp
// Description: The source file for the platform layer, abstracting the
//              platform dependent APIs
// ====================================================================================

#include "platform.hpp"

int platformInitAPIs(GameWindow& game_window)
{
    // Returns 1 on success, 0 on failure
    
    // Initialize GLFW (Enumerates windows, joysticks, starts the timer)
    if(!platformInitGLFW()) {return 0;}

    // Initialize window
    if(!platformInitWindow(game_window, 1920, 1080, "First Game")) {return 0;}

    // Load OpenGL Functions & Extensions (Must be called after window creation)
    if(!platformInitOpenGL()) {return 0;}

    return 1;
}

int platformInitWindow(GameWindow& game_window, uint _width, uint _height, c_char* name)
{
    // Returns 1 on success, 0 on failure
    
    /////////////////
    // Init Values //
    /////////////////
    // Set window measurements
    game_window.win_width   = _width;
    game_window.win_height  = _height;
    game_window.view_width  = (uint)(game_window.win_width * 1.0f);
    game_window.view_height = (uint)(game_window.win_height * 1.0f);
    game_window.x_center    = game_window.win_width * 0.5f;
    game_window.y_center    = game_window.win_height * 0.5f;
    game_window.win_ar      = (float)game_window.win_width / (float)game_window.win_height;
    // Set close condition
    game_window.close = false;
    // Set time fields
    game_window.target_framerate = 0;

    /////////////////////////////
    // Get framerate from GLFW //
    /////////////////////////////
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if(monitor)
    {
	// Get vidmode struct from GLFW, and read the refresh rate from it
	const GLFWvidmode* vid_mode = glfwGetVideoMode(monitor);
	if(vid_mode)
	{
	    game_window.target_framerate = vid_mode->refreshRate;
	    char msg[256];
	    sprintf_s(msg, "Target framerate found: %i\n", game_window.target_framerate);
	    OutputDebugStringA(msg);
	}
    }
    // If framerate not found, set to 60 by default
    if(game_window.target_framerate == 0) {game_window.target_framerate = 60;} 
    game_window.target_cycle_length_secs = 1.0f / game_window.target_framerate;  
    game_window.cycle_start_time_secs    = 0.0f;
    game_window.d_time                   = 0;

    ///////////////////////////////////
    // Set sleep granularity to 1 MS //
    ///////////////////////////////////
    game_window.sleep_is_granular = (timeBeginPeriod(1) == TIMERR_NOERROR);
    
    /////////////////////////////
    // Create window & context //
    /////////////////////////////
    game_window.window_p = (void*)glfwCreateWindow(game_window.win_width, game_window.win_height,
						   name, NULL, NULL);
    outputGLFWError();
    if(game_window.window_p)
    {
	// Make new window the current context	
	glfwMakeContextCurrent((GLFWwindow*)game_window.window_p);
	outputGLFWError();
	
	// Disable cursor
	glfwSetInputMode((GLFWwindow*)game_window.window_p, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	outputGLFWError();

	return 1;
    }
    else
    {
	OutputDebugStringA("ERROR: GLFW failed to create window/context\n");
	return 0;
    }
}

void platformFreeWindow(GameWindow& game_window)
{
    glfwDestroyWindow((GLFWwindow*)game_window.window_p);
    glfwTerminate(); // This will stop all GLFW functions including timer and input
}

void platformSwapBuffers(GameWindow& game_window)
{
    double time_elapsed_this_cycle_secs = glfwGetTime() - game_window.cycle_start_time_secs;

    // While the cycle is early
    if(time_elapsed_this_cycle_secs < game_window.target_cycle_length_secs)
    {
	// Wait for the difference of time between the current cycle time and the target time
	if(game_window.sleep_is_granular)
	{
	    DWORD sleep_time_ms = (DWORD)(1000.0f * (game_window.target_cycle_length_secs -
						     time_elapsed_this_cycle_secs));
	    Sleep(sleep_time_ms);
	    time_elapsed_this_cycle_secs = glfwGetTime() - game_window.cycle_start_time_secs;
	}
    }
    // TODO: Handle case where cycle takes LONGER than the target cycle length
    
    // Swap buffers
    glfwSwapBuffers((GLFWwindow*)game_window.window_p);
    outputGLFWError();

    // Update delta time
    game_window.d_time = (float)time_elapsed_this_cycle_secs;

    char msg2[256];
    sprintf_s(msg2, "Delta time from previous frame: %f\n", game_window.d_time);
    OutputDebugStringA(msg2);

    // Reset counters
    game_window.cycle_start_time_secs = glfwGetTime();
}

void platformGetInputsThisFrame(InputManager &im, GameWindow &gw)
{
    glfwPollEvents(); // Processes all input events that occurred this cycle
    
    // Populate all inputs received this cycle into inputs_on_frame[1_FRAME_PRIOR]
    // Will be processed in the game layer next cycle
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_UP]    = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_UP);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_DOWN]  = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_DOWN);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_LEFT]  = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_LEFT);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_RIGHT] = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_RIGHT);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_SPACE]       = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_SPACE);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ESC]         = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_ESCAPE);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_W]           = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_W);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_S]           = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_S);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_A]           = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_A);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_D]           = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_D);

    // Get cursor input
    glfwGetCursorPos((GLFWwindow*)gw.window_p, &im.cursor.x_pos, &im.cursor.y_pos);
}

////////////////////////
// API Init Functions //
////////////////////////

int platformInitGLFW()
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

int platformInitOpenGL()
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
