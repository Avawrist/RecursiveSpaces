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
    dir   = Vec3F(0.0f, -1.0f, -1.0f);
    ambient_strength = 0.25f;
}

DirLight::DirLight(Vec3F _color, Vec3F _dir, float _ambient_strength)
{
    color = _color;
    dir   = _dir;
    ambient_strength = _ambient_strength;
}

////////////////////////
// Point Light Struct //
////////////////////////

///////////////////////
// Spot Light Struct //
///////////////////////
