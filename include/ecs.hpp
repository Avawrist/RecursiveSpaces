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
    CAMERA             = 3,
    TOTAL_ENTITY_TYPES = 4
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
    COMPONENT_SFX       = 1 << 2,
    COMPONENT_CAMERA    = 1 << 3,
    COMPONENT_DIR_LIGHT = 1 << 4
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

typedef struct Camera
{
    bool  is_selected;
    float pitch;
    float yaw;
    float n;
    float f;
    float fov;
    float ar;
    Camera();
} Camera;

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
    Camera                      camera[MAX_ENTITIES];
} ActiveEntities;

/////////////////////////
// Function Prototypes //
/////////////////////////

// ActiveEntities Function Prototypes
int activeEntitiesCreateChest(ActiveEntities& entities, Vec3F origin, uint type);

int activeEntitiesCreateCamera(ActiveEntities& entities, Vec3F origin, uint type);

void activeEntitiesRemoveEntity(ActiveEntities& entities, int entity_ID);

// Transform Function Prototypes
Mat4F transformGetModel(Transform& transform);

// Camera Function Prototypes
void  cameraOffsetAngles(Camera& cam, float o_yaw, float o_pitch);

Mat4F cameraGetView(const Camera& cam, Vec3F cam_pos);

Mat4F cameraGetPerspective(const Camera& cam, float ar);

Mat4F cameraGetOrthographic(const Camera& cam, int win_width, int win_height);

#endif
