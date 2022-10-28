// ======================================================================
// Title: entity.hpp
// Description: The header file for game objects
// ======================================================================

#ifndef ENTITY_H
#define ENTITY_H

// My libs
#include "typedefs.hpp"
#include "mdcla.hpp"

enum objectType 
{
    test = 0
};

typedef struct gameObject
{
    int        id;
    objectType type;
    Vec3F      position;
    gameObject();
} gameObject;

#endif
