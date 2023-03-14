// ====================================================================================
// Title: game.cpp
// Description: The primary source file for an untitled game, containing the game loop.
// ====================================================================================

// Platform lib
#include "platform.hpp"

// Game libs
#include "input.hpp"
#include "asset.hpp"
#include "ecs.hpp"
#include "draw.hpp"

#include "camera.hpp" // Move camera to component
#include "light.hpp" // Move light to component

// Utility libs
#include "utility.hpp"
#include "mdcla.hpp"

using namespace std;

////////////////////////
// Function Prototpes //
////////////////////////

void gameInit(GameWindow& game_window, InputManager& input_manager);

void gameUpdateCamera(Camera& cam, GameWindow& game_window, InputManager& input_manager);

void gameUpdate(SoundStream* sound_stream_p, Camera& camera, GameWindow& game_window,
		InputManager& input_manager, ActiveEntities* active_entities_p);

void gameRender(ActiveEntities& active_entities, GameWindow& game_window, FrameTexture* ftexture_p,
                AssetManager& asset_manager, Camera& camera, DirLight& dir_light, DebugGrid* grid_p);

void gameUpdateInputs(InputManager& im, GameWindow& game_window);

//////////
// Main //
//////////

int main()
{
    /////////////////////////////////////////////////////////////////////
    // Initialize Window, Input, Render, AssetManager, Sound, Entities //
    /////////////////////////////////////////////////////////////////////
    GameWindow      game_window;
    InputManager    input_manager;
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
    // Initialize Framebuffer
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
	activeEntitiesCreateEntity(*active_entities_p, pos, CHEST, &sound_interface);
    }

    ///////////////
    // Game Loop //
    ///////////////
    while(!game_window.close)
    {
	gameUpdate(test_soundStream_p, camera, game_window,
		   input_manager, active_entities_p); // Update entities
	gameRender(*active_entities_p, game_window, ftexture_p, asset_manager,
		   camera, dir_light, grid_p); // Render entities
        gameUpdateInputs(input_manager, game_window); // Store all inputs received this cycle

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

//////////////////////////
// Function Definitions //
//////////////////////////

void gameInit(GameWindow& game_window, InputManager& input_manager)
{
    platformInitAPIs(game_window);
    gameUpdateInputs(input_manager, game_window);
}

void gameUpdateCamera(Camera& cam, GameWindow& game_window, InputManager& input_manager)
{
    Mat4F V          = cameraGetView(cam);
    float d_time_spd = game_window.d_time * cam.speed;
    Vec2F distance   = cursorGetDistance(input_manager.cursor);  

    // Camera Strafing

    // Move forward
    if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_W] == KEY_DOWN)
    {
	cam.pos -= normalize(Vec3F(V(0, 2), V(1, 2), V(2, 2))) * d_time_spd;
    }
    // Move back
    if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_S] == KEY_DOWN)
    {
        cam.pos += normalize(Vec3F(V(0, 2), V(1, 2), V(2, 2))) * d_time_spd;
    }
    // Move right
    if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_D] == KEY_DOWN)
    {
        cam.pos += normalize(Vec3F(V(0, 0), V(1, 0), V(2, 0))) * d_time_spd;
    }
    // Move left
    if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_A] == KEY_DOWN)
    {
        cam.pos -= normalize(Vec3F(V(0, 0), V(1, 0), V(2, 0))) * d_time_spd;
    }

    ////////////////////
    // Camera Looking //
    ////////////////////
    cameraOffsetAngles(cam,
		       cam.sensitivity * distance.x,
		       cam.sensitivity * distance.y);
}

void gameUpdate(SoundStream* sound_stream_p, Camera& camera, GameWindow& game_window,
		InputManager& input_manager, ActiveEntities* active_entities_p)
{
	// Update soundstream
	soundStreamUpdate(sound_stream_p);
	
	// Update camera - TODO: Move camera to component, update with entities
        gameUpdateCamera(camera, game_window, input_manager);

	// Update Entities
	//...
}

void gameRender(ActiveEntities& active_entities, GameWindow& game_window, FrameTexture* ftexture_p,
                AssetManager& asset_manager, Camera& camera, DirLight& dir_light, DebugGrid* grid_p)
{
    // Pass 1 - Render Entities
    platformSetRenderStateDefault(game_window, *ftexture_p);
    platformPrepShaderDefault(game_window, asset_manager, camera, dir_light);    
    for(uint i = 0; i < MAX_ENTITIES; i++)
    {
	if((active_entities.mask[i] & std::bitset<MAX_COMPONENTS>(COMPONENT_RENDER | COMPONENT_TRANSFORM)) ==
	   (COMPONENT_RENDER | COMPONENT_TRANSFORM))
	{
	    Mat4F model = transformGetModel(active_entities.transform[i]);
	    platformRenderEntity(asset_manager, active_entities.type[i], model);
	}
    }
    platformPrepShaderDebug(game_window, asset_manager, camera);
    platformRenderDebug(asset_manager, grid_p);
    
    // Pass 2 - Post Processing
    platformRenderPP(asset_manager, game_window, ftexture_p);

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
