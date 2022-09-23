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

using namespace std;

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
    glEnable(GL_CULL_FACE);
    // TO-DO: Remove following lines for release build
    glEnable(GL_DEBUG_OUTPUT); 
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    // END TO-DO
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    ////////////////////////////
    // Initialize Render Data //
    ////////////////////////////
    
    // Vertex data
    float testVertices[] = {
	// Positions
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
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
    
    ////////////////////
    // Create Shaders //
    ////////////////////

    Shader basicShaderProgram("F:\\assets\\shaders\\basic.vs", "F:\\assets\\shaders\\basic.fs"); // TO-DO: Make generic path

    ////////////////
    // Math Tests //
    ////////////////
    
    /////////////////
    // Render Loop //
    /////////////////

    while(!glfwWindowShouldClose(window))
    {
	// Clear screen
        glClear(GL_COLOR_BUFFER_BIT);
	
	// Render pass 1
	glUseProgram(basicShaderProgram.program_id);
	glBindVertexArray(testVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	// Swap buffers
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
}

void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void
mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Test passed
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
