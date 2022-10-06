// ======================================================================
// Title: renderer.cpp
// Description: The primary source file for an untitled renderer.
// ======================================================================

// Win libs
#include <windows.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// My libs
#include <mdcla.hpp>
#include <shader.hpp>
#include <camera.hpp>

using namespace std;

/////////////
// Globals //
/////////////

// Window
const unsigned int WIN_WIDTH  = 640;
const unsigned int WIN_HEIGHT = 480;
const unsigned int X_CENTER = WIN_WIDTH / 2.0f;
const unsigned int Y_CENTER = WIN_HEIGHT / 2.0f;
double last_x = X_CENTER;
double last_y = Y_CENTER;
float WIN_AR = (float)WIN_WIDTH / (float)WIN_HEIGHT;

// Time
float prevTime   = 0.0f;
float dTime      = 0.0f;
float FRAME_RATE = 30.0f;

// Camera
Camera globalCam(Vec3F(0.0f, 0.0f, 3.0f),
                 Quaternion(1.0f, 0.0f, 0.0f, 0.0f));

///////////////
// Functions //
///////////////

void
odGLFWError();

void
printGLFWError();

void
framebufferSizeCallback(GLFWwindow* window, int width, int height);

void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void
mousePosCallback(GLFWwindow* window, double xpos, double ypos);

void
mouseBtnCallback(GLFWwindow* window, int button, int action, int mods);

void
mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void
APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                       GLsizei length, const char *message, const void *userParam);

int
main()
{
    //////////////////////
    // Window & Context //
    //////////////////////
    
    // Initialize GLFW (Enumerates windows, joysticks, starts the timer)
    if(!glfwInit())
    {
	OutputDebugStringA("ERROR: Failed to initialize GLFW\n");
	return -1;
    }
    odGLFWError();
        
    // Set Context Hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // TO-DO: Remove on release
    glfwWindowHint(GLFW_SAMPLES, 4); // multisame buffer with 4 samples
	
    // Create window & context
    GLFWwindow* window = glfwCreateWindow(640, 480, "First Game", NULL, NULL);
    if(!window)
    {
	OutputDebugStringA("ERROR: GLFW failed to create window/context\n");
	return -1;
    }
    odGLFWError();

    //////////////////////////////
    // Set GLFW event callbacks //
    //////////////////////////////
    
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetMouseButtonCallback(window, mouseBtnCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    odGLFWError();
    
    // Make created window/context the current context
    glfwMakeContextCurrent(window);
    odGLFWError();

    ////////////////////////////////////////
    // Load OpenGL Functions & Extensions //
    ////////////////////////////////////////
    
    // Load OpenGL functions with glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
	OutputDebugStringA("ERROR: Glad failed to load the OpenGL functions/extensions\n");
	return -1;
    }

    ////////////////////////////
    // Configure OpenGL State //
    ////////////////////////////
    
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Cull Faces
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    // Debug - TO-DO: Remove following lines for release build
    glEnable(GL_DEBUG_OUTPUT); 
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    
    ////////////////////////////
    // Initialize Render Data //
    ////////////////////////////
    
    // Vertex data
    float testVertices[] = {
	// Positions
	-0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,           
	0.5f,  0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f,  
	-0.5f,  0.5f, -0.5f,  
	// Front face
	-0.5f, -0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f,  
	// Left face
	-0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f,  
	-0.5f, -0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f,  
	// Right face
	0.5f,  0.5f,  0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f,  0.5f, -0.5f,           
	0.5f, -0.5f, -0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,       
	// Bottom face
	-0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f, -0.5f,  
	// Top face
	-0.5f,  0.5f, -0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f, -0.5f,       
	0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f, -0.5f,  
	-0.5f,  0.5f,  0.5f
    };

    // VBOs
    GLuint testVBO;
    glGenBuffers(1, &testVBO);
    
    // VAOs
    GLuint testVAO;
    glGenVertexArrays(1, &testVAO);

    glBindVertexArray(testVAO); // Bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, testVBO); // Bind VBO while VAO is bound
    glBufferData(GL_ARRAY_BUFFER, sizeof(testVertices), testVertices, GL_STATIC_DRAW); // Copy array to buffer
    // Tell OpenGL how the vertex array is divided into attribute arrays: 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Enable the first attribute array (position)
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer (it is tied to the vertex array object now)
    glBindVertexArray(0); // Unbind until we are ready to draw
    
    /////////////////////////////
    // Transformation Matrices //
    /////////////////////////////

    // Model Matrix (Local space -> world space)
    Mat4F model(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
    
    // View Matrix (World space -> view space)
    Mat4F view = getView(globalCam);
    
    // Projection Matrix (View space -> clip space/NDC)
    Mat4F projection = getPerspectiveMat(degToRads(45.0f), WIN_AR, 1.0f, 100.0f);
    
    ////////////////////
    // Create Shaders //
    ////////////////////

    // TO-DO: Make generic path
    Shader basicShaderProgram("F:\\assets\\shaders\\basic.vs",
			      "F:\\assets\\shaders\\basic.fs");

    // Load uniform values to GPU
    glUseProgram(basicShaderProgram.program_id);
    basicShaderProgram.addMat4Uniform("model", model.getPointer());
    basicShaderProgram.addMat4Uniform("view", view.getPointer());
    basicShaderProgram.addMat4Uniform("projection", projection.getPointer());
    
    /////////////////
    // Render Loop //
    /////////////////

    while(!glfwWindowShouldClose(window))
    {
	// Update delta time
	float currTime = glfwGetTime();
	dTime = currTime - prevTime;
	prevTime = currTime;

	////////////////////////////////////
	// Update Transformation Matrices //
	////////////////////////////////////
	
	glUseProgram(basicShaderProgram.program_id);

	// Update View
	view = getView(globalCam);
	basicShaderProgram.addMat4Uniform("view", view.getPointer());
	
	// Update perspective matrix with potential new AR
	// (TO-DO: this is expensive, only calculate new projection mat if ar changes )
        projection = getPerspectiveMat(degToRads(45.0f), WIN_AR, 1.0f, 100.0f);
	basicShaderProgram.addMat4Uniform("projection", projection.getPointer());

	//////////////////
	// Clear screen //
	//////////////////
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	///////////////////
	// Render pass 1 //
	///////////////////
	glUseProgram(basicShaderProgram.program_id);
	glBindVertexArray(testVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//////////////////
	// Swap buffers //
	//////////////////
	glfwSwapBuffers(window);
	glfwPollEvents();
	odGLFWError();
    }

    /////////////
    // Cleanup //
    /////////////

    // Delete Vertex Array Objects
    glDeleteVertexArrays(1, &testVAO);
    
    // Delete buffers
    glDeleteBuffers(1, &testVBO);
    
    // Delete window
    glfwDestroyWindow(window);
	
    // Terminate GLFW on program exit:
    glfwTerminate();
    
    return 0;
}

void
odGLFWError()
{
    const char* errorMsg;
    int errorCode = glfwGetError(&errorMsg);
    if(errorMsg)
    {
        OutputDebugStringA(errorMsg);
	OutputDebugStringA("\n");
    }
}

void
printGLFWError()
{
    const char* errorMsg;
    int errorCode = glfwGetError(&errorMsg);
    if(errorMsg)
    {
	cout << "GLFW Error: " << errorMsg << "ID: " << errorCode << endl;
    }
}

void
framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    WIN_AR = (float)width / (float)height; // Update the global
}

void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    Mat3F camRot   = quatToMat3(globalCam.orientation);
    float dTimeSpd = dTime * globalCam.move_speed;
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
	globalCam.pos -= (dTimeSpd * normalize(Vec3F(camRot(0, 2), camRot(1, 2), camRot(2, 2))));
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
	globalCam.pos += (dTimeSpd * normalize(Vec3F(camRot(0, 2), camRot(1, 2), camRot(2, 2))));
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
	globalCam.pos += (dTimeSpd * normalize(Vec3F(camRot(0, 0), camRot(1, 0), camRot(2, 0))));
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
	globalCam.pos -= (dTimeSpd * normalize(Vec3F(camRot(0, 0), camRot(1, 0), camRot(2, 0))));
    }
}

void
mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
    const double X_DIST = xpos - last_x;
    const double Y_DIST = ypos - last_y;
    last_x = xpos;
    last_y = ypos;

    Vec2F distance = normalize(Vec2F((float)X_DIST, (float)Y_DIST));
     updateRotFromMouse(globalCam, distance);
}

void
mouseBtnCallback(GLFWwindow* window, int button, int action, int mods)
{
    // Test passed
}

void
mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Not tested
}

void
APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                       GLsizei length, const char *message, const void *userParam)
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
