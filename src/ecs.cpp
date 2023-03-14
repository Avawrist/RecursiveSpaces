// ================================================
// Title: ecs.cpp
// Description: The source file component structs
// ================================================

#include "ecs.hpp"

////////////////////////////////
// Struct Component Transform //
////////////////////////////////

Transform::Transform()
{
    x_scale = 1.0f;
    y_scale = 1.0f;
    z_scale = 1.0f;
}

// ActiveEntities Functions

int activeEntitiesCreateEntity(ActiveEntities& entities, Vec3F origin, uint type,
			       void* sound_interface_p)
{
    // Returns entity ID on success, -1 on failure
    
    _assert(type >= 0 && type < TOTAL_ENTITY_TYPES);

    // Parse all entities and find next open slot in array:
    for(uint i = 0; i < MAX_ENTITIES; i++)
    {
	if(entities.mask[i] == COMPONENT_NONE)
	{
	    // TODO: Get entity mask using type, record mask in entities (line below is temp)
	    entities.mask[i] = COMPONENT_TRANSFORM | COMPONENT_RENDER | COMPONENT_SFX; 
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

// Component Functions
Mat4F transformGetModel(Transform& transform)
{
    Mat4F model = Mat4F(transform.x_scale, 0.0f, 0.0f, 0.0f,
	                0.0f, transform.y_scale, 0.0f, 0.0f,
	                0.0f, 0.0f, transform.z_scale, 0.0f,
	                transform.position.x, transform.position.y, transform.position.z, 1.0f);
    return model;
}

