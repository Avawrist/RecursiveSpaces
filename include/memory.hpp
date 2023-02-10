// =======================================================
// Title: memory.hpp
// Description: The header file for the structs storing
//              pointers to mesh, texture and sfx assets
// =======================================================

#ifndef MEMORY_H
#define MEMORY_H

// Win libs
#include <stdlib.h>

// My libs
#include "sound.hpp"
#include "asset.hpp"
#include "typedefs.hpp"

#define KILOBYTES 1024
#define MEGABYTES 1024 * KILOBYTES
#define GIGABYTES 1024 * MEGABYTES
#define TERABYTES 1024 * GIGABYTES

#define POINTER_SIZE 8

///////////////////////////
// Struct ActiveTextures //
///////////////////////////

typedef struct ActiveTextures
{
    c_uint   TOTAL_TEXTURES = 20;
    uint     registered_count;
    Texture* texture_memory_p;
    ActiveTextures();
    ~ActiveTextures();
} ActiveTextures;
int  activeTexturesRegister(ActiveTextures* activeTextures, c_char* path);
void activeTexturesUnregisterAll(ActiveTextures* activeTextures);

#endif
