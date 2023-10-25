// ====================================================================================
// Title: game.cpp
// Description: The primary source file for an untitled game, containing the game loop.
// ====================================================================================

// Platform lib
#include "platform.hpp"

// C Libs
#include "cmath"

// Game libs
#include "input.hpp"
#include "asset.hpp"
#include "ecs.hpp"
#include "draw.hpp"
#include "utility.hpp"
#include "mdcla.hpp"

////////////////////////
// Function Prototpes //
////////////////////////

void gameInit(uint width, uint height);

void gameUpdateStates(ActiveEntities& entities);

void gameUpdatePlayer();

void gameUpdateAI();

void gameUpdateTransforms();

uint gameUpdateCameras();

uint gameUpdateDirLights(float time);

void gameUpdateRoomGrid();

int gameUpdate(SoundStream* sound_stream_p,
	       int& cam_id,
	       int& dir_light_id);

int gameRender(FrameTexture* depth_ftexture_p,
	       FrameTexture* ftexture_msaa_p,
	       FrameTexture* ftexture_non_msaa_p,
	       const RoomGridLookup& roomgrid_lookup,
	       DebugGrid* grid_p,
	       uint cam_id,
	       uint dir_light_id);

void gameUpdateInputs();

int gameMoveEntitiesOnGrid(RoomGrid& grid,
			   Vec3F cur_grid_position,
			   Vec3F new_grid_position);

/////////////
// Globals //
/////////////

InputManager    input_manager;
GameWindow      game_window;
AssetManager    asset_manager;
ActiveEntities* active_entities_p = new ActiveEntities();
RoomGridLookup  roomgrid_lookup;

//////////
// Main //
//////////

int main()
{
    /////////////////////////////////////////////////////////////////////
    // Initialize Window, Input, Render, AssetManager, Sound, Entities //
    /////////////////////////////////////////////////////////////////////
    gameInit(1920, 1080);
    SoundInterface  sound_interface;
    platformLoadEntityTemplatesFromTxt(*active_entities_p, "..\\data\\templates\\entity_templates.txt");

    ////////////////////
    // Base Room Grid //
    ////////////////////

    roomGridLookupInit(roomgrid_lookup);
    int br_id = activeEntitiesCreateEntity(*active_entities_p,
					   roomgrid_lookup,
					   -1,
					   Vec3F(0.0f, 0.0f, 0.0f),
					   BLOCK_ROOM);
    roomgrid_lookup.roomgrid_pointers[ROOMGRID_A] = &active_entities_p->roomgrids[br_id];
        
    /////////////////
    // Temp Assets //
    /////////////////
    
    // Initialize BGM
    SoundStream* test_soundStream_p = new SoundStream("..\\data\\assets\\bgm\\elephant.wav", sound_interface);
    // Initialize Framebuffers
    FrameTexture* depth_ftexture_p = new FrameTexture(2048, 2048, true, false);
    frameTextureDataToGPU(depth_ftexture_p);
    FrameTexture* ftexture_msaa_p = new FrameTexture(game_window.win_width,
						     game_window.win_height,
						     false,
						     true);
    frameTextureDataToGPU(ftexture_msaa_p);
    FrameTexture* ftexture_non_msaa_p = new FrameTexture(game_window.win_width,
	                                                 game_window.win_height,
	                                                 false,
							 false);
    frameTextureDataToGPU(ftexture_non_msaa_p);
    // Create Debug Grid Object
    DebugGrid* grid_p = new DebugGrid(active_entities_p->roomgrids[br_id].current_scale,
				      MAX_WIDTH + 1,
				      MAX_LENGTH + 1,
				      Vec3F(-active_entities_p->roomgrids[br_id].current_scale * 0.5f,
					    -0.5f,
					    -active_entities_p->roomgrids[br_id].current_scale * 0.5f));

    ///////////////////
    // Test Entities //
    ///////////////////
    
    // Blocks
    for(int x = 0; x < MAX_WIDTH; x++)
    {
	for(int z = 0; z < MAX_LENGTH; z++)
	{
	    activeEntitiesCreateEntity(*active_entities_p,
				       roomgrid_lookup,
				       ROOMGRID_A,
				       Vec3F((float)x, 0.0f, (float)z),
				       BLOCK);	    
	}
    } 
    
    activeEntitiesCreateEntity(*active_entities_p,
			       roomgrid_lookup,
			       ROOMGRID_A,
			       Vec3F(4.0f, 1.0f, 4.0f),
			       BLOCK);
    activeEntitiesCreateEntity(*active_entities_p,
			       roomgrid_lookup,
			       ROOMGRID_A,
			       Vec3F(9.0f, 1.0f, 9.0f),
			       BLOCK);

    // Special Block
    activeEntitiesCreateEntity(*active_entities_p,
			       roomgrid_lookup,
			       ROOMGRID_A,
			       Vec3F(MAX_WIDTH - 2, 1.0f, MAX_LENGTH - 2),
			       SPECIAL_BLOCK);
    activeEntitiesCreateEntity(*active_entities_p,
			       roomgrid_lookup,
			       ROOMGRID_A,
			       Vec3F(1.0f, 1.0f, MAX_LENGTH - 2), SPECIAL_BLOCK);
    activeEntitiesCreateEntity(*active_entities_p,
			       roomgrid_lookup,
			       ROOMGRID_A,
			       Vec3F(MAX_WIDTH - 2, 1.0f, 1.0f),
			       SPECIAL_BLOCK);
    
    // Player
    activeEntitiesCreateEntity(*active_entities_p,
			       roomgrid_lookup,
			       ROOMGRID_A,
			       Vec3F(0.0f, 1.0f, 1.0f),
			       PLAYER);

    // DirLight
    Vec3F dirlight_target = roomgrid_lookup.roomgrid_pointers[ROOMGRID_A]->center;
    Vec3F dirlight_offset = Vec3F(11.0f, 10.0f, -11.0f);
    int dir_light_id = activeEntitiesCreateEntity(*active_entities_p,
						  roomgrid_lookup,
						  ROOMGRID_A,
						  dirlight_target + dirlight_offset,
						  DIR_LIGHT);
    active_entities_p->dir_lights[dir_light_id].target = dirlight_target;
    active_entities_p->dir_lights[dir_light_id].offset = dirlight_offset;
    active_entities_p->dir_lights[dir_light_id].dir = (dirlight_target - (dirlight_target + dirlight_offset));

    // Camera
    int cam_id = activeEntitiesCreateEntity(*active_entities_p,
					    roomgrid_lookup,
					    ROOMGRID_A,
					    (active_entities_p->roomgrids[br_id].center +
					     Vec3F(11.0f, 11.0f, 11.0f)),
					    CAMERA);
    active_entities_p->cameras[cam_id].target = roomgrid_lookup.roomgrid_pointers[ROOMGRID_A]->center;
    
    ///////////////
    // Game Loop //
    ///////////////
    
    while(!game_window.close)
    {	
        gameUpdate(test_soundStream_p,
		   cam_id,
		   dir_light_id);

	gameRender(depth_ftexture_p,
		   ftexture_msaa_p,
		   ftexture_non_msaa_p,
		   roomgrid_lookup,
		   grid_p,
		   cam_id,
		   dir_light_id);

	gameUpdateInputs();
	
	// Close condition
	if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ESC] == KEY_DOWN) { game_window.close = true;}
    }

    /////////////
    // Cleanup //
    /////////////
    platformFreeWindow(game_window);
    
    delete active_entities_p;
    delete grid_p;
    delete test_soundStream_p;
    delete depth_ftexture_p;
    delete ftexture_msaa_p;
    delete ftexture_non_msaa_p;
    
    return 0;
}

//////////////////////////
// Function Definitions //
//////////////////////////

void gameInit(uint width, uint height)
{
    platformInitAPIs(game_window, width, height);
    gameUpdateInputs();
}

void gameUpdatePlayer()
{
    for(uint i = 0; i < active_entities_p->count; i++)
    {
	if(active_entities_p->entity_templates.table[active_entities_p->types[i]][COMPONENT_PLAYER])
	{
	    // Current and target positions
	    Vec3F cur_grid_pos = active_entities_p->grid_positions[i].position;
	    Vec3F new_grid_pos = cur_grid_pos;

	    // Set target position based on input
	    if(active_entities_p->states[i].input_cooldown == 0)
	    {
		// Down
		if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_DOWN] == KEY_DOWN)
		{
		    new_grid_pos = Vec3F(cur_grid_pos.x,
					 cur_grid_pos.y,
					 cur_grid_pos.z + 1);
		    active_entities_p->states[i].input_cooldown = INPUT_COOLDOWN_DUR;
		}
		// Up
		else if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_UP] == KEY_DOWN)
		{
		    new_grid_pos = Vec3F(cur_grid_pos.x,
					 cur_grid_pos.y,
					 cur_grid_pos.z - 1);
		    active_entities_p->states[i].input_cooldown = INPUT_COOLDOWN_DUR;
		}
		// Left
		else if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_LEFT] == KEY_DOWN)
		{
		    new_grid_pos = Vec3F(cur_grid_pos.x - 1,
					 cur_grid_pos.y,
					 cur_grid_pos.z);
		    active_entities_p->states[i].input_cooldown = INPUT_COOLDOWN_DUR;
		}
		// Right
		else if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_RIGHT] == KEY_DOWN)
		{
		    new_grid_pos = Vec3F(cur_grid_pos.x + 1,
					 cur_grid_pos.y,
					 cur_grid_pos.z);
		    active_entities_p->states[i].input_cooldown = INPUT_COOLDOWN_DUR;
		}
	    }

	    // Attempt to move player and subsequent entities based on target
	    int roomgrid_id = active_entities_p->grid_positions[i].roomgrid_owner_id;
	    if(roomgrid_id > -1)
	    {
		RoomGrid* grid_p = roomgrid_lookup.roomgrid_pointers[roomgrid_id];
		gameMoveEntitiesOnGrid(*grid_p, cur_grid_pos, new_grid_pos);
	    }
	}
    }
}

void gameUpdateAI()
{
    for(uint i = 0; i < active_entities_p->count; i++)
    {
	if(active_entities_p->entity_templates.table[active_entities_p->types[i]][COMPONENT_AI])
	{
	    // If walk, then walk
	    if(active_entities_p->ai[i].next_move == MOVE_WALK)
	    {
		int new_x = (rand() % 3) - 1; // x will be -1, 0 or 1
		int new_z = ((rand() % 3) - 1) * (new_x == 0);
		Vec3F move_dir = Vec3F((float)new_x, 0.0f, (float)new_z);
		Vec3F cur_pos = active_entities_p->grid_positions[i].position;

		// Assumes Entity with AI component ALSO has a GridPosition component
		int roomgrid_id = active_entities_p->grid_positions[i].roomgrid_owner_id;
		if(roomgrid_id > -1)
		{
		    RoomGrid* grid_p = roomgrid_lookup.roomgrid_pointers[roomgrid_id];
		    gameMoveEntitiesOnGrid(*grid_p, cur_pos, cur_pos + move_dir);
		}
	    }
	}
    }
}

void gameUpdateStates()
{
    for(uint i = 0; i < active_entities_p->count; i++)
    {
	if(active_entities_p->entity_templates.table[active_entities_p->types[i]][COMPONENT_STATE])
	{
	    active_entities_p->states[i].input_cooldown -= 1;
	    active_entities_p->states[i].input_cooldown = (int)clamp((float)active_entities_p->states[i].input_cooldown,
						           0, INPUT_COOLDOWN_DUR);
	}
    }
}

void gameUpdateTransforms()
{
    for(uint i = 0; i < active_entities_p->count; i++)
    {
	if(active_entities_p->entity_templates.table[active_entities_p->types[i]][COMPONENT_GRID_POSITION])
	{
	    int roomgrid_id = active_entities_p->grid_positions[i].roomgrid_owner_id;
	    if(roomgrid_id > -1)
	    {
		RoomGrid* grid_p = roomgrid_lookup.roomgrid_pointers[roomgrid_id];
		active_entities_p->transforms[i].position = (active_entities_p->grid_positions[i].position *
							     grid_p->current_scale);
	    }
	}
    }
}

uint gameUpdateCameras()
{
    uint entity_id = 0;
    for(uint i = 0; i < active_entities_p->count; i++)
    {
	if(active_entities_p->entity_templates.table[active_entities_p->types[i]][COMPONENT_CAMERA])
	{
	    // Set ID to Render Camera
	    if(active_entities_p->cameras[i].is_selected) { entity_id = i; }
	}
    }

    return entity_id;
}

uint gameUpdateDirLights(float time)
{
    uint entity_id = 0;
    
    for(uint i = 0; i < active_entities_p->count; i++)
    {
	if(active_entities_p->entity_templates.table[active_entities_p->types[i]][COMPONENT_DIR_LIGHT])
	{
	    // Get ID for use by later functions in the loop
	    entity_id = i;
	    
	    // Update Position & Direction
	    Vec3F offset = active_entities_p->dir_lights[i].offset;
	    float rotate_speed = active_entities_p->dir_lights[i].speed;
	    if(rotate_speed)
	    {
	        active_entities_p->transforms[i].position = (active_entities_p->dir_lights[i].target +
							     Vec3F(offset.x * sin(time),
								   offset.y,
								   offset.z * cos(time)));
	        active_entities_p->dir_lights[i].dir = (active_entities_p->dir_lights[i].target -
							active_entities_p->transforms[i].position);
	    }
	}
    }

    return entity_id;
}

void gameUpdateRoomGrids()
{
    for(uint i = 0; i < active_entities_p->count; i++)
    {
	if(active_entities_p->entity_templates.table[active_entities_p->types[i]][COMPONENT_ROOM_GRID])
	{
	    RoomGrid& rg = active_entities_p->roomgrids[i];

	    if(rg.cooldown) {rg.cooldown -= 1;}
	    if(rg.t < 1.0f) {rg.t += 0.01f;}
    
	    rg.current_scale = lerp(rg.current_scale, rg.target_scale, rg.t);
	    
	    if(rg.cooldown == 0)
	    {
		if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_W] == KEY_DOWN)
		{
		    rg.target_scale *= MAX_WIDTH;
		    rg.t = 0.01f;
		    rg.cooldown = 10;
		}
		if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_S] == KEY_DOWN)
		{
		    rg.target_scale /= MAX_WIDTH;
		    rg.t = 0.01f;
		    rg.cooldown = 10;
		}
	    }
	}
    }
}

int gameUpdate(SoundStream* sound_stream_p,
	       int& cam_id,
	       int& dir_light_id)
{
    ////////////
    // Update //
    ////////////

    // Update States
    gameUpdateStates();
    
    // Update Player
    gameUpdatePlayer();

    // Update AI
    gameUpdateAI();

    // Update Room Grids
    gameUpdateRoomGrids();
    
    // Update Transforms
    gameUpdateTransforms();
    
    // Update soundstream
    soundStreamUpdate(sound_stream_p);
    
    // Update Cameras
    cam_id = gameUpdateCameras();

    // Update Lights - TODO: store an array of all lights
    dir_light_id = gameUpdateDirLights((float)platformGetTime());
   
    // Remove Inactive Entities - Must be run after all other entity updates
    activeEntitiesRemoveInactives(*active_entities_p, roomgrid_lookup);

    return 1;
}

int gameRender(FrameTexture* depth_ftexture_p,
	       FrameTexture* ftexture_msaa_p,
	       FrameTexture* ftexture_non_msaa_p,
	       const RoomGridLookup& roomgrid_lookup,
	       DebugGrid* grid_p,
	       uint cam_id,
	       uint dir_light_id)
{
    // Render Pass 1 - Shadow Map
    platformRenderShadowMapToBuffer(*active_entities_p,
				    *depth_ftexture_p,
				    roomgrid_lookup,
				    asset_manager,
				    game_window,
				    dir_light_id);
    
    // Render Pass 2 -  Entities
    platformRenderEntitiesToBuffer(*active_entities_p,
				   *ftexture_msaa_p,
				   *depth_ftexture_p,
				   roomgrid_lookup,
				   game_window,
				   asset_manager,
				   dir_light_id,
				   cam_id);
    
    // Render Pass 3 - Debug
    platformRenderDebugElementsToBuffer(game_window,
					asset_manager,
					active_entities_p->transforms[cam_id].position,
				        active_entities_p->cameras[cam_id].target,
					grid_p);


    // Render Pass 4 - Post Processing
    platformBlitBufferToBuffer(*ftexture_msaa_p,
			       *ftexture_non_msaa_p,
			       0,
			       0,
			       ftexture_msaa_p->width,
			       ftexture_msaa_p->height,
			       0,
			       0,
			       ftexture_non_msaa_p->width,
			       ftexture_non_msaa_p->height);
    platformRenderPP(asset_manager, *ftexture_non_msaa_p);

    //////////////////
    // Swap buffers //
    //////////////////
    platformSwapBuffers(game_window);

    return 1;
}

void gameUpdateInputs()
{

    // Copy frame inputs to the prior frame inputs to make room for new inputs
    for(int i = TOTAL_FRAMES; i > 1; i--)
    {
	for(int j = 0; j < TOTAL_KEYS; j++)
	{
	    input_manager.inputs_on_frame[i - 1][j] = input_manager.inputs_on_frame[i - 2][j];
	}
    }
    
    // Get inputs received this frame
    platformGetInputsThisFrame(input_manager, game_window);
}

//////////////////////
// Helper functions //
//////////////////////

int gameMoveEntitiesOnGrid(RoomGrid& grid, Vec3F cur_grid_pos, Vec3F new_grid_pos)
{
    // Recursive function
    // Returns 1 on success, 0 on failure

    // Check that target position is not current position
    if(new_grid_pos == cur_grid_pos)
    {
	return 0;
    }
    
    // Check that target position is valid (within the grid)
    if(new_grid_pos.x < 0.0f || new_grid_pos.x >= MAX_WIDTH ||
       new_grid_pos.y < 0.0f || new_grid_pos.y >= MAX_HEIGHT ||
       new_grid_pos.z < 0.0f || new_grid_pos.z >= MAX_LENGTH)
    {
	return 0;
    }
    
    // Get entity ID for moving later. If -1, this call is invalid and should fail. 
    int entity_id = grid.grid[(int)cur_grid_pos.x][(int)cur_grid_pos.y][(int)cur_grid_pos.z];
    if(entity_id <= -1) {return 0;}
    
    // Check neighbor and set entity destination accordingly
    int neighbor_id = grid.grid[(int)new_grid_pos.x][(int)new_grid_pos.y][(int)new_grid_pos.z];
    if(neighbor_id > -1 && !active_entities_p->states[neighbor_id].inactive) // will early out if ID is invalid
    {
	// If target destination contains entity with collision, move fails
	uint neighbor_type = active_entities_p->types[neighbor_id];
	if(active_entities_p->entity_templates.table[neighbor_type][COMPONENT_COLLISION])
	{
	    return 0;
	}

	// If target destination contains a pushable entity, try to push it.
	// If push fails, move fails. 
	if(active_entities_p->entity_templates.table[neighbor_type][COMPONENT_PUSHABLE])
	{
	    Vec3F move_dir       = new_grid_pos - cur_grid_pos;
	    Vec3F n_cur_grid_pos = active_entities_p->grid_positions[neighbor_id].position;
	    Vec3F n_new_grid_pos = n_cur_grid_pos + move_dir;
	    if(!gameMoveEntitiesOnGrid(grid, n_cur_grid_pos, n_new_grid_pos)) {return 0;}
	}
    }

    // If none of the fail conditions were met, safe to move the entity
    roomGridRemoveEntity(grid, cur_grid_pos);
    roomGridSetEntity(grid, new_grid_pos, entity_id);
    active_entities_p->grid_positions[entity_id].position = new_grid_pos;
    return 1;
}
