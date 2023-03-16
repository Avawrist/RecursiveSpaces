// =====================================================
// Title: ecs.hpp
// Description: The header file for component structs
// =====================================================

#ifndef ECS_H
#define ECS_H

// Utility Libs
#include "utility.hpp"
#include "mdcla.hpp"

// C++ Utility Lib
#include <cstring>

//////////////////
// Entity Types //
//////////////////

typedef enum EntityType
{
    NONE = 0,
    CHEST,
    GRID,
    CAMERA,     // TODO: Remove as types are developed
    DIR_LIGHT,  // TODO: Remove as types are developed
    TOTAL_ENTITY_TYPES
} EntityType;

/////////////////////
// Component Masks //
/////////////////////

// TODO: Account for larger than 32 bit values
typedef enum Component
{
    COMPONENT_TRANSFORM = 0,
    COMPONENT_RENDER,
    COMPONENT_SFX,
    COMPONENT_CAMERA,
    COMPONENT_DIR_LIGHT,
    TOTAL_COMPONENT_TYPES
} Component;

//////////////////////
// Entity Templates //
//////////////////////

typedef struct EntityTemplates
{
    uint table[TOTAL_ENTITY_TYPES][TOTAL_COMPONENT_TYPES];
    EntityTemplates();
} EntityTemplates;

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

typedef struct DirLight
{
    Vec3F color;
    Vec3F dir;
    float ambient_strength;
    DirLight();
} DirLight;

////////////////////////////////
// Struct of Component Arrays //
////////////////////////////////

#define MAX_COMPONENTS 128
#define MAX_ENTITIES   10000
  
typedef struct ActiveEntities
{
    EntityTemplates entity_templates;
    uint       type[MAX_ENTITIES];
    Transform transform[MAX_ENTITIES];
    Camera    camera[MAX_ENTITIES];
    DirLight  dir_light[MAX_ENTITIES];
    ActiveEntities();
} ActiveEntities;

/////////////////////////
// Function Prototypes //
/////////////////////////

// ActiveEntities Function Prototypes
int activeEntitiesCreateEntity(ActiveEntities& entities, Vec3F origin, uint entity_type);

void activeEntitiesRemoveEntity(ActiveEntities& entities, int entity_ID);

// Transform Function Prototypes
Mat4F transformGetModel(Transform& transform);

// Camera Function Prototypes
void  cameraOffsetAngles(Camera& cam, float o_yaw, float o_pitch);

Mat4F cameraGetView(const Camera& cam, Vec3F cam_pos);

Mat4F cameraGetPerspective(const Camera& cam, float ar);

Mat4F cameraGetOrthographic(const Camera& cam, int win_width, int win_height);

#endif
