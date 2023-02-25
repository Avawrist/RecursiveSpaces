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

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"
#include "mdcla.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "asset.hpp"
#include "light.hpp"
#include "sound.hpp"
#include "asset_manager.hpp"
#include "render.hpp"
#include "input.hpp"

using namespace std;

int main()
{
    /////////////////////////
    // Init Render & Input //
    /////////////////////////
    
    // Initialize GLFW (Enumerates windows, joysticks, starts the timer)
    if(!initGLFW()) {return -1;}

    // Get Game Window
    GameWindow game_window(1024, 576, "First Game"); // GLFW terminates on deletion 
    if(!game_window.window_p) {return -1;}

    // Get Camera
    Camera camera(Vec3F(0.0f, 2.0f, 4.0f), 0.8f, 100.0f, 45.0f, game_window.win_ar);
    
    // Get Cursor
    Cursor cursor(game_window.x_center, game_window.y_center);
    
    // Get Input Manager
    InputManager input_manager;
    
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
    FrameTexture *ftexture_p = new FrameTexture(game_window.view_width, game_window.view_height);
    frameTextureDataToGPU(ftexture_p);
    
    /////////////////////////////
    // Transformation Matrices //
    /////////////////////////////

    // Model Matrix (Local space -> world space)
    Mat4F model(1.0f);
    // View Matrix (World space -> view space)
    Mat4F view = cameraGetView(camera);
    // Projection Matrix (View space -> clip space/NDC)
    Mat4F projection = cameraGetPerspective(camera);
    
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
	// At the highest level should boil down to:
	// 1. GAME - Update game state based on prior cycle inputs
	// 2. PLATFORM - Render game state
	// 3. PLATFORM - Get all inputs received this cycle (to be used to update game state next cycle)
	
	///////////////////////
	// Update delta time //
	///////////////////////
	gameWindowUpdateTime(game_window);

	///////////////////
	// Update cursor //
	///////////////////
	cursorUpdate(cursor, game_window.window_p);
	
	//////////////////
	// Update sound //
	//////////////////
	if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_SPACE] == KEY_DOWN)
	{
	    soundPlay(test_sound_p);
	}
	if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_LEFT] == KEY_DOWN)
	{
	    soundPause(test_sound_p);
	}
	if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_RIGHT] == KEY_DOWN)
	{
	    soundStop(test_sound_p);
	}

	/////////////////////////
	// Update sound stream //
	/////////////////////////
	soundStreamUpdate(test_soundStream_p);
	
	///////////////////
	// Update camera //
	///////////////////
	cameraUpdate(camera, game_window, input_manager, cursorGetDistance(cursor));
	
	////////////////////////////
	// Update Shader Uniforms //
	////////////////////////////
	glUseProgram(bp_shader_p->program_id);
	
	// DirLights
	shaderAddVec3Uniform(bp_shader_p,  "dirLight.color", dirLight_p->color);
	shaderAddVec3Uniform(bp_shader_p,  "dirLight.dir",   dirLight_p->dir);
	shaderAddFloatUniform(bp_shader_p, "dirLight.ambient_strength", dirLight_p->ambient_strength);

	// Cam Position
	shaderAddVec3Uniform(bp_shader_p, "cam_pos", camera.pos);
	
	// Transformation Matrices
     
	// Update View
	view = cameraGetView(camera);
	shaderAddMat4Uniform(bp_shader_p, "view", view.getPointer());
	
	// Update perspective matrix with potential new AR
	// (TO-DO: this is expensive, only calculate new projection mat if ar changes )
	projection = cameraGetPerspective(camera);
	shaderAddMat4Uniform(bp_shader_p, "projection", projection.getPointer());

	/////////////////////////
	// ** Render pass 1 ** //
	/////////////////////////

	// Use this render pass to write pre-processed image to the color texture.
	glViewport(0, 0, game_window.view_width, game_window.view_height); // Render to a smaller area first
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
	glViewport(0, 0, game_window.win_width, game_window.win_height); // Render to the full screen now,
	                                         // blowing up the screen texture
        frameTextureDraw(ftexture_p, pp_shader_p);
	
	//////////////////
	// Swap buffers //
	//////////////////
	gameWindowSwapBuffers(game_window);

	////////////////////
	// Process Inputs //
	////////////////////
        // Store all inputs that occurred this cycle in the input_manager. To be processed next cycle.
	inputManagerGetInputsThisFrame(input_manager, game_window);
	
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
