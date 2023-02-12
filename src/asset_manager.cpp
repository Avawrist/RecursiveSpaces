// ==============================================================
// Title: asset_manager.cpp
// Description: The source file for the memory structs storing
//              pointers to mesh, texture and sfx assets
// ==============================================================

#include "asset_manager.hpp"

// TODO:
// 1. ADD ASSERTIONS TO EVERYTHING. Why are assertions not working?
// 2. Exception handling

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
    table[TEST][TEXTURE_D]  = "..\\assets\\textures\\brickwall.bmp";
    table[TEST][TEXTURE_N]  = "..\\assets\\textures\\brickwall_normal.bmp";
    table[TEST][TEXTURE_S]  = nullptr;
    table[TEST][MESH01]     = "..\\assets\\meshes\\cube.obj";
    table[TEST][SFX01]      = "..\\assets\\sfx\\taunt.wav";
    table[TEST][SFX02]      = nullptr;
    table[TEST][SFX03]      = nullptr;

    // Chest Object
    table[CHEST][TEXTURE_D]  = "..\\assets\\textures\\chest.bmp";
    table[CHEST][TEXTURE_N]  = nullptr;
    table[CHEST][TEXTURE_S]  = nullptr;
    table[CHEST][MESH01]     = "..\\assets\\meshes\\chest.obj";
    table[CHEST][SFX01]      = nullptr;
    table[CHEST][SFX02]      = nullptr;
    table[CHEST][SFX03]      = nullptr;
}

///////////////////////////
// Struct ActiveTextures //
///////////////////////////

ActiveTextures::ActiveTextures()
{
    // Init pointer
    registered_count = 0;
}

int activeTexturesRegister(ActiveTextures &activeTextures, AssetManager &assetManager,
			   int object_type, int asset_type, c_char* path)
{
    // Assert that there is room to register

    // Allocate new texture and add pointer to activeTextures registry
    activeTextures.textures[activeTextures.registered_count] = new Texture(path);

    // Texture specific code: move the texture data to GPU
    textureDataToGPU(activeTextures.textures[activeTextures.registered_count]);
    
    // Add ID to asset table ID
    assetManager.assetTableID.table[object_type][asset_type] = activeTextures.registered_count;

    // Update registered count
    activeTextures.registered_count++;
    
    return 1;
}

void activeTexturesUnregisterAll(ActiveTextures &activeTextures)
{
    // Delete all pointers
    for(int i = 0; i < activeTextures.registered_count; i++)
    {
	delete activeTextures.textures[i];
    }
    // Reset count
    activeTextures.registered_count = 0;
}

/////////////////////////
// Struct ActiveMeshes //
/////////////////////////

ActiveMeshes::ActiveMeshes()
{
    registered_count = 0;
}

int activeMeshesRegister(ActiveMeshes &activeMeshes, AssetManager &assetManager,
                         int object_type, int asset_type, c_char* path)
{
    // Assert that there is room to register

    // Allocate new mesh and add pointer to activeMeshes registry
    activeMeshes.meshes[activeMeshes.registered_count] = new Mesh(path);

    // Mesh specific code: move the mesh data to GPU
    meshDataToGPU(activeMeshes.meshes[activeMeshes.registered_count]);

    // Add ID to asset table ID
    assetManager.assetTableID.table[object_type][asset_type] = activeMeshes.registered_count;

    // Update registered count
    activeMeshes.registered_count++;

    return 1;
}

void activeMeshesUnregisterAll(ActiveMeshes &activeMeshes)
{
    // Delete all pointers
    for(int i = 0; i < activeMeshes.registered_count; i++)
    {
	delete activeMeshes.meshes[i];
    }
    // Reset count
    activeMeshes.registered_count = 0;
}

//////////////////////////
// Struct AssetManager //
//////////////////////////

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
    case TEXTURE_D:
	return activeTexturesRegister(assetManager.activeTexturesD,
				      assetManager, object_type, asset_type, path);
    case TEXTURE_N:
	return activeTexturesRegister(assetManager.activeTexturesN,
				      assetManager, object_type, asset_type, path);
    case TEXTURE_S:
	return activeTexturesRegister(assetManager.activeTexturesS,
				      assetManager, object_type, asset_type, path);
    case MESH01:
	return activeMeshesRegister(assetManager.activeMeshes,
				    assetManager, object_type, asset_type, path);
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
    
    // Unregister Diffuse textures
    activeTexturesUnregisterAll(assetManager.activeTexturesD);

    // Unregister Normal textures]
    activeTexturesUnregisterAll(assetManager.activeTexturesN);
    
    // Unregister Specular textures
    activeTexturesUnregisterAll(assetManager.activeTexturesS);
    
    // Delete all mesh pointers
    activeMeshesUnregisterAll(assetManager.activeMeshes);
    
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
    case TEXTURE_D:
	return (void*)assetManager.activeTexturesD.textures[index];
    case TEXTURE_N:
        return (void*)assetManager.activeTexturesN.textures[index];
    case TEXTURE_S:
	return (void*)assetManager.activeTexturesS.textures[index];
    case MESH01:
	return (void*)assetManager.activeMeshes.meshes[index]; 
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
