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

using namespace std;

void
odGLFWError();

void
printGLFWError();

void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int
main()
{
    // Initialize GLFW (Enumerates windows, joysticks, starts the timer)
    if(glfwInit())
    {
	// Get any errors from initialization
        odGLFWError();
        
	// Set Context Hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create window & context
	GLFWwindow* window = glfwCreateWindow(640, 480, "First Game", NULL, NULL);
	odGLFWError();

	if(window)
	{
	    // Set keyboard input callback
	    glfwSetKeyCallback(window, keyCallback);
	    
	    // Prep test render data

	    // Render loop
	    while(!glfwWindowShouldClose(window))
	    {
		// Render pass 1

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	    }

	    // Delete window
	    glfwDestroyWindow(window);
	}
	else
	{
	    OutputDebugStringA("ERROR: GLFW failed to create window/context\n");
	}
	
	// Terminate GLFW on program exit:
	glfwTerminate();
    }
    else
    {
	OutputDebugStringA("ERROR: Failed to initialize GLFW\n");
    }
    
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
