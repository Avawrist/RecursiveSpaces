// ======================================================================
// Title: light.cpp
// Description: The source file for directional, point & spotlight objects
// ======================================================================

#include "light.hpp"

//////////////////////////////
// Directional Light Struct //
//////////////////////////////

DirLight::DirLight()
{
    color = Vec3F(1.0f, 1.0f, 1.0f);
    dir   = Vec3F(0.0f, -1.0f, 0.0f);
    ambient_strength = 0.1f;
}

DirLight::DirLight(Vec3F _color, Vec3F _dir)
{
    color = _color;
    dir   = _dir;
    ambient_strength = 0.1f;
}

////////////////////////
// Point Light Struct //
////////////////////////

///////////////////////
// Spot Light Struct //
///////////////////////
