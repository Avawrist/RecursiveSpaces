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
#include "asset.hpp"
#include "light.hpp"
#include "sound.hpp"
#include "asset_manager.hpp"
#include "render.hpp"
#include "input.hpp"
#include "draw.hpp"
#include "component.hpp"
#include "entity.hpp"

using namespace std;

int main()
{
    /////////////////////////
    // Init Render & Input //
    /////////////////////////
    
    // Initialize GLFW (Enumerates windows, joysticks, starts the timer)
    if(!initGLFW()) {return -1;}

    // Get Game Window
    GameWindow game_window(1920, 1080, "First Game"); // GLFW terminates on deletion 
    if(!game_window.window_p) {return -1;}
    
    // Get Input Manager
    InputManager input_manager(game_window);

    // Get Camera
    Camera camera(Vec3F(350.0f, 350.0f, 350.0f), game_window.win_ar);
    
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
    // Initialize BGM //
    ////////////////////
    SoundStream* test_soundStream_p = new SoundStream("..\\assets\\bgm\\elephant.wav", sound_interface);
    
    ///////////////////////
    // Initialize Lights //
    ///////////////////////
    DirLight* dirLight_p = new DirLight(Vec3F(1.0f, 1.0f, 1.0f),
					Vec3F(0.0f, -1.0f, -1.0f),
	                                0.25f);

    /////////////////////////////
    // Initialize Framebuffers //
    /////////////////////////////
    
    // Texture size should match the viewport.
    //We will then blow it up to fit the screen for a low res look.
    FrameTexture* ftexture_p = new FrameTexture(game_window.view_width, game_window.view_height);
    frameTextureDataToGPU(ftexture_p);
    
    /////////////////////////////
    // Transformation Matrices //
    /////////////////////////////

    // Model Matrix (Local space -> world space)
    Mat4F model(1.0f); // Each game object will have its own model matrix
    // View Matrix (World space -> view space)
    Mat4F view = cameraGetView(camera);
    // Projection Matrix (View space -> clip space/NDC)
    Mat4F projection = cameraGetOrthographic(camera, game_window);
    
    ////////////////////
    // Create Shaders //
    ////////////////////

    // Blinn-phong shader
    Shader* bp_shader_p = (Shader*)shaderManagerGetShaderP(shader_manager, BLINNPHONG);
    
    // Load initial uniform values to GPU
    glUseProgram(bp_shader_p->program_id);
    shaderAddMat4Uniform(bp_shader_p, "model",      model.getPointer());
    shaderAddMat4Uniform(bp_shader_p, "view",       view.getPointer());
    shaderAddMat4Uniform(bp_shader_p, "projection", projection.getPointer());
    shaderAddIntUniform(bp_shader_p, "diffuse_map", 0);
    shaderAddIntUniform(bp_shader_p, "normal_map",  1);
    
    // Post-processing shader
    Shader* pp_shader_p = (Shader*)shaderManagerGetShaderP(shader_manager, POSTPROCESS);
	
    // Load initial uniform values to GPU
    glUseProgram(pp_shader_p->program_id);
    shaderAddIntUniform(pp_shader_p, "color_texture", 0);

    // Grid shader
    Shader* grid_shader_p = (Shader*)shaderManagerGetShaderP(shader_manager, GRID);

    // Load initial uniform values to GPU
    glUseProgram(grid_shader_p->program_id);
    shaderAddMat4Uniform(grid_shader_p, "model", model.getPointer());
    shaderAddMat4Uniform(grid_shader_p, "view", view.getPointer());
    shaderAddMat4Uniform(grid_shader_p, "projection", projection.getPointer());

    ////////////////////////
    // Create Grid Object //
    ////////////////////////
    DebugGrid* grid_p = new DebugGrid(5.0f, 10, 10, Vec3F(0.0f, 0.0f, 0.0f));

    ///////////////////////////
    // Create ActiveEntities //
    ///////////////////////////

    ActiveEntities* active_entities_p = new ActiveEntities();

    ////////////////////////////
    // Add some test entities //
    ////////////////////////////
    for(int i = 0; i < 10; i++)
    {
	Vec3F pos = Vec3F(i * 5.0f, 0.0f, 0.0f);
	activeEntitiesCreateEntity(*active_entities_p, asset_manager, shader_manager,
			           pos, CHEST, &sound_interface);
    }
    
    ///////////////
    // Game Loop //
    ///////////////
    while(!glfwWindowShouldClose(game_window.window_p))
    {
	// At the highest level should boil down to:
	// 1. GAME - Update game state based on prior cycle inputs
	// 2. PLATFORM - Render game state
	// 3. PLATFORM - Get all inputs received this cycle (to be used to update game state next cycle)

	/////////////////////////
	// Update sound stream //
	/////////////////////////
	soundStreamUpdate(test_soundStream_p);
	
	///////////////////
	// Update camera //
	///////////////////
	cameraUpdate(camera, game_window, input_manager);
        
	////////////////////////////////////////
	// Update Blinn-Phong Shader Uniforms //
	////////////////////////////////////////
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
	projection = cameraGetOrthographic(camera, game_window);
	shaderAddMat4Uniform(bp_shader_p, "projection", projection.getPointer());

	/////////////////////////////////
	// Update Line Shader Uniforms //
	/////////////////////////////////
	glUseProgram(grid_shader_p->program_id);

	// Update view & projection matrix uniforms
	shaderAddMat4Uniform(grid_shader_p, "view", view.getPointer());
	shaderAddMat4Uniform(grid_shader_p, "projection", projection.getPointer());
	
	/////////////////////////
	// ** Render pass 1 ** //
	/////////////////////////
        
	// Render Entities
	activeEntitiesRender(*active_entities_p, game_window, *ftexture_p);
	
	// Draw DebugGrid
	debugGridDraw(grid_p, grid_shader_p, Vec3F(0.0f, 0.0f, 1.0f), 1.0f);
	
	///////////////////////////////////////////
	// ** Render pass 2 (Post-processing) ** //
	///////////////////////////////////////////

	// use this render pass to draw post-processed color texture to screen.
	glViewport(0, 0, game_window.win_width, game_window.win_height); // Render to the full screen now,
	                                                                 // blowing up the screen texture
        frameTextureDraw(ftexture_p, pp_shader_p);

	////////////////////
	// Process Inputs //
	////////////////////

        // Store all inputs this cycle, to be processed next cycle. Update the cursor pos.
	inputManagerUpdate(input_manager, game_window);

	/////////////////////
	// Close condition //
	/////////////////////
	if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ESC] == KEY_DOWN)
	{
	    gameWindowClose(game_window);
	}
	
	//////////////////
	// Swap buffers //
	//////////////////
	gameWindowSwapBuffers(game_window);
    }

    /////////////
    // Cleanup //
    /////////////
    
    // Delete pointers to structs on the heap
    delete active_entities_p;
    delete grid_p;
    delete test_soundStream_p;
    delete ftexture_p;
    delete dirLight_p;
    
    return 0;
}
