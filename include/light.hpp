// ======================================================================
// Title: light.hpp
// Description: The header file for directional, point & spotlight objects
// ======================================================================

#ifndef LIGHT_H
#define LIGHT_H

// My libs
#include "utility.hpp"
#include "mdcla.hpp"

//////////////////////////////
// Directional Light Struct //
//////////////////////////////

typedef struct DirLight
{
    Vec3F color;
    Vec3F dir;
    float ambient_strength;
    DirLight();
    DirLight(Vec3F _color, Vec3F _dir, float _ambient_strength);
} DirLight;

////////////////////////
// Point Light Struct //
////////////////////////
/*
typedef struct PointLight
{

} PointLight;

///////////////////////
// Spot Light Struct //
///////////////////////

typedef struct SpotLight
{
    
} Spotlight;
*/
#endif
