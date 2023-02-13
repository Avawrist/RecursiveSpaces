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
    table[TEST][TEXTURE_D]  = "..\\assets\\textures\\brickwall.bmp";
    table[TEST][TEXTURE_N]  = "..\\assets\\textures\\brickwall_normal.bmp";
    table[TEST][TEXTURE_S]  = nullptr;
    table[TEST][MESH01]     = "..\\assets\\meshes\\cube.obj";
    table[TEST][SOUND01]      = "..\\assets\\sfx\\taunt.wav";
    table[TEST][SOUND02]      = nullptr;
    table[TEST][SOUND03]      = nullptr;

    // Chest Object
    table[CHEST][TEXTURE_D]  = "..\\assets\\textures\\chest.bmp";
    table[CHEST][TEXTURE_N]  = nullptr;
    table[CHEST][TEXTURE_S]  = nullptr;
    table[CHEST][MESH01]     = "..\\assets\\meshes\\chest.obj";
    table[CHEST][SOUND01]      = nullptr;
    table[CHEST][SOUND02]      = nullptr;
    table[CHEST][SOUND03]      = nullptr;
}

///////////////////////////
// Struct ActiveTextures //
///////////////////////////

ActiveTextures::ActiveTextures()
{
    registered_count = 0;

    // Initialize array to contain all nullptrs
    memset(textures, 0, sizeof(textures));
}

int activeTexturesRegister(ActiveTextures &activeTextures, AssetManager &assetManager,
			   int object_type, int asset_type, c_char* path)
{
    // Check that there is room to register
    if(!(activeTextures.registered_count < MAX_TEXTURES)) {return 0;}

    // Allocate new texture and add pointer to activeTextures registry
    activeTextures.textures[activeTextures.registered_count] = new Texture(path);
    if(!activeTextures.textures[activeTextures.registered_count]) {return 0;}
    
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

    // Init array to contain all nullptrs
    memset(meshes, 0, sizeof(meshes));
}

int activeMeshesRegister(ActiveMeshes &activeMeshes, AssetManager &assetManager,
                         int object_type, int asset_type, c_char* path)
{
    // Check that there is room to register
    if(!(activeMeshes.registered_count < MAX_MESHES)) {return 0;}
    
    // Allocate new mesh and add pointer to activeMeshes registry
    activeMeshes.meshes[activeMeshes.registered_count] = new Mesh(path);
    if(!activeMeshes.meshes[activeMeshes.registered_count]) {return 0;}

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

/////////////////////////
// Struct ActiveSounds //
/////////////////////////

ActiveSounds::ActiveSounds()
{
    registered_count = 0;

    // Init sound pointer array to nullptrs
    memset(sounds, 0, sizeof(sounds));
}

int activeSoundsRegister(ActiveSounds &activeSounds, AssetManager &assetManager, int object_type,
			 int asset_type, c_char* path, SoundInterface *soundInterface)
{
    // Assert that there is room to register
    if(!(activeSounds.registered_count < MAX_SOUNDS)) {return 0;}

    // Allocate new sound and add pointer to activeSounds registry
    activeSounds.sounds[activeSounds.registered_count] = new Sound(path, *soundInterface);
    if(!activeSounds.sounds[activeSounds.registered_count]) {return 0;}
    
    // Add ID to asset table ID
    assetManager.assetTableID.table[object_type][asset_type] = activeSounds.registered_count;

    // Update registered count
    activeSounds.registered_count++;

    return 0;
}

void activeSoundsUnregisterAll(ActiveSounds &activeSounds)
{
    // Delete all pointers
    for(int i = 0; i < activeSounds.registered_count; i++)
    {
	delete activeSounds.sounds[i];
    }
    // Reset count
    activeSounds.registered_count = 0;
}

//////////////////////////
// Struct AssetManager //
//////////////////////////

AssetManager::~AssetManager()
{
    assetManagerUnregisterAll(*this);
}

int assetManagerRegister(AssetManager &assetManager, int object_type, int asset_type, void *soundInterfaceP)
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
    case SOUND01:
	return activeSoundsRegister(assetManager.activeSounds01, assetManager,
	                            object_type, asset_type, path, (SoundInterface*)soundInterfaceP);
    case SOUND02:
	return activeSoundsRegister(assetManager.activeSounds02, assetManager,
	                            object_type, asset_type, path, (SoundInterface*)soundInterfaceP);
    case SOUND03:
	return activeSoundsRegister(assetManager.activeSounds03, assetManager,
	                            object_type, asset_type, path, (SoundInterface*)soundInterfaceP);
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
    
    // Delete all sound01 pointers
    activeSoundsUnregisterAll(assetManager.activeSounds01);
    
    // Delete all sound02 pointers
    activeSoundsUnregisterAll(assetManager.activeSounds02);
    
    // Delete all sound03 pointers
    activeSoundsUnregisterAll(assetManager.activeSounds03);
}

void* assetManagerGetAssetP(AssetManager &assetManager, int object_type, int asset_type, void *soundInterfaceP)
{
    int index = assetManager.assetTableID.table[object_type][asset_type];
    if(index < 0)
    {
	assetManagerRegister(assetManager, object_type, asset_type, soundInterfaceP);
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
    case SOUND01:
	return (void*)assetManager.activeSounds01.sounds[index];
    case SOUND02:
	return (void*)assetManager.activeSounds02.sounds[index];
    case SOUND03:
	return (void*)assetManager.activeSounds03.sounds[index];
    }
    return nullptr;
}
