// ====================================================================================
// Title: game.cpp
// Description: The primary source file for an untitled game, containing the game loop.
// ====================================================================================

// Win libs
#include <windows.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <comdef.h>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"
#include "mdcla.hpp"
#include "shader.hpp"
#include "cursor.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "asset.hpp"
#include "light.hpp"
#include "sound.hpp"
#include "asset_manager.hpp"
#include "render.hpp"
#include "input.hpp"

using namespace std;

/////////////
// Globals //
/////////////

// Window
c_uint WIN_WIDTH   = 1024;
c_uint WIN_HEIGHT  = 576;
c_uint VIEW_WIDTH  = (c_uint)(WIN_WIDTH * 0.25f);
c_uint VIEW_HEIGHT = (c_uint)(WIN_HEIGHT * 0.25f);
c_uint X_CENTER    = (c_uint)(WIN_WIDTH * 0.5f);
c_uint Y_CENTER    = (c_uint)(WIN_HEIGHT * 0.5f);
float  win_ar      = (float)WIN_WIDTH / (float)WIN_HEIGHT;

// Time
int   frame_rate  = 60;
float prev_time   = 0.0f;
float d_time      = 0.0f;

// Cursor
Cursor global_cursor(X_CENTER, Y_CENTER);

// Camera
Camera global_cam(Vec3F(0.0f, 2.0f, 4.0f), 0.8f, 100.0f, 45.0f, win_ar);

// BGM Volume
int master_bgm_volume = 50;

//////////////////////////
// Function Definitions //
//////////////////////////

int main()
{
    /////////////////////////
    // Init Render & Input //
    /////////////////////////
    
    // Initialize GLFW (Enumerates windows, joysticks, starts the timer)
    if(!initGLFW()) {return -1;}

    // Get Game Window
    GameWindow game_window(WIN_WIDTH, WIN_HEIGHT, "First Game"); // GLFW terminates on deletion 
    if(!game_window.window_p) {return -1;}
    
    // Get Input Manager
    // ...
    
    // Load OpenGL Functions & Extensions (Must be called after window creation)
    if(!initOpenGL()) {return -1;}

    //////////////////////////
    // Load XAudio2 Library //
    //////////////////////////
    SoundInterface sound_interface;

    /////////////////////////
    // Prep asset managers //
    /////////////////////////
    AssetManager  asset_manager;
    ShaderManager shader_manager;

    ////////////////////
    // Initialize SFX //
    ////////////////////
    Sound* test_sound_p = (Sound*)assetManagerGetAssetP(asset_manager, TEST, SOUND01, &sound_interface); 
    
    ////////////////////
    // Initialize BGM //
    ////////////////////
    SoundStream *test_soundStream_p = new SoundStream("..\\assets\\bgm\\elephant.wav", sound_interface);
    
    //////////////////////////
    // Initialize Test Mesh //
    //////////////////////////
    Mesh *mesh_p = (Mesh*)assetManagerGetAssetP(asset_manager, CHEST, MESH01, 0);

    /////////////////////////
    // Initialize Textures //
    /////////////////////////
    Texture* d_texture_p = (Texture*)assetManagerGetAssetP(asset_manager, CHEST, TEXTURE_D, 0);
    Texture* n_texture_p = (Texture*)assetManagerGetAssetP(asset_manager, TEST, TEXTURE_N, 0);

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

    // Blinn-phong shader
    Shader *bp_shader_p = (Shader*)shaderManagerGetShaderP(shader_manager, BLINNPHONG);
    
    // Load initial uniform values to GPU
    glUseProgram(bp_shader_p->program_id);
    shaderAddMat4Uniform(bp_shader_p, "model",      model.getPointer());
    shaderAddMat4Uniform(bp_shader_p, "view",       view.getPointer());
    shaderAddMat4Uniform(bp_shader_p, "projection", projection.getPointer());
    shaderAddIntUniform(bp_shader_p, "diffuse_map", 0);
    shaderAddIntUniform(bp_shader_p, "normal_map",  1);
    
    // Post-processing shader
    Shader *pp_shader_p = (Shader*)shaderManagerGetShaderP(shader_manager, POSTPROCESS);
	
    // Load initial uniform values to GPU
    glUseProgram(pp_shader_p->program_id);
    shaderAddIntUniform(pp_shader_p, "color_texture", 0);

    ///////////////
    // Game Loop //
    ///////////////
    while(!glfwWindowShouldClose(game_window.window_p))
    {
	///////////////////////
	// Update delta time //
	///////////////////////
	float curr_time = (float)glfwGetTime();
	d_time    = curr_time - prev_time;
	prev_time = curr_time;

	///////////////////
	// Update cursor //
	///////////////////
	cursorUpdate(global_cursor, game_window.window_p);

	///////////////////
	// Update Volume //
	///////////////////

	if(glfwGetKey(game_window.window_p, GLFW_KEY_UP) == GLFW_PRESS)
	{
	    master_bgm_volume += 1;
	}
	if(glfwGetKey(game_window.window_p, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
	    master_bgm_volume -= 1;
	}
	soundSetVolume(test_sound_p, master_bgm_volume);
	
	//////////////////
	// Update sound //
	//////////////////

	if(glfwGetKey(game_window.window_p, GLFW_KEY_Q) == GLFW_PRESS)
	{
	    //soundStreamPlay(test_soundStream_p);
	    soundPlay(test_sound_p);
	}
	if(glfwGetKey(game_window.window_p, GLFW_KEY_W) == GLFW_PRESS)
	{
	    //soundStreamPause(test_soundStream_p);
	    soundPause(test_sound_p);
	}
	if(glfwGetKey(game_window.window_p, GLFW_KEY_E) == GLFW_PRESS)
	{
	    //soundStreamStop(test_soundStream_p);
	    soundStop(test_sound_p);
	}

	/////////////////////////
	// Update sound stream //
	/////////////////////////
	soundStreamUpdate(test_soundStream_p);
	
	///////////////////
	// Update camera //
	///////////////////

	cameraUpdate(global_cam, game_window.window_p, cursorGetDistance(global_cursor), d_time);
	
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
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh_p->data.size());

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
	GameWindowSwapBuffers(game_window);

	////////////////////
	// Process Inputs //
	////////////////////
        // Store all inputs that occurred this cycle in the input_manager. To be processed next cycle.
	// inputManagerGetInputsThisFrame(input_manager);
	 
	/////////////////////
	// Close condition //
	/////////////////////
	if(glfwGetKey(game_window.window_p, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
	    glfwSetWindowShouldClose(game_window.window_p, GLFW_TRUE);
	}
    }

    /////////////
    // Cleanup //
    /////////////
    
    // Delete pointers to structs on the heap
    delete test_soundStream_p;
    delete ftexture_p;
    delete dirLight_p;
    
    return 0;
}
