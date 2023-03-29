// ====================================================================================
// Title: game.cpp
// Description: The primary source file for an untitled game, containing the game loop.
// ====================================================================================

// Platform lib
#include "platform.hpp"

// C Libs
#include "cstdlib"

// Game libs
#include "input.hpp"
#include "asset.hpp"
#include "ecs.hpp"
#include "draw.hpp"

#include "utility.hpp"
#include "mdcla.hpp"

using namespace std;

////////////////////////
// Function Prototpes //
////////////////////////

void gameInit(GameWindow& game_window, InputManager& input_manager);

void gameUpdateStates(ActiveEntities& entities);

void gameUpdateGridPositions(ActiveEntities& entities, LevelGrid& grid, InputManager& input_manager);

void gameUpdateTransforms(ActiveEntities& active_entities, LevelGrid& grid);

uint gameUpdateCameras(ActiveEntities& active_entities, GameWindow& game_window,
		       InputManager& input_manager);

int gameUpdateAndRender(SoundStream* sound_stream_p, GameWindow& game_window, InputManager& input_manager,
			ActiveEntities& active_entities, AssetManager& asset_manager,
			FrameTexture* ftexture_p, LevelGrid& level_grid, DebugGrid* grid_p);

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
    gameInit(game_window, input_manager);
    AssetManager    asset_manager;
    SoundInterface  sound_interface;
    ActiveEntities* active_entities_p = new ActiveEntities();
    platformLoadEntityTemplatesFromTxt(*active_entities_p, "..\\data\\templates\\entity_templates.txt");

    ////////////////
    // Level Grid //
    ////////////////
    LevelGrid* level_grid_p = new LevelGrid();
    
    /////////////////
    // Temp Assets //
    /////////////////
    
    // Initialize BGM
    SoundStream* test_soundStream_p = new SoundStream("..\\data\\assets\\bgm\\elephant.wav", sound_interface);
    // Initialize Framebuffer
    FrameTexture* ftexture_p = new FrameTexture(game_window.view_width, game_window.view_height);
    frameTextureDataToGPU(ftexture_p);
    // Create Debug Grid Object
    DebugGrid* grid_p = new DebugGrid(level_grid_p->dimensions, MAX_WIDTH + 1, MAX_LENGTH + 1,
				      Vec3F(-level_grid_p->dimensions * 0.5f, 0.0f, -level_grid_p->dimensions * 0.5f));

    ///////////////////
    // Test Entities //
    ///////////////////
    
    // Blocks
    for(int x = 0; x < MAX_WIDTH; x++)
    {
	for(int z = 0; z < MAX_LENGTH; z++)
	{
	    activeEntitiesCreateEntity(*active_entities_p, *level_grid_p,
				       Vec3F((float)x, 0.0f, (float)z), BLOCK);
	}
    }
    
    // DirLight
    activeEntitiesCreateEntity(*active_entities_p, *level_grid_p, Vec3F(0.0f, 0.0f, 0.0f), DIR_LIGHT);
    
    // Camera
    activeEntitiesCreateEntity(*active_entities_p, *level_grid_p, Vec3F(60.0f, 60.0f, 60.0f), CAMERA);

    ///////////////
    // Game Loop //
    ///////////////
    while(!game_window.close)
    {	
	gameUpdateAndRender(test_soundStream_p, game_window, input_manager, *active_entities_p,
			    asset_manager, ftexture_p, *level_grid_p, grid_p);
        gameUpdateInputs(input_manager, game_window); // Store all inputs received this cycle
	
	// Close condition
	if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ESC] == KEY_DOWN) { game_window.close = true;}
    }

    /////////////
    // Cleanup //
    /////////////
    platformFreeWindow(game_window);
    
    // Delete pointers to structs on the heap
    delete level_grid_p;
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

void gameUpdateStates(ActiveEntities& entities)
{
    for(uint i = 0; i < entities.count; i++)
    {
	if(entities.entity_templates.table[entities.type[i]][COMPONENT_STATE])
	{
	    entities.state[i].input_cooldown -= 1;
	    entities.state[i].input_cooldown = (int)clamp((float)entities.state[i].input_cooldown,
						           0, INPUT_COOLDOWN_DUR);
	}
    }
}

void gameUpdateGridPositions(ActiveEntities& entities, LevelGrid& grid, InputManager& input_manager)
{
    for(uint i = 0; i < entities.count; i++)
    {
	if(entities.entity_templates.table[entities.type[i]][COMPONENT_GRID_POSITION])
	{
	    Vec3F cur_grid_pos = entities.grid_position[i].position;
	    Vec3F new_grid_pos = cur_grid_pos;

	    if(entities.state[i].input_cooldown == 0)
	    {
		// Down
		if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_DOWN] == KEY_DOWN)
		{
		    if(cur_grid_pos.z + 1 >= 0.0f && cur_grid_pos.z + 1 < MAX_LENGTH)
		    {
			new_grid_pos = Vec3F(cur_grid_pos.x,
					     cur_grid_pos.y,
					     cur_grid_pos.z + 1);
			entities.state[i].input_cooldown = INPUT_COOLDOWN_DUR;
		    }
		}
		// Up
		else if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_UP] == KEY_DOWN)
		{
		    if(cur_grid_pos.z - 1 >= 0.0f && cur_grid_pos.z - 1 < MAX_LENGTH)
		    {
			new_grid_pos = Vec3F(cur_grid_pos.x,
					     cur_grid_pos.y,
					     cur_grid_pos.z - 1);
			entities.state[i].input_cooldown = INPUT_COOLDOWN_DUR;
		    }
		}
		// Left
		else if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_LEFT] == KEY_DOWN)
		{
		    if(cur_grid_pos.x - 1 >= 0.0f && cur_grid_pos.x - 1 < MAX_WIDTH)
		    {
			new_grid_pos = Vec3F(cur_grid_pos.x - 1,
					     cur_grid_pos.y,
					     cur_grid_pos.z);
			entities.state[i].input_cooldown = INPUT_COOLDOWN_DUR;
		    }
		}
		// Right
		else if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_RIGHT] == KEY_DOWN)
		{
		    if(cur_grid_pos.x + 1 >= 0.0f && cur_grid_pos.x + 1 < MAX_WIDTH)
		    {
			new_grid_pos = Vec3F(cur_grid_pos.x + 1,
					     cur_grid_pos.y,
					     cur_grid_pos.z);
			entities.state[i].input_cooldown = INPUT_COOLDOWN_DUR;
		    }
		}
	    }
	    levelGridRemoveEntity(grid, cur_grid_pos);
	    levelGridSetEntity(grid, entities, new_grid_pos, i);
	    entities.grid_position[i].position = new_grid_pos;
	}
    }
}
    
void gameUpdateTransforms(ActiveEntities& entities, LevelGrid& grid)
{
    for(uint i = 0; i < entities.count; i++)
    {
	if(entities.entity_templates.table[entities.type[i]][COMPONENT_GRID_POSITION])
	{
	    entities.transform[i].position.x = entities.grid_position[i].position.x * grid.dimensions;
	    entities.transform[i].position.y = entities.grid_position[i].position.y * grid.dimensions;
	    entities.transform[i].position.z = entities.grid_position[i].position.z * grid.dimensions;
	}
    }
}

uint gameUpdateCameras(ActiveEntities& active_entities, GameWindow& game_window,
		       InputManager& input_manager)
{
    float speed       = 8.0f;
    float z_speed     = 20.0f;
    float sensitivity = 1.0f;
    uint  entity_id   = 0;
    
    for(uint i = 0; i < active_entities.count; i++)
    {
	if(active_entities.entity_templates.table[active_entities.type[i]][COMPONENT_TRANSFORM] &&
	   active_entities.entity_templates.table[active_entities.type[i]][COMPONENT_CAMERA])
	{
	    Mat4F V          = cameraGetView(active_entities.camera[i],
		                             active_entities.transform[i].position);
	    float d_time_spd = game_window.d_time * speed;
	    Vec2F distance   = cursorGetDistance(input_manager.cursor);  

	    // Camera Strafing

	    // Move forward
	    if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_W] == KEY_DOWN)
	    {
		active_entities.transform[i].position -= (normalize(Vec3F(V(0, 2), V(1, 2), V(2, 2)))
							  * d_time_spd);
	    }
	    // Move back
	    if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_S] == KEY_DOWN)
	    {
		active_entities.transform[i].position += (normalize(Vec3F(V(0, 2), V(1, 2), V(2, 2)))
							  * d_time_spd);
	    }
	    // Move right
	    if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_D] == KEY_DOWN)
	    {
	        active_entities.transform[i].position += (normalize(Vec3F(V(0, 0), V(1, 0), V(2, 0)))
							  * d_time_spd);
	    }
	    // Move left
	    if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_A] == KEY_DOWN)
	    {
	        active_entities.transform[i].position -= (normalize(Vec3F(V(0, 0), V(1, 0), V(2, 0)))
							  * d_time_spd);
	    }

	    ////////////////////
	    // Camera Looking //
	    ////////////////////
	    cameraOffsetAngles(active_entities.camera[i],
			       sensitivity * distance.x,
			       sensitivity * distance.y);

	    /////////////////////////////
	    // Set ID to Render Camera //
	    /////////////////////////////
	    if(active_entities.camera[i].is_selected) { entity_id = i; }
	}
    }

    return entity_id;
}

uint gameUpdateDirLights(ActiveEntities& active_entities, GameWindow& game_window)
{
    uint entity_id = 0;
    
    for(uint i = 0; i < active_entities.count; i++)
    {
	if(active_entities.entity_templates.table[active_entities.type[i]][COMPONENT_DIR_LIGHT])
	{
	    entity_id = i;
	}
    }

    return entity_id;
}

int gameUpdateAndRender(SoundStream* sound_stream_p, GameWindow& game_window, InputManager& input_manager,
			ActiveEntities& active_entities, AssetManager& asset_manager,
			FrameTexture* ftexture_p, LevelGrid& level_grid, DebugGrid* grid_p)
{
    ////////////
    // Update //
    ////////////

    // Update States
    gameUpdateStates(active_entities);
    
    // Update Grid Positions
    gameUpdateGridPositions(active_entities, level_grid, input_manager);
    
    // Update Transforms
    gameUpdateTransforms(active_entities, level_grid);
    
    // Update soundstream
    soundStreamUpdate(sound_stream_p);
    
    // Update Cameras
    uint cam_id = gameUpdateCameras(active_entities, game_window, input_manager);

    // Update Lights - TODO: Should eventually store an array of all lights
    uint dir_light_id = gameUpdateDirLights(active_entities, game_window);
   
    // Remove Inactive Entities - Must be run after all other entity updates
    activeEntitiesRemoveInactives(active_entities, level_grid);
    
    ///////////////////////////////
    // Render Pass 1 -  Entities //
    ///////////////////////////////

    // Update shader uniforms
    platformSetRenderStateDefault(game_window, *ftexture_p);
    platformPrepShaderDefault(game_window,
			      asset_manager,
			      active_entities.camera[cam_id],
			      active_entities.transform[cam_id].position,
			      active_entities.dir_light[dir_light_id]);

    // Render renderable entities
    for(uint i = 0; i < active_entities.count; i++)
    {
	if(active_entities.entity_templates.table[active_entities.type[i]][COMPONENT_RENDER] &&
	   active_entities.entity_templates.table[active_entities.type[i]][COMPONENT_TRANSFORM])
	{
	    Mat4F model = transformGetModel(active_entities.transform[i]);
	    platformRenderEntity(asset_manager, active_entities.type[i], model);
	}
    }

    // Render debug elements
    platformPrepShaderDebug(game_window,
			    asset_manager,
			    active_entities.camera[cam_id],
			    active_entities.transform[cam_id].position);
    platformRenderDebug(asset_manager, grid_p);

    /////////////////////////////////////
    // Render Pass 2 - Post Processing //
    /////////////////////////////////////
    platformRenderPP(asset_manager, game_window, ftexture_p);

    // Swap buffers
    platformSwapBuffers(game_window);

    return 1;
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
