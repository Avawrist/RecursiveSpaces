// =====================================================
// Title: component.hpp
// Description: The header file for component structs
// =====================================================

#ifndef COMPONENT_H
#define COMPONENT_H

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"
#include "mdcla.hpp"
#include "camera.hpp"
#include "asset_manager.hpp"
#include "entity.hpp"

// C++ Utility Lib
#include <bitset>
#include <cstring>

/////////////////////
// Component Masks //
/////////////////////

// TODO: Account for larger than 32 bit values
typedef enum Component
{
    COMPONENT_NONE      = 0,
    COMPONENT_TRANSFORM = 1,
    COMPONENT_RENDER    = 1 << 1
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
    float scale;
    Transform();
} Transform;

typedef struct Render
{
    uint mesh_count;
    bool has_diffuse_map;
    bool has_specular_map;
    bool has_normal_map;
    Render();
} Render;

////////////////////////////////
// Struct of Component Arrays //
////////////////////////////////

#define MAX_COMPONENTS 128
#define MAX_ENTITIES   10000
  
typedef struct ActiveEntities
{
    std::bitset<MAX_COMPONENTS> mask[MAX_ENTITIES];
    uint             type[MAX_ENTITIES];
    Transform        transform[MAX_ENTITIES];
    Render           render[MAX_ENTITIES];
} ActiveEntities;
int  activeEntitiesCreateEntity(ActiveEntities& entities, Vec3F origin, uint type);
void activeEntitiesRemoveEntity(ActiveEntities& entities, int entity_ID); 

////////////////////////////////
// Component Update Functions //
////////////////////////////////

uint activeEntitiesRender(ActiveEntities& entities, uint entity_ID);

#endif
