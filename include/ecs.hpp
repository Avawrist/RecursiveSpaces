// =====================================================
// Title: component.hpp
// Description: The header file for component structs
// =====================================================

#ifndef COMPONENT_H
#define COMPONENT_H

// Utility Libs
#include "utility.hpp"
#include "mdcla.hpp"

// C++ Utility Lib
#include <bitset>
#include <cstring>

//////////////////
// Entity Types //
//////////////////

typedef enum EntityType
{
    TEST               = 0,
    CHEST              = 1,
    GRID               = 2,
    TOTAL_ENTITY_TYPES = 3
} EntityType;

/////////////////////
// Component Masks //
/////////////////////

// TODO: Account for larger than 32 bit values
typedef enum Component
{
    COMPONENT_NONE      = 0,
    COMPONENT_TRANSFORM = 1,
    COMPONENT_RENDER    = 1 << 1,
    COMPONENT_SFX       = 1 << 2
} Component;

//////////////////////
// Entity Templates //
//////////////////////

/* typedef struct EntityTemplates
{
    look up type, get component mask
}
loadMasksFromFile();
 */
   
///////////////////////
// Component Structs //
///////////////////////

typedef struct Transform
{
    Vec3F position;
    Vec3F rotation;
    float x_scale;
    float y_scale;
    float z_scale;
    Transform();
} Transform;

////////////////////////////////
// Struct of Component Arrays //
////////////////////////////////

#define MAX_COMPONENTS 128
#define MAX_ENTITIES   10000
  
typedef struct ActiveEntities
{
    std::bitset<MAX_COMPONENTS> mask[MAX_ENTITIES];
    uint                        type[MAX_ENTITIES];
    Transform                   transform[MAX_ENTITIES];
} ActiveEntities;

/////////////////////////
// Function Prototypes //
/////////////////////////

// ActiveEntities Function Prototypes
int activeEntitiesCreateEntity(ActiveEntities& entities, Vec3F origin, uint type,
			       void* sound_interface_p);

void activeEntitiesRemoveEntity(ActiveEntities& entities, int entity_ID);

// Component Function Prototypes
Mat4F transformGetModel(Transform& transform);

#endif
