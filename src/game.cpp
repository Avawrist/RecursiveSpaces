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

// Utility libs
#include "utility.hpp"
#include "mdcla.hpp"

// Game libs
#include "game_window.hpp"
#include "input.hpp"
#include "asset.hpp"
#include "component.hpp"
#include "entity.hpp"

#include "camera.hpp" // Move camera to component
#include "light.hpp" // Move light to component
#include "sound.hpp" // Move sound to component
#include "draw.hpp" // Move debug grid to component

// Platform lib
#include "platform.hpp"

using namespace std;

void gameInit(GameWindow& game_window, InputManager& input_manager);

void gameUpdate(SoundStream* sound_stream_p, Camera& camera, GameWindow& game_window,
		InputManager& input_manager, ActiveEntities* active_entities_p);

void gameRender(ActiveEntities* active_entities_p, GameWindow& game_window, FrameTexture* ftexture_p,
                AssetManager& asset_manager, Camera& camera, DirLight& dir_light, DebugGrid* grid_p);

void gameUpdateInputs(InputManager& im, GameWindow& game_window);

int main()
{
    /////////////////////////////////////////////////////////////////////
    // Initialize Window, Input, Render, AssetManager, Sound, Entities //
    /////////////////////////////////////////////////////////////////////
    GameWindow      game_window;
    InputManager    input_manager(game_window);
    AssetManager    asset_manager;
    SoundInterface  sound_interface;
    ActiveEntities* active_entities_p = new ActiveEntities();
    gameInit(game_window, input_manager);

    ///////////////////////////////////////
    // Temp Objects - Move to Components //
    ///////////////////////////////////////

    // Initialize BGM
    SoundStream* test_soundStream_p = new SoundStream("..\\assets\\bgm\\elephant.wav", sound_interface);
    // Init Camera
    Camera camera(Vec3F(350.0f, 350.0f, 350.0f), game_window.win_ar);
    // Initialize Lights
    DirLight dir_light(Vec3F(1.0f, 1.0f, 1.0f),
		       Vec3F(0.0f, -1.0f, -1.0f),
	               0.25f);
    // Initialize Framebuffers    
    // Texture size should match the viewport.
    // We will then blow it up to fit the screen for a low res look.
    FrameTexture* ftexture_p = new FrameTexture(game_window.view_width, game_window.view_height);
    frameTextureDataToGPU(ftexture_p);
    // Create Grid Object
    DebugGrid* grid_p = new DebugGrid(5.0f, 10, 10, Vec3F(0.0f, 0.0f, 0.0f));

    ///////////////////
    // Temp Entities //
    ///////////////////
    for(int i = 0; i < 10; i++)
    {
	Vec3F pos = Vec3F(i * 5.0f, 0.0f, 0.0f);
	activeEntitiesCreateEntity(*active_entities_p, asset_manager, pos, CHEST, &sound_interface);
    }

    ///////////////
    // Game Loop //
    ///////////////
    while(!game_window.close)
    {
	gameUpdate(test_soundStream_p, camera, game_window, input_manager, active_entities_p); // Update entities
	gameRender(active_entities_p, game_window, ftexture_p, asset_manager, camera, dir_light, grid_p); // Render entities
        gameUpdateInputs(input_manager, game_window); // Store all inputs received this cycle, processed next cycle

	// Close condition
	if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ESC] == KEY_DOWN) { game_window.close = true;}
    }

    /////////////
    // Cleanup //
    /////////////
    platformFreeWindow(game_window);
    
    // Delete pointers to structs on the heap
    delete active_entities_p;
    delete grid_p;
    delete test_soundStream_p;
    delete ftexture_p;
    
    return 0;
}

void gameInit(GameWindow& game_window, InputManager& input_manager)
{
    platformInitAPIs(game_window);
    gameUpdateInputs(input_manager, game_window);
}

void gameUpdate(SoundStream* sound_stream_p, Camera& camera, GameWindow& game_window,
		InputManager& input_manager, ActiveEntities* active_entities_p)
{
	// Update soundstream
	soundStreamUpdate(sound_stream_p);
	
	// Update camera - TODO: Move camera to component, update with entities
	cameraUpdate(camera, game_window, input_manager);

	// Update Entities
	//...
}

void gameRender(ActiveEntities* active_entities_p, GameWindow& game_window, FrameTexture* ftexture_p,
                AssetManager& asset_manager, Camera& camera, DirLight& dir_light, DebugGrid* grid_p)
{
    // Pass 1 - Render Entities
    activeEntitiesRender(*active_entities_p, game_window, *ftexture_p,
			 asset_manager, camera, dir_light, grid_p);

    // Pass 2 - Post Processing
    Shader* pp_shader_p = (Shader*)assetManagerGetShaderP(asset_manager, POSTPROCESS);
    frameTextureRender(ftexture_p, game_window, pp_shader_p);

    // Swap buffers
    platformSwapBuffers(game_window);
}

void gameUpdateInputs(InputManager& im, GameWindow& game_window)
{

    // Copy frame inputs to the prior frame inputs to make room for new inputs
    for(int i = TOTAL_FRAMES; i > 1; i--)
    {
	for(int j = 0; j < TOTAL_KEYS; j++)
	{
	    im.inputs_on_frame[i - 1][j] = im.inputs_on_frame[i - 2][j];
	}
    }
    
    // Get inputs received this frame
    platformGetInputsThisFrame(im, game_window);
}
