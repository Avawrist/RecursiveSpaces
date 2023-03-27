// =====================================================
// Title: ecs.hpp
// Description: The header file for component structs
// =====================================================

#ifndef ECS_H
#define ECS_H

// Utility Libs
#include "utility.hpp"
#include "mdcla.hpp"

// Win libs
#include <windows.h>

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
    COMPONENT_GRID_POSITION,
    COMPONENT_STATE,
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

/////////////////////////
// Component Transform //
/////////////////////////

typedef struct Transform
{
    Vec3F position;
    float x_scale;
    float y_scale;
    float z_scale;
    Transform();
    Transform(Vec3F _position);
} Transform;

//////////////////////
// Component Camera //
//////////////////////

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

////////////////////////
// Component DirLight //
////////////////////////

typedef struct DirLight
{
    Vec3F color;
    Vec3F dir;
    float ambient_strength;
    DirLight();
} DirLight;

////////////////////////////
// Component GridPosition //
////////////////////////////

typedef struct GridPosition
{
    Vec3F position;
    GridPosition();
    GridPosition(Vec3F _position);
} GridPosition;

/////////////////////
// Component State //
/////////////////////

typedef enum StateMeta
{
    INPUT_COOLDOWN_DUR = 10
} StateMeta;

typedef struct State
{
    int input_cooldown = 0;
} State;

////////////////////////////////
// Struct of Component Arrays //
////////////////////////////////

#define MAX_COMPONENTS 128
#define MAX_ENTITIES   10000
  
typedef struct ActiveEntities
{
    EntityTemplates entity_templates;
    uint         type[MAX_ENTITIES];
    Transform    transform[MAX_ENTITIES];
    Camera       camera[MAX_ENTITIES];
    DirLight     dir_light[MAX_ENTITIES];
    GridPosition grid_position[MAX_ENTITIES];
    State        state[MAX_ENTITIES];
    uint      count;
    ActiveEntities();
} ActiveEntities;

//////////////////////
// Struct LevelGrid //
//////////////////////

typedef enum GridMeasurements
{
    MAX_WIDTH  = 25,
    MAX_LENGTH = 25,
    MAX_HEIGHT = 4
} GridMeasurements;

typedef struct LevelGrid
{
    int grid[MAX_WIDTH][MAX_HEIGHT][MAX_LENGTH];
    float dimensions = 2.5f;
    LevelGrid();
} LevelGrid;

/////////////////////////
// Function Prototypes //
/////////////////////////

// ActiveEntities Function Prototypes
int activeEntitiesCreateEntity(ActiveEntities& entities, Vec3F origin, uint entity_type);

void activeEntitiesMarkInactive(ActiveEntities& entities, uint entity_ID);

void activeEntitiesRemoveInactives(ActiveEntities& entities, LevelGrid& level_grid);

// Transform Function Prototypes
Mat4F transformGetModel(Transform& transform);

// Camera Function Prototypes
void  cameraOffsetAngles(Camera& cam, float o_yaw, float o_pitch);

Mat4F cameraGetView(const Camera& cam, Vec3F cam_pos);

Mat4F cameraGetPerspective(const Camera& cam, float ar);

Mat4F cameraGetOrthographic(const Camera& cam, int win_width, int win_height);

// LevelGrid Function Prototypes
void levelGridSetEntity(LevelGrid& level_grid, ActiveEntities& entities, Vec3F pos, int entity_ID);

void levelGridRemoveEntity(LevelGrid& level_grid, Vec3F pos);

#endif
