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
    uint         count;
    ActiveEntities();
} ActiveEntities;

//////////////////////
// Struct LevelGrid //
//////////////////////

typedef enum GridMeasurements
{
    MAX_WIDTH  = 15,
    MAX_LENGTH = 15,
    MAX_HEIGHT = 4
} GridMeasurements;

typedef enum EntityCodes
{
    NO_ENTITY     = -1,
    INVALID_RANGE = -2
} EntityCodes;

typedef struct LevelGrid
{
    int grid[MAX_WIDTH][MAX_HEIGHT][MAX_LENGTH];
    float unit_length = 1.0f;
    Vec3F center = Vec3F(MAX_WIDTH * unit_length * 0.5f, 0.0f, MAX_LENGTH * unit_length * 0.5f);
    LevelGrid();
} LevelGrid;

//////////////////
// Struct Level //
//////////////////

typedef struct Level
{
    LevelGrid grid;
} Level;

/////////////////////////
// Function Prototypes //
/////////////////////////

// ActiveEntities Function Prototypes
int activeEntitiesCreateEntity(ActiveEntities& entities, LevelGrid& level_grid,
			       Vec3F origin, uint entity_type);

void activeEntitiesMarkInactive(ActiveEntities& entities, uint entity_ID);

void activeEntitiesRemoveInactives(ActiveEntities& entities, LevelGrid& level_grid);

// Transform Function Prototypes
Mat4F transformGetModel(const Transform& transform);

// Camera Function Prototypes
void  cameraOffsetAngles(Camera& cam, float o_yaw, float o_pitch);

Mat4F cameraGetView(const Camera& cam, Vec3F cam_pos);

Mat4F cameraGetPerspective(const Camera& cam, float ar);

Mat4F cameraGetOrthographic(const Camera& cam, int win_width, int win_height);

// LevelGrid Function Prototypes
int levelGridGetEntity(LevelGrid& level_grid, Vec3F pos);

void levelGridSetEntity(LevelGrid& level_grid, Vec3F pos, int entity_ID);

void levelGridRemoveEntity(LevelGrid& level_grid, Vec3F pos);

Vec3F levelGridFindNearestType(LevelGrid& level_grid, ActiveEntities& entities,
			       Vec3F cur_pos, uint target_type);

// AI Function Prototypes
Vec3F aStarFindPath(LevelGrid& grid, Vec3F cur_grid_pos, Vec3F target_grid_pos);

#endif
