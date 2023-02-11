// ==============================================================
// Title: asset_manager.cpp
// Description: The source file for the memory structs storing
//              pointers to mesh, texture and sfx assets
// ==============================================================

#include "asset_manager.hpp"

/////////////////////////
// Struct AssetTableID //
/////////////////////////

AssetTableID::AssetTableID()
{
    memset(table, -1, sizeof(int) * TOTAL_OBJECT_TYPES * TOTAL_ASSET_TYPES);
}

//////////////////////////
// Struct AssetTableDir //
//////////////////////////

AssetTableDir::AssetTableDir()
{
    // Test Object
    table[TEST][TEXTURE01]  = "";
    table[TEST][TEXTURE02]  = "..\\assets\\textures\\brickwall_normal.bmp";
    table[TEST][MESH01]     = "";
    table[TEST][SFX01]      = "..\\assets\\sfx\\taunt.wav";
    table[TEST][SFX02]      = "";
    table[TEST][SFX03]      = "";

    // Chest Object
    table[CHEST][TEXTURE01]  = "..\\assets\\textures\\chest.bmp";
    table[CHEST][TEXTURE02]  = "";
    table[CHEST][MESH01]     = "..\\assets\\meshes\\chest.obj";
    table[CHEST][SFX01]      = "";
    table[CHEST][SFX02]      = "";
    table[CHEST][SFX03]      = "";
}

///////////////////////////
// Struct ActiveTextures //
///////////////////////////

ActiveTextures::ActiveTextures()
{
    // Init pointer
    registered_count = 0;
}

//////////////////////////
// Struct Asset Manager //
//////////////////////////

// TODO:
// 1. ADD ASSERTIONS TO EVERYTHING. Why are assertions not working?
// 2. Exception handling
// 3. Add API specific load code for each asset type to registration code

AssetManager::~AssetManager()
{
    assetManagerUnregisterAll(*this);
}

int assetManagerRegister(AssetManager &assetManager, int object_type, int asset_type)
{
    // Get asset path
    c_char* path = assetManager.assetTableDir.table[object_type][asset_type];
    
    switch(asset_type)
    {
    case TEXTURE01:
	assetManager.activeTextures01.textures[assetManager.activeTextures01.registered_count] = new Texture(path);
	assetManager.assetTableID.table[object_type][asset_type] = assetManager.activeTextures01.registered_count;
	assetManager.activeTextures01.registered_count++;
	return 1;
    case TEXTURE02:
	assetManager.activeTextures02.textures[assetManager.activeTextures02.registered_count] = new Texture(path);
	assetManager.assetTableID.table[object_type][asset_type] = assetManager.activeTextures02.registered_count;
	assetManager.activeTextures02.registered_count++;
	return 1;
    case MESH01:
	return 0; // undefined case
    case SFX01:
	return 0; // undefined case
    case SFX02:
	return 0; // undefined case
    case SFX03:
	return 0; // undefined case
    default:
	return 0;
    }
}

void assetManagerUnregisterAll(AssetManager &assetManager)
{
    
    // Delete all texture01 pointers
    for(int i = 0; i < assetManager.activeTextures01.registered_count; i++)
    {
	delete assetManager.activeTextures01.textures[i];
    }
    // Reset count
    assetManager.activeTextures01.registered_count = 0;

    // Delete all texture02 pointers
    for(int i = 0; i < assetManager.activeTextures02.registered_count; i++)
    {
	delete assetManager.activeTextures02.textures[i];
    }
    // Reset count
    assetManager.activeTextures02.registered_count = 0;
    
    // Delete all mesh pointers

    // Delete all sfx01 pointers

    // Delete all sfx02 pointers

    // Delete all sfx03 pointers
}

void* assetManagerGetAssetP(AssetManager &assetManager, int object_type, int asset_type)
{
    int index = assetManager.assetTableID.table[object_type][asset_type];
    if(index < 0)
    {
	assetManagerRegister(assetManager, object_type, asset_type);
	index = assetManager.assetTableID.table[object_type][asset_type];
    }
    
    switch(asset_type)
    {
    case TEXTURE01:
	return (void*)assetManager.activeTextures01.textures[index];
	break;
    case TEXTURE02:
        return (void*)assetManager.activeTextures02.textures[index];
	break;
    case MESH01:
	// Undefined 
	break;
    case SFX01:
	// Undefined
	break;
    case SFX02:
	// Undefined
	break;
    case SFX03:
	// Undefined
	break;
    }
    return nullptr;
}
