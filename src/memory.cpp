// ==============================================================
// Title: memory.cpp
// Description: The source file for the memory structs storing
//              pointers to mesh, texture and sfx assets
// ==============================================================

#include "memory.hpp"

///////////////////////////
// Struct ActiveTextures //
///////////////////////////

ActiveTextures::ActiveTextures()
{
    // Init pointer
    registered_count = 0;
}

ActiveTextures::~ActiveTextures()
{
    // Unregister all registered textures
    activeTexturesUnregisterAll(this);
}

int activeTexturesRegister(ActiveTextures* activeTextures, c_char* path)
{
    // Assert that there is room to register a new texture
    if(!(activeTextures->registered_count < TOTAL_TEXTURES))
    {
	OutputDebugStringA("ERROR: ActiveTextures is full.\n");
	return -1;
    }
 
    // Add texture pointer to the registry
    activeTextures->textures[activeTextures->registered_count] = new Texture(path); 

    // Update registered count
    activeTextures->registered_count++;

    // Return index at which asset is registered
    return (activeTextures->registered_count - 1);
}

void activeTexturesUnregisterAll(ActiveTextures* activeTextures)
{
    // Delete all texture pointers
    for(int i = 0; i < activeTextures->registered_count; i++)
    {
	delete activeTextures->textures[i];
    }
    
    // Reset count
    activeTextures->registered_count = 0;
}
