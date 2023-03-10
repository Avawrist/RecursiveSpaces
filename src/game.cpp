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
#include "camera.hpp" // Move camera to component
#include "asset.hpp"
#include "light.hpp" // Move light to component
#include "sound.hpp" // Move sound to component
#include "asset_manager.hpp"
#include "render.hpp"
#include "input.hpp"
#include "draw.hpp" // Move debug grid to component
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
    DirLight dir_light(Vec3F(1.0f, 1.0f, 1.0f),
		       Vec3F(0.0f, -1.0f, -1.0f),
	               0.25f);

    /////////////////////////////
    // Initialize Framebuffers //
    /////////////////////////////
    
    // Texture size should match the viewport.
    // We will then blow it up to fit the screen for a low res look.
    FrameTexture* ftexture_p = new FrameTexture(game_window.view_width, game_window.view_height);
    frameTextureDataToGPU(ftexture_p);

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

    ////////////////////////
    // Create Grid Object //
    ////////////////////////
    DebugGrid* grid_p = new DebugGrid(5.0f, 10, 10, Vec3F(0.0f, 0.0f, 0.0f));
    
    ///////////////
    // Game Loop //
    ///////////////
    while(!glfwWindowShouldClose(game_window.window_p))
    {
	///////////////////////
	// Update Game State //
	///////////////////////

	// Update soundstream
	soundStreamUpdate(test_soundStream_p);
	
	// Update camera - TODO: Move camera to component, update with entities
	cameraUpdate(camera, game_window, input_manager);

	// Update Entities
	//...
	
	/////////////////
	// Render Game //
	/////////////////
        
	// Pass 1 - Render Entities
	activeEntitiesRender(*active_entities_p, game_window, *ftexture_p,
			     shader_manager, camera, dir_light, grid_p);

        // Pass 2 - Post Processing
	glViewport(0, 0, game_window.win_width, game_window.win_height);
	Shader* pp_shader_p = (Shader*)shaderManagerGetShaderP(shader_manager, POSTPROCESS);
	shaderAddIntUniform(pp_shader_p, "color_texture", 0);
	frameTextureDraw(ftexture_p, pp_shader_p);

	// Swap buffers
	gameWindowSwapBuffers(game_window);
	
	////////////////
	// Get Inputs //
	////////////////

        // Store all inputs this cycle, to be processed next cycle. Update the cursor pos.
	inputManagerUpdate(input_manager, game_window);

	// Close condition
	if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ESC] == KEY_DOWN)
	{
	    gameWindowClose(game_window);
	}
    }

    /////////////
    // Cleanup //
    /////////////
    
    // Delete pointers to structs on the heap
    delete active_entities_p;
    delete grid_p;
    delete test_soundStream_p;
    delete ftexture_p;
    
    return 0;
}
