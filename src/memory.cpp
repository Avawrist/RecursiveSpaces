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
    texture_memory_p = nullptr;
    registered_count = 0;
    
    // Allocate memory
    texture_memory_p = (Texture*)malloc(TOTAL_TEXTURES * POINTER_SIZE);
    if(!texture_memory_p)
    {
	OutputDebugStringA("ERROR: Unable to allocate the TEXTURE memory.\n");
    }
}

ActiveTextures::~ActiveTextures()
{
    // Unregister all registered textures
    activeTexturesUnregisterAll(this);
    
    // Free memory
    free(texture_memory_p);
}

int activeTexturesRegister(ActiveTextures* activeTextures, c_char* path)
{
    // Assert that there is room to register a new texture
    if(!(activeTextures->registered_count < activeTextures->TOTAL_TEXTURES))
    {
	OutputDebugStringA("ERROR: ActiveTextures is full.\n");
	return -1;
    }
 
    // Add texture pointer to the registry
    Texture *temp_texture_p = new Texture(path);
    activeTextures->texture_memory_p[activeTextures->registered_count] = temp_texture_p; 

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
	delete activeTextures->texture_memory_p[i];
    }
    
    // Reset count
    activeTextures->registered_count = 0;
}
