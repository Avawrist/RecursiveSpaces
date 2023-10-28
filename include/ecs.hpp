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

// C/C++ Utility Lib
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <iostream>

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
    PLAYER,
    BLOCK,
    SPECIAL_BLOCK,
    BLOCK_ROOM,
    TOTAL_ENTITY_TYPES
} EntityType;

/////////////////////
// Component Types //
/////////////////////

typedef enum Component
{
    COMPONENT_TRANSFORM = 0,
    COMPONENT_RENDER,
    COMPONENT_SFX,
    COMPONENT_CAMERA,
    COMPONENT_DIR_LIGHT,
    COMPONENT_POINT_LIGHT,
    COMPONENT_GRID_POSITION,
    COMPONENT_STATE,
    COMPONENT_PLAYER,
    COMPONENT_COLLISION,
    COMPONENT_PUSHABLE,
    COMPONENT_AI,
    COMPONENT_ROOM_GRID,
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
    Vec3F target;
    Camera();
} Camera;

////////////////////////
// Component DirLight //
////////////////////////

typedef struct DirLight
{
    Vec3F color;
    Vec3F dir;
    Vec3F target;
    Vec3F offset;
    float speed;
    float ambient_strength;
    DirLight();
} DirLight;

//////////////////////////
// Component PointLight //
//////////////////////////

typedef struct PointLight
{
    Vec3F color;
    float ambient_strength;
    PointLight();
} PointLight;

////////////////////////////
// Component GridPosition //
////////////////////////////

typedef struct GridPosition
{
    Vec3F position;
    int roomgrid_owner_id = -1; 
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
    bool inactive; 
    int input_cooldown;
    State();
} State;

//////////////////
// Component AI //
//////////////////

typedef enum Moves
{
    MOVE_WALK = 0,
    MOVE_SPECIAL
} Moves;

typedef struct Node
{
    Vec3F grid_pos;
    uint g_cost;
    uint h_cost;
    uint f_cost;
    Node* parent_p;
    Node();
    Node(Vec3F start_pos, Vec3F _grid_pos, Vec3F target_pos, Node* _parent_p);
} Node;

typedef struct AI
{
    Vec3F face_dir;
    uint  next_move;
    AI();
} AI;

////////////////////////
// Component RoomGrid //
////////////////////////

typedef enum GridMeasurements
{
    RG_MAX_WIDTH  = 20,
    RG_MAX_LENGTH = 20,
    RG_MAX_HEIGHT = 20
} GridMeasurements;

typedef enum EntityCodes
{
    NO_ENTITY     = -1,
    INVALID_RANGE = -2
} EntityCodes;

typedef enum RoomGridCodes
{
    ROOMGRID_A = 0,
    ROOMGRID_B,
    ROOMGRID_C,
    ROOMGRID_D,
    ROOMGRID_E,
    TOTAL_ROOMGRIDS
} RoomGridCodes;

typedef struct RoomGrid
{
    int grid[RG_MAX_WIDTH][RG_MAX_HEIGHT][RG_MAX_LENGTH];
    float current_scale = 1.0f;
    float target_scale = 1.0f;
    float t = 1.0f;
    Vec3F center = Vec3F(RG_MAX_WIDTH * current_scale * 0.5f, 0.0f, RG_MAX_LENGTH * current_scale * 0.5f);
    uint cooldown = 0;
    int roomgrid_id = -1; 
    RoomGrid();
} RoomGrid;

typedef struct RoomGridLookup
{
    RoomGrid* roomgrid_pointers[TOTAL_ROOMGRIDS];
} RoomGridLookup;

////////////////////////////////
// Struct of Component Arrays //
////////////////////////////////

#define MAX_COMPONENTS 128
#define MAX_ENTITIES   10000
  
typedef struct ActiveEntities
{
    EntityTemplates entity_templates;
    uint         types[MAX_ENTITIES];
    Transform    transforms[MAX_ENTITIES];
    Camera       cameras[MAX_ENTITIES];
    DirLight     dir_lights[MAX_ENTITIES];
    PointLight   point_lights[MAX_ENTITIES];
    GridPosition grid_positions[MAX_ENTITIES];
    State        states[MAX_ENTITIES];
    AI           ai[MAX_ENTITIES];
    RoomGrid     roomgrids[MAX_ENTITIES];
    uint         count;
    ActiveEntities();
} ActiveEntities;

/////////////////////////
// Function Prototypes //
/////////////////////////

// ActiveEntities Function Prototypes
int activeEntitiesCreateEntity(ActiveEntities& entities,
			       const RoomGridLookup& roomgrid_lookup,
			       int room_grid_owner_id, 
			       Vec3F origin,
			       uint entity_type);

void activeEntitiesMarkInactive(ActiveEntities& entities, uint entity_ID);

void activeEntitiesRemoveInactives(ActiveEntities& entities, RoomGridLookup& roomgrid_lookup);

// Transform Function Prototypes
Mat4F transformGetModel(const Transform& transform);

// Camera Function Prototypes
void  cameraOffsetAngles(Camera& cam, float o_yaw, float o_pitch);

// RoomGrid Function Prototypes
int roomGridGetEntity(RoomGrid& room_grid, Vec3F pos);

void roomGridSetEntity(RoomGrid& room_grid, Vec3F pos, int entity_ID);

void roomGridRemoveEntity(RoomGrid& room_grid, Vec3F pos);

Vec3F roomGridFindNearestType(RoomGrid& room_grid, ActiveEntities& entities,
			       Vec3F cur_pos, uint target_type);

void roomGridLookupInit(RoomGridLookup& rgl);

// AI Function Prototypes
Vec3F aStarFindPath(RoomGrid& grid, Vec3F cur_grid_pos, Vec3F target_grid_pos);

#endif
