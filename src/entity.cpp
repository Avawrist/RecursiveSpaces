// ======================================================================
// Title: entity.cpp
// Description: The source file for game objects
// ======================================================================

#include "entity.hpp"

gameObject::gameObject()
{
    id       = 0;
    type     = test;
    position = Vec3F(0.0f, 0.0f, 0.0f);
}
