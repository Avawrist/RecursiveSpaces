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

/////////////////////
// Component Masks //
/////////////////////

typedef enum Component
{
    COMPONENT_NONE      = 0,
    COMPONENT_TRANSFORM = 1 << 0,
    COMPONENT_RENDER    = 1 << 1
} Component;

///////////////////////
// Component Structs //
///////////////////////

typedef struct Transform
{
    Vec3F position;
    Vec3F rotation;
    float scale; 
} Transform;

typedef struct Render
{
    uint appearance;
} Render;

////////////////////////////////
// Struct of Component Arrays //
////////////////////////////////

#define MAX_ENTITIES 200
  
typedef struct ActiveEntities
{
     int       mask[MAX_ENTITIES];
     Transform transform[MAX_ENTITIES];
     Render    render[MAX_ENTITIES];
} ActiveEntities;

#endif
