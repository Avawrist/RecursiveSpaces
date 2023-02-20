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
    table[TEST][TEXTURE_D] = "..\\assets\\textures\\brickwall.bmp";
    table[TEST][TEXTURE_N] = "..\\assets\\textures\\brickwall_normal.bmp";
    table[TEST][TEXTURE_S] = NULL;
    table[TEST][MESH01]    = "..\\assets\\meshes\\cube.obj";
    table[TEST][SOUND01]   = "..\\assets\\sfx\\taunt.wav";
    table[TEST][SOUND02]   = NULL;
    table[TEST][SOUND03]   = NULL;
   
    // Chest Object
    table[CHEST][TEXTURE_D] = "..\\assets\\textures\\chest.bmp";
    table[CHEST][TEXTURE_N] = NULL;
    table[CHEST][TEXTURE_S] = NULL;
    table[CHEST][MESH01]    = "..\\assets\\meshes\\chest.obj";
    table[CHEST][SOUND01]   = NULL;
    table[CHEST][SOUND02]   = NULL;
    table[CHEST][SOUND03]   = NULL;
}

///////////////////////////
// Struct ActiveTextures //
///////////////////////////

ActiveTextures::ActiveTextures()
{
    registered_count = 0;

    // Initialize array to contain all NULL
    memset(textures, 0, sizeof(textures));
}

int activeTexturesRegister(ActiveTextures &activeTextures, AssetManager &assetManager,
			   int object_type, int asset_type)
{
    // Returns 1 on success, 0 on failure.

    // Assert that object and asset types are within acceptable range
    _assert(object_type >= 0 && object_type <= TOTAL_OBJECT_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);

    // Get path
    c_char* path = assetManager.assetTableDir.table[object_type][asset_type];
    if(!path) {return 0;}
    
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
    for(uint i = 0; i < activeTextures.registered_count; i++)
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

    // Init array to contain all NULLs
    memset(meshes, 0, sizeof(meshes));
}

int activeMeshesRegister(ActiveMeshes &activeMeshes, AssetManager &assetManager,
                         int object_type, int asset_type)
{
    // Returns 1 on success, 0 on failure.
    
    // Assert that object and asset types are within acceptable range
    _assert(object_type >= 0 && object_type <= TOTAL_OBJECT_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);
    
    // Get path
    c_char* path = assetManager.assetTableDir.table[object_type][asset_type];
    if(!path) {return 0;}
    
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
    for(uint i = 0; i < activeMeshes.registered_count; i++)
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

    // Init sound pointer array to NULL
    memset(sounds, 0, sizeof(sounds));
}

int activeSoundsRegister(ActiveSounds &activeSounds, AssetManager &assetManager, int object_type,
			 int asset_type, SoundInterface *soundInterface)
{
    // Returns 1 on success, 0 on failure.
    
    // Assert that object and asset types are within acceptable range
    _assert(object_type >= 0 && object_type <= TOTAL_OBJECT_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);

    // Get path
    c_char* path = assetManager.assetTableDir.table[object_type][asset_type];
    if(!path) {return 0;}
    
    // _assert that there is room to register
    if(!(activeSounds.registered_count < MAX_SOUNDS)) {return 0;}

    // Allocate new sound and add pointer to activeSounds registry
    activeSounds.sounds[activeSounds.registered_count] = new Sound(path, *soundInterface);
    if(!activeSounds.sounds[activeSounds.registered_count]) {return 0;}
    
    // Add ID to asset table ID
    assetManager.assetTableID.table[object_type][asset_type] = activeSounds.registered_count;

    // Update registered count
    activeSounds.registered_count++;

    return 1;
}

void activeSoundsUnregisterAll(ActiveSounds &activeSounds)
{
    // Delete all pointers
    for(uint i = 0; i < activeSounds.registered_count; i++)
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
    // Returns 1 on success, 0 on failure
    
    // Assert that object and asset types are within acceptable range
    _assert(object_type >= 0 && object_type <= TOTAL_OBJECT_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);
    
    switch(asset_type)
    {
    case TEXTURE_D:
	return activeTexturesRegister(assetManager.activeTexturesD,
				      assetManager, object_type, asset_type);
    case TEXTURE_N:
	return activeTexturesRegister(assetManager.activeTexturesN,
				      assetManager, object_type, asset_type);
    case TEXTURE_S:
	return activeTexturesRegister(assetManager.activeTexturesS,
				      assetManager, object_type, asset_type);
    case MESH01:
	return activeMeshesRegister(assetManager.activeMeshes,
				    assetManager, object_type, asset_type);
    case SOUND01:
	return activeSoundsRegister(assetManager.activeSounds01, assetManager,
	                            object_type, asset_type, (SoundInterface*)soundInterfaceP);
    case SOUND02:
	return activeSoundsRegister(assetManager.activeSounds02, assetManager,
	                            object_type, asset_type, (SoundInterface*)soundInterfaceP);
    case SOUND03:
	return activeSoundsRegister(assetManager.activeSounds03, assetManager,
	                            object_type, asset_type, (SoundInterface*)soundInterfaceP);
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
    // Returns void asset pointer on success, NULL on failure.

    // Assert that object and asset types are within acceptable range
    _assert(object_type >= 0 && object_type <= TOTAL_OBJECT_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);
    
    // init return value
    void* return_p = NULL;
    
    int index = assetManager.assetTableID.table[object_type][asset_type];
    if(index < 0)
    {
	if(!assetManagerRegister(assetManager, object_type, asset_type, soundInterfaceP))
	{
	    return return_p;
	}
	index = assetManager.assetTableID.table[object_type][asset_type];
    }
    
    switch(asset_type)
    {
    case TEXTURE_D:
	return_p = (void*)assetManager.activeTexturesD.textures[index];
	break;
    case TEXTURE_N:
        return_p = (void*)assetManager.activeTexturesN.textures[index];
	break;
    case TEXTURE_S:
	return_p = (void*)assetManager.activeTexturesS.textures[index];
	break;
    case MESH01:
	return_p = (void*)assetManager.activeMeshes.meshes[index];
	break;
    case SOUND01:
	return_p = (void*)assetManager.activeSounds01.sounds[index];
	break;
    case SOUND02:
	return_p = (void*)assetManager.activeSounds02.sounds[index];
	break;
    case SOUND03:
	return_p = (void*)assetManager.activeSounds03.sounds[index];
	break;
    }
    return return_p;
}

//////////////////////////
// Struct ShaderTableID //
//////////////////////////

ShaderTableID::ShaderTableID()
{
    memset(table, -1, sizeof(int) * TOTAL_SHADER_OBJECTS * TOTAL_SHADER_TYPES);
}

///////////////////////////
// Struct ShaderTableDir //
///////////////////////////

ShaderTableDir::ShaderTableDir()
{
    // Blinn-Phong Shaders
    table[BLINNPHONG][VERTEX]   = "..\\assets\\shaders\\blinn_phong.vert";
    table[BLINNPHONG][GEOMETRY] = NULL;
    table[BLINNPHONG][FRAGMENT] = "..\\assets\\shaders\\blinn_phong.frag";

    // Post-Processing Shaders
    table[POSTPROCESS][VERTEX]   = "..\\assets\\shaders\\pp.vert";
    table[POSTPROCESS][GEOMETRY] = NULL;
    table[POSTPROCESS][FRAGMENT] = "..\\assets\\shaders\\pp.frag";
}

//////////////////////////
// Struct ActiveShaders //
//////////////////////////

ActiveShaders::ActiveShaders()
{
    registered_count = 0;
    
    // Initialize array to contain all NULL
    memset(shaders, 0, sizeof(shaders));
}

//////////////////////////
// Struct ShaderManager //
//////////////////////////
