// ================================================
// Title: component.cpp
// Description: The source file component structs
// ================================================

#include "component.hpp"

////////////////////////////////
// Struct Component Transform //
////////////////////////////////

Transform::Transform()
{
    scale = 0.0f;
}

/////////////////////////////
// Struct Component Render //
/////////////////////////////

Render::Render()
{
    mesh_count       = 1;
    has_diffuse_map  = true;
    has_specular_map = false;
    has_normal_map   = false;
}

////////////////////////////////
// Struct of Component Arrays //
////////////////////////////////

int activeEntitiesCreateEntity(ActiveEntities& entities, Vec3F origin, uint type)
{
    // Returns entity ID on success, -1 on failure
    
    _assert(type >= 0 && type < TOTAL_ENTITY_TYPES);

    // Parse all entities and find next open slot in array:
    for(uint i = 0; i < MAX_ENTITIES; i++)
    {
	if(entities.mask[i] == COMPONENT_NONE)
	{
	    // TODO: Get entity mask using type, record mask in entities (line below is temp)
	    entities.mask[i] = COMPONENT_TRANSFORM | COMPONENT_RENDER; 
	    // Record type in entities
	    entities.type[i] = type;
	    // Set transform position with origin
	    if((entities.mask[i] & std::bitset<MAX_COMPONENTS>(COMPONENT_TRANSFORM)) == COMPONENT_TRANSFORM)
	    {
		entities.transform[i].position = origin;
	    }
	    
	    return i;
	}
    }

    return -1;
}

void activeEntitiesRemoveEntity(ActiveEntities& entities, int entity_ID)
{
    _assert(entity_ID >= 0 && entity_ID < MAX_ENTITIES);
    
    // Remove components from entity, system knows the entity is free to reassign/overwrite
    entities.mask[entity_ID] = COMPONENT_NONE;
}

////////////////////////////////
// Component Update Functions //
////////////////////////////////

uint activeEntitiesRender(ActiveEntities& entities, uint entity_ID, AssetManager& asset_manager,
			  ShaderManager& shader_manager, Camera& camera, GameWindow& game_window,
			  FrameTexture& framebuffer)
{
    _assert(entity_ID >= 0 && entity_ID < MAX_ENTITIES);

    // returns 1 on success, 0 on failure

    // 0. Set GL state

    for(uint i = 0; i < MAX_ENTITIES; i++)
    {
	if((entities.mask[i] & std::bitset<MAX_COMPONENTS>(COMPONENT_RENDER)) == COMPONENT_RENDER)
	{
	    uint entity_type = entities.type[i];
	    // 1. Get meshes from asset manager using type
	    // 2. Get textures from asset manager using type
	    // 3. Get shader from shader manager using type
	    // 4. Update shader uniforms (will need to check shader type)
            // 5. bind shader
	    // 6. bind textures
	    // 7. bind VAO from mesh
	    // 8. draw
	}
    }

    return 1;
}
