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

typedef struct Render
{
    Mesh*    mesh_01_p   = NULL;
    Texture* texture_d_p = NULL;
    Texture* texture_n_p = NULL;
    Texture* texture_s_p = NULL;
    Shader*  shader_p    = NULL;
} Render;

typedef struct SFX
{
    Sound* sound_01_p = NULL;
    Sound* sound_02_p = NULL;
    Sound* sound_03_p = NULL;
} SFX;

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
    Render                      render[MAX_ENTITIES];
    SFX                         sfx[MAX_ENTITIES];
} ActiveEntities;

/////////////////////////
// Function Prototypes //
/////////////////////////

// ActiveEntities Function Prototypes

void activeEntitiesSetRenderPointers(AssetManager& asset_manager, ShaderManager& shader_manager,
                                     Render& render_comp, uint type);

void activeEntitiesSetSFXPointers(AssetManager& asset_manager, void* sound_interface_p,
                                     SFX& sound_comp, uint type);

int  activeEntitiesCreateEntity(ActiveEntities& entities, AssetManager& asset_manager,
	                        ShaderManager& shader_manager, Vec3F origin, uint type,
				void* sound_interface_p);

void activeEntitiesRemoveEntity(ActiveEntities& entities, int entity_ID);

// Component Function Prototypes
Mat4F transformGetModel(Transform& transform);

// Component Update Functions

void activeEntitiesRender(ActiveEntities& entities, GameWindow& game_window, FrameTexture& framebuffer);

#endif
