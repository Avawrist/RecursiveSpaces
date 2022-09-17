// ======================================================================
// Title: renderer.cpp
// Description: The primary source file for an untitled renderer.
// Last Modified: 9/6/2022 MDC
// ======================================================================

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <windows.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include <shader.h>

using namespace std;

void
odGLFWError();

void
printGLFWError();

void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void
mousePosCallback(GLFWwindow* window, double xpos, double ypos);

void
mouseBtnCallback(GLFWwindow* window, int button, int action, int mods);

void
mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
    // Create window & context
    GLFWwindow* window = glfwCreateWindow(640, 480, "First Game", NULL, NULL);
    if(!window)
    {
	OutputDebugStringA("ERROR: GLFW failed to create window/context\n");
	return -1;
    }
    odGLFWError();

    // Set input callbacks
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
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    
    ////////////////////////////
    // Initialize Render Data //
    ////////////////////////////
    
    // Vertex data
    float testVertices[] = {
	// Positions
	1.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); 
    glEnableVertexAttribArray(0); // Enable the first attribute array (position)
    glBindVertexArray(0); // Unbind until we are ready to draw
    
    ////////////////////
    // Create Shaders //
    ////////////////////

    Shader myShader(GL_VERTEX_SHADER);
    
    /////////////////
    // Render Loop //
    /////////////////

    while(!glfwWindowShouldClose(window))
    {
	// Render pass 1
	
		
	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
	odGLFWError();
    }

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

