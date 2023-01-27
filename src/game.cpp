// ====================================================================================
// Title: game.cpp
// Description: The primary source file for an untitled game, containing the game loop.
// ====================================================================================

// Win libs
#include <windows.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <comdef.h>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// My libs
#include "typedefs.hpp"
#include "mdcla.hpp"
#include "shader.hpp"
#include "cursor.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "asset.hpp"
#include "light.hpp"
#include "sound.hpp"

using namespace std;

/////////////
// Globals //
/////////////

// Window
c_uint WIN_WIDTH   = 1024;
c_uint WIN_HEIGHT  = 576;
c_uint VIEW_WIDTH  = WIN_WIDTH * 0.25f;
c_uint VIEW_HEIGHT = WIN_HEIGHT * 0.25f;
c_uint X_CENTER    = WIN_WIDTH * 0.5f;
c_uint Y_CENTER    = WIN_HEIGHT * 0.5f;
float  win_ar      = (float)WIN_WIDTH / (float)WIN_HEIGHT;

// Time
int   frame_rate  = 60;
float prev_time   = 0.0f;
float d_time      = 0.0f;

// Cursor
Cursor global_cursor(X_CENTER, Y_CENTER);

// Camera
Camera global_cam(Vec3F(0.0f, 2.0f, 4.0f), 0.8f, 100.0f, 45.0f, win_ar);

/////////////////////////
// Function Prototypes //
/////////////////////////

void odGLFWError();

void printGLFWError();

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                       GLsizei length, const char *message, const void *userParam);

//////////////////////////
// Function Definitions //
//////////////////////////

int main()
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
    glfwWindowHint(GLFW_SAMPLES, 0); // disable multisampling
	
    // Create window & context
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "First Game", NULL, NULL);
    if(!window)
    {
	OutputDebugStringA("ERROR: GLFW failed to create window/context\n");
	return -1;
    }
    odGLFWError();

    //////////////////////////////
    // Set GLFW event callbacks //
    //////////////////////////////
    
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
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.71f, 0.89f, 1.0f);
    // Cull Faces
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    // Debug - TO-DO: Remove following lines for release build
    glEnable(GL_DEBUG_OUTPUT); 
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    //////////////////////////
    // Load XAudio2 Library //
    //////////////////////////

    SoundInterface soundInterface;
    Sound *test_sound_p   = new Sound("..\\assets\\sfx\\elephant.wav", soundInterface);
    Sound *test_sound_2_p = new Sound("..\\assets\\sfx\\taunt.wav", soundInterface);
    
    //////////////////////////
    // Initialize Test Mesh //
    //////////////////////////

    Mesh *mesh_p = new Mesh("..\\assets\\meshes\\chest.obj");
    meshDataToGPU(mesh_p);

    /////////////////////////
    // Initialize Textures //
    /////////////////////////

    Texture *d_texture_p = new Texture("..\\assets\\textures\\chest.bmp");
    textureDataToGPU(d_texture_p);

    Texture *n_texture_p = new Texture("..\\assets\\textures\\brickwall_normal.bmp");
    textureDataToGPU(n_texture_p);

    ///////////////////////
    // Initialize Lights //
    ///////////////////////

    DirLight *dirLight_p = new DirLight(Vec3F(1.0f, 1.0f, 1.0f),
					Vec3F(0.0f, -1.0f, -1.0f),
	                                0.25f);

    /////////////////////////////
    // Initialize Framebuffers //
    /////////////////////////////
    
    // Texture size should match the viewport.
    //We will then blow it up to fit the screen for a low res look.
    FrameTexture *ftexture_p = new FrameTexture(VIEW_WIDTH, VIEW_HEIGHT);
    frameTextureDataToGPU(ftexture_p);
    
    /////////////////////////////
    // Transformation Matrices //
    /////////////////////////////

    // Model Matrix (Local space -> world space)
    Mat4F model(1.0f);
    // View Matrix (World space -> view space)
    Mat4F view = cameraGetView(global_cam);
    // Projection Matrix (View space -> clip space/NDC)
    Mat4F projection = cameraGetPerspective(global_cam);
    
    ////////////////////
    // Create Shaders //
    ////////////////////

    // Simple object shader
    Shader *bp_shader_p = new Shader("..\\assets\\shaders\\blinn_phong.vert",
					"..\\assets\\shaders\\blinn_phong.frag");

    // Load initial uniform values to GPU
    glUseProgram(bp_shader_p->program_id);
    shaderAddMat4Uniform(bp_shader_p, "model",      model.getPointer());
    shaderAddMat4Uniform(bp_shader_p, "view",       view.getPointer());
    shaderAddMat4Uniform(bp_shader_p, "projection", projection.getPointer());
    shaderAddIntUniform(bp_shader_p, "diffuse_map", 0);
    shaderAddIntUniform(bp_shader_p, "normal_map", 1);
    
    // Post-processing shader
    Shader *pp_shader_p = new Shader("..\\assets\\shaders\\pp.vert",
				     "..\\assets\\shaders\\pp.frag");
    // Load initial uniform values to GPU
    glUseProgram(pp_shader_p->program_id);
    shaderAddIntUniform(pp_shader_p, "color_texture", 0);
    
    
    /////////////////
    // Render Loop //
    /////////////////

    while(!glfwWindowShouldClose(window))
    {	
	///////////////////////
	// Update delta time //
	///////////////////////
	float curr_time = glfwGetTime();
	d_time    = curr_time - prev_time;
	prev_time = curr_time;
	
	///////////////////
	// Update cursor //
	///////////////////
	cursorUpdate(global_cursor, window);

	/////////////////////////
	// Update Sound Output //
	/////////////////////////

	//TODO: Implement XAudio2 audio graph
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
	    soundPlay(test_sound_p);
	}
	
	///////////////////
	// Update camera //
	///////////////////
	cameraUpdate(global_cam, window, cursorGetDistance(global_cursor), d_time);
	
	////////////////////////////
	// Update Shader Uniforms //
	////////////////////////////

	glUseProgram(bp_shader_p->program_id);
	
	// DirLights //
	shaderAddVec3Uniform(bp_shader_p,  "dirLight.color", dirLight_p->color);
	shaderAddVec3Uniform(bp_shader_p,  "dirLight.dir",   dirLight_p->dir);
	shaderAddFloatUniform(bp_shader_p, "dirLight.ambient_strength", dirLight_p->ambient_strength);

	// Cam Position //
	shaderAddVec3Uniform(bp_shader_p, "cam_pos", global_cam.pos);
	
	// Transformation Matrices // 
     
	// Update View
	view = cameraGetView(global_cam);
	shaderAddMat4Uniform(bp_shader_p, "view", view.getPointer());
	
	// Update perspective matrix with potential new AR
	// (TO-DO: this is expensive, only calculate new projection mat if ar changes )
	projection = cameraGetPerspective(global_cam);
	shaderAddMat4Uniform(bp_shader_p, "projection", projection.getPointer());

	/////////////////////////
	// ** Render pass 1 ** //
	/////////////////////////

	// Use this render pass to write pre-processed image to the color texture.
	glViewport(0, 0, VIEW_WIDTH, VIEW_HEIGHT); // Render to a smaller area first
	                                           // so we can blow it up and lower the res
	glBindFramebuffer(GL_FRAMEBUFFER, ftexture_p->fbo); // bind the fbo with color texture
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(bp_shader_p->program_id); // Use the basic object shader
	// Move into gameobject draw code eventually:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d_texture_p->texture_id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, n_texture_p->texture_id);
	
	glBindVertexArray(mesh_p->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh_p->data.size());

	///////////////////////////////////////////
	// ** Render pass 2 (Post-processing) ** //
	///////////////////////////////////////////

	// use this render pass to draw post-processed color texture to screen.
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); // Render to the full screen now,
	                                         // blowing up the screen texture
        frameTextureDraw(ftexture_p, pp_shader_p);
	
	//////////////////
	// Swap buffers //
	//////////////////
	glfwSwapBuffers(window);
	glfwPollEvents();
	odGLFWError();

	/////////////////////
	// Close condition //
	/////////////////////
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwSetWindowShouldClose(window, GLFW_TRUE);}
    }

    /////////////
    // Cleanup //
    /////////////

    // Stop all sounds
    soundStop(test_sound_p);
    soundStop(test_sound_2_p);
    
    // Delete pointers to structs on the heap
    delete test_sound_p;
    delete mesh_p;
    delete d_texture_p;
    delete n_texture_p;
    delete bp_shader_p;
    delete pp_shader_p;
    delete ftexture_p;
    delete dirLight_p;
    
    // Delete window
    glfwDestroyWindow(window);
	
    // Terminate GLFW on program exit:
    glfwTerminate();
    
    return 0;
}

void odGLFWError()
{
    c_char* errorMsg;
    int errorCode = glfwGetError(&errorMsg);
    if(errorMsg)
    {
        OutputDebugStringA(errorMsg);
	OutputDebugStringA("\n");
    }
}

void printGLFWError()
{
    c_char* errorMsg;
    int errorCode = glfwGetError(&errorMsg);
    if(errorMsg)
    {
	cout << "GLFW Error: " << errorMsg << "ID: " << errorCode << endl;
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
