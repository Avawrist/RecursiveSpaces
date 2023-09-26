// ====================================================================================
// Title: game.cpp
// Description: The primary source file for an untitled game, containing the game loop.
// ====================================================================================

// Platform lib
#include "platform.hpp"

// C Libs
#include "cstdlib"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void gameUpdatePlayer(ActiveEntities& entities, Level& level, InputManager& input_manager);

void gameUpdateAI(ActiveEntities& entities, Level& level);

void gameUpdateDogStates(ActiveEntities& entities);

void gameUpdateTransforms(ActiveEntities& active_entities, LevelGrid& grid);

uint gameUpdateCameras(ActiveEntities& active_entities, GameWindow& game_window,
		       InputManager& input_manager);

int gameUpdateAndRender(SoundStream* sound_stream_p, GameWindow& game_window, InputManager& input_manager,
			ActiveEntities& active_entities, AssetManager& asset_manager,
			FrameTexture* depth_ftexture_p, FrameTexture* ftexture_p, Level& level,
			DebugGrid* grid_p);

void gameUpdateInputs(InputManager& im, GameWindow& game_window);

int gameMoveEntitiesOnGrid(LevelGrid& grid, ActiveEntities& entities,
			   Vec3F cur_grid_position, Vec3F new_grid_position);

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
    Level* level_p = new Level();
    
    /////////////////
    // Temp Assets //
    /////////////////
    
    // Initialize BGM
    SoundStream* test_soundStream_p = new SoundStream("..\\data\\assets\\bgm\\elephant.wav", sound_interface);
    // Initialize Pre-Process Framebuffer
    FrameTexture* depth_ftexture_p = new FrameTexture(1024, 1024, true);
    frameTextureDataToGPU(depth_ftexture_p, true);
    FrameTexture* ftexture_p = new FrameTexture(game_window.view_width, game_window.view_height, false);
    frameTextureDataToGPU(ftexture_p, false);
    // Create Debug Grid Object
    DebugGrid* grid_p = new DebugGrid(level_p->grid.dimensions,
				      MAX_WIDTH + 1,
				      MAX_LENGTH + 1,
				      Vec3F(-level_p->grid.dimensions * 0.5f, 0.0f,
					    -level_p->grid.dimensions * 0.5f));

    ///////////////////
    // Test Entities //
    ///////////////////
    
    // Blocks
    for(int x = 0; x < MAX_WIDTH; x++)
    {
	for(int z = 0; z < MAX_LENGTH; z++)
	{
	    activeEntitiesCreateEntity(*active_entities_p, level_p->grid,
				       Vec3F((float)x, 0.0f, (float)z), BLOCK);	    
	}
    }

    activeEntitiesCreateEntity(*active_entities_p, level_p->grid, Vec3F(4.0f, 1.0f, 4.0f),  BLOCK);
    activeEntitiesCreateEntity(*active_entities_p, level_p->grid, Vec3F(9.0f, 1.0f, 9.0f),  BLOCK);

    // Special Block
    activeEntitiesCreateEntity(*active_entities_p, level_p->grid, Vec3F(4.0f, 1.0f, 2.0f), SPECIAL_BLOCK);
    
    // Player
    activeEntitiesCreateEntity(*active_entities_p, level_p->grid, Vec3F(0.0f, 1.0f, 1.0f), PLAYER);
    
    // DirLight
    activeEntitiesCreateEntity(*active_entities_p, level_p->grid, Vec3F(6.0f, 6.0f, 6.0f), DIR_LIGHT);
    
    // Camera
    activeEntitiesCreateEntity(*active_entities_p, level_p->grid,
			       Vec3F(6.0f, 6.0f, 6.0f), CAMERA);
    
    ///////////////
    // Game Loop //
    ///////////////
    while(!game_window.close)
    {	
	gameUpdateAndRender(test_soundStream_p, game_window, input_manager, *active_entities_p,
			    asset_manager, depth_ftexture_p, ftexture_p, *level_p, grid_p);
        gameUpdateInputs(input_manager, game_window); // Store all inputs received this cycle
	
	// Close condition
	if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ESC] == KEY_DOWN) { game_window.close = true;}
    }

    /////////////
    // Cleanup //
    /////////////
    platformFreeWindow(game_window);
    
    // Delete pointers to structs on the heap
    delete level_p;
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

void gameUpdatePlayer(ActiveEntities& entities, Level& level, InputManager& input_manager)
{
    for(uint i = 0; i < entities.count; i++)
    {
	if(entities.entity_templates.table[entities.type[i]][COMPONENT_PLAYER])
	{
	    // Current and target positions
	    Vec3F cur_grid_pos = entities.grid_position[i].position;
	    Vec3F new_grid_pos = cur_grid_pos;

	    // Set target position based on input
	    if(entities.state[i].input_cooldown == 0)
	    {
		// Down
		if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_DOWN] == KEY_DOWN)
		{
		    new_grid_pos = Vec3F(cur_grid_pos.x,
					 cur_grid_pos.y,
					 cur_grid_pos.z + 1);
		    entities.state[i].input_cooldown = INPUT_COOLDOWN_DUR;
		    level.turn = TURN_AI;
		}
		// Up
		else if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_UP] == KEY_DOWN)
		{
		    new_grid_pos = Vec3F(cur_grid_pos.x,
					 cur_grid_pos.y,
					 cur_grid_pos.z - 1);
		    entities.state[i].input_cooldown = INPUT_COOLDOWN_DUR;
		    level.turn = TURN_AI;
		}
		// Left
		else if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_LEFT] == KEY_DOWN)
		{
		    new_grid_pos = Vec3F(cur_grid_pos.x - 1,
					 cur_grid_pos.y,
					 cur_grid_pos.z);
		    entities.state[i].input_cooldown = INPUT_COOLDOWN_DUR;
		    level.turn = TURN_AI;
		}
		// Right
		else if(input_manager.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_RIGHT] == KEY_DOWN)
		{
		    new_grid_pos = Vec3F(cur_grid_pos.x + 1,
					 cur_grid_pos.y,
					 cur_grid_pos.z);
		    entities.state[i].input_cooldown = INPUT_COOLDOWN_DUR;
		    level.turn = TURN_AI;
		}
	    }

	    // Attempt to move player and subsequent entities based on target
	    gameMoveEntitiesOnGrid(level.grid, entities, cur_grid_pos, new_grid_pos);
	}
    }
}

void gameUpdateAI(ActiveEntities& entities, Level& level)
{
    for(uint i = 0; i < entities.count; i++)
    {
	if(entities.entity_templates.table[entities.type[i]][COMPONENT_AI])
	{
	    // If walk, then walk
	    if(entities.ai[i].next_move == MOVE_WALK)
	    {
		int new_x = (rand() % 3) - 1; // x will be -1, 0 or 1
		int new_z = ((rand() % 3) - 1) * (new_x == 0);
		Vec3F move_dir = Vec3F((float)new_x, 0.0f, (float)new_z);
		Vec3F cur_pos = entities.grid_position[i].position;
	    
		gameMoveEntitiesOnGrid(level.grid, entities, cur_pos, cur_pos + move_dir);
	    }

	    // If special move, use special move based on additional AI type
	    else if(entities.ai[i].next_move == MOVE_SPECIAL)
	    {
		if(entities.type[i] == SMALL_DOG)
		{
		    OutputDebugStringA("Performing small special move.\n");
		    // Determine target position
                    /*
		    Vec3F cur_pos    = entities.grid_position[i].position;
		    Vec3F target_pos = levelGridFindNearestType(level.grid, entities, cur_pos, SPECIAL_BLOCK);
		    Vec3F move_dir   = aStarFindPath(level.grid, cur_pos, target_pos);
		    gameMoveEntitiesOnGrid(level.grid, entities, cur_pos, cur_pos + move_dir); */
		}

		else if(entities.type[i] == MEDIUM_DOG)
		{
		    OutputDebugStringA("Performing medium special move.\n");
		}

		else if(entities.type[i] == LARGE_DOG)
		{
		    // Perform dig special move
		    Vec3F center_pos_to_dirty = entities.grid_position[i].position - (entities.ai[i].face_dir * 2.0f);
		    for(int x = -1; x < 2; x++)
		    {
			for(int z = -1; z < 2; z++)
			{
			    Vec3F pos_to_dirty = center_pos_to_dirty + Vec3F((float)x, 0.0f, (float)z);
			    int neighbor_id = levelGridGetEntity(level.grid, pos_to_dirty);
			    if(neighbor_id > -1)
			    {
				uint type = entities.type[neighbor_id];
				if(entities.entity_templates.table[type][COMPONENT_DOG_STATE])
				{
				    entities.dog_state[neighbor_id].filth_level = FILTHY;
				}
			    }
			}
		    }
		}
	    }

	    // Set next move
	    if(entities.type[i] == LARGE_DOG || entities.type[i] == SMALL_DOG)
	    {
		entities.ai[i].next_move = MOVE_SPECIAL;
	    }
	    else
	    {
		entities.ai[i].next_move = rand() % 2;
	    }
	}
    }
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

void gameUpdateDogStates(ActiveEntities& entities)
{
    for(uint i = 0; i < entities.count; i++)
    {
	if(entities.entity_templates.table[entities.type[i]][COMPONENT_DOG_STATE])
	{
	    // TODO: load this off of a template eventually
	    if(entities.dog_state[i].first)
	    {
		if(entities.type[i] == SMALL_DOG) {entities.dog_state[i].speed_level = 3;}
		else if(entities.type[i] == MEDIUM_DOG) {entities.dog_state[i].speed_level = 2;}
		else if(entities.type[i] == LARGE_DOG) {entities.dog_state[i].speed_level = 1;}
		entities.dog_state[i].first = 0;
	    }

	    // If dirty, remove from floor
	    if(entities.dog_state[i].filth_level == FILTHY && entities.type[i] != PLAYER)
	    {
		activeEntitiesMarkInactive(entities, i);
	    }
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
			FrameTexture* depth_ftexture_p, FrameTexture* ftexture_p, Level& level,
			DebugGrid* grid_p)
{
    ////////////
    // Update //
    ////////////

    // Update States
    gameUpdateStates(active_entities);

    // Update Dog States
    gameUpdateDogStates(active_entities);
    
    // Update Player
    if(level.turn == TURN_PLAYER)
    {
	gameUpdatePlayer(active_entities, level, input_manager);
    }

    // Update AI
    if (level.turn == TURN_AI)
    {
	gameUpdateAI(active_entities, level);
	level.turn = TURN_PLAYER;
    }
    
    // Update Transforms
    gameUpdateTransforms(active_entities, level.grid);
    
    // Update soundstream
    soundStreamUpdate(sound_stream_p);
    
    // Update Cameras
    uint cam_id = gameUpdateCameras(active_entities, game_window, input_manager);

    // Update Lights - TODO: Should eventually store an array of all lights
    uint dir_light_id = gameUpdateDirLights(active_entities, game_window);
   
    // Remove Inactive Entities - Must be run after all other entity updates
    activeEntitiesRemoveInactives(active_entities, level.grid);

    ///////////////////////////////
    // Render Pass 0 - Depth Map //
    ///////////////////////////////
/*
    // -> Move everything into platformSetRenderStateShadowMap(),
    //    platformPrepShaderShadowMap(),
    //    platformRenderShadowMap()

    // platformSetRenderStateShadowMap()
    glViewport(0, 0, depth_ftexture_p->width, depth_ftexture_p->height);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_ftexture_p->fbo);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // platformPrepShaderShadowMap()
    Shader* shadowmap_shader_p = (Shader*)assetManagerGetShaderP(asset_manager, SHADOWMAP);
    glUseProgram(shadowmap_shader_p->program_id);
    Mat4F view = lookAt(active_entities.transform[dir_light_id].position,
			Vec3F(0.0f, 0.0f, 0.0f),
			Vec3F(0.0f, 1.0f, 0.0f));
    //Mat4F projection = cameraGetOrthographic(active_entities.camera[cam_id],
    //					     game_window.win_width,
    //					     game_window.win_height);
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.05f, 10.0f);
    shaderAddMat4Uniform(shadowmap_shader_p, "view", view.getPointer());
    shaderAddMat4Uniform(shadowmap_shader_p, "projection", glm::value_ptr(projection));


    // Render renderable entities
    for(uint i = 0; i < active_entities.count; i++)
    {
	if(active_entities.entity_templates.table[active_entities.type[i]][COMPONENT_RENDER] &&
	   active_entities.entity_templates.table[active_entities.type[i]][COMPONENT_TRANSFORM])
	{
	    Mat4F model = transformGetModel(active_entities.transform[i]);
	    // platformFillShadowMap()
	    shaderAddMat4Uniform(shadowmap_shader_p, "model", model.getPointer());
	    Mesh* mesh_01_p  = (Mesh*)assetManagerGetAssetP(asset_manager, active_entities.type[i],
							    MESH01, 0);
	    glBindVertexArray(mesh_01_p->vao);
	    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh_01_p->data.size());
	}
    }

    // PlatformRenderShadowMap()
    Shader* sm_debug_shader_p = (Shader*)assetManagerGetShaderP(asset_manager, SMDEBUG);
    glViewport(0, 0, game_window.win_width, game_window.win_height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(sm_debug_shader_p->program_id);
    shaderAddIntUniform(sm_debug_shader_p, "depth_map", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depth_ftexture_p->depth_text_id);
    glBindVertexArray(depth_ftexture_p->quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);    
*/
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

//////////////////////
// Helper functions //
//////////////////////

int gameMoveEntitiesOnGrid(LevelGrid& grid, ActiveEntities& entities, Vec3F cur_grid_pos, Vec3F new_grid_pos)
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
    if(neighbor_id > -1 && !entities.state[neighbor_id].inactive) // will early out if ID is invalid
    {
	// If target destination contains entity with collision, move fails
	uint neighbor_type = entities.type[neighbor_id];
	if(entities.entity_templates.table[neighbor_type][COMPONENT_COLLISION])
	{
	    return 0;
	}

	// If target destination contains a pushable entity, try to push it.
	// If push fails, move fails. 
	if(entities.entity_templates.table[neighbor_type][COMPONENT_PUSHABLE])
	{
	    Vec3F move_dir       = new_grid_pos - cur_grid_pos;
	    Vec3F n_cur_grid_pos = entities.grid_position[neighbor_id].position;
	    Vec3F n_new_grid_pos = n_cur_grid_pos + move_dir;
	    if(!gameMoveEntitiesOnGrid(grid, entities, n_cur_grid_pos, n_new_grid_pos)) {return 0;}
	}
    }

    // If none of the fail conditions were met, safe to move the entity
    levelGridRemoveEntity(grid, cur_grid_pos);
    levelGridSetEntity(grid, entities, new_grid_pos, entity_id);
    entities.grid_position[entity_id].position = new_grid_pos;
    return 1;
}
