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
    memset(table, -1, sizeof(int) * TOTAL_ENTITY_TYPES * TOTAL_ASSET_TYPES);
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
    table[CHEST][TEXTURE_N] = "..\\assets\\textures\\brickwall_normal.bmp";
    table[CHEST][TEXTURE_S] = NULL;
    table[CHEST][MESH01]    = "..\\assets\\meshes\\chest.obj";
    table[CHEST][SOUND01]   = "..\\assets\\sfx\\taunt.wav";;
    table[CHEST][SOUND02]   = NULL;
    table[CHEST][SOUND03]   = NULL;

    // Line Object
    table[GRID][TEXTURE_D] = NULL;
    table[GRID][TEXTURE_N] = NULL;
    table[GRID][TEXTURE_S] = NULL;
    table[GRID][MESH01]    = "..\\assets\\meshes\\grid.obj";
    table[GRID][SOUND01]   = NULL;
    table[GRID][SOUND02]   = NULL;
    table[GRID][SOUND03]   = NULL;
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

int activeTexturesRegister(ActiveTextures &active_textures, AssetManager &asset_manager,
			   int entity_type, int asset_type)
{
    // Returns 1 on success, 0 on failure.

    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);

    // Get path
    c_char* path = asset_manager.asset_table_dir.table[entity_type][asset_type];
    if(!path) {return 0;}
    
    // Check that there is room to register
    if(!(active_textures.registered_count < MAX_TEXTURES)) {return 0;}

    // Allocate new texture and add pointer to activeTextures registry
    active_textures.textures[active_textures.registered_count] = new Texture(path);
    if(!active_textures.textures[active_textures.registered_count]) {return 0;}

    // Add ID to asset table ID
    asset_manager.asset_table_ID.table[entity_type][asset_type] = active_textures.registered_count;

    // Update registered count
    active_textures.registered_count++;
    
    return 1;
}

void activeTexturesUnregisterAll(ActiveTextures &active_textures)
{
    // Delete all pointers
    for(uint i = 0; i < active_textures.registered_count; i++)
    {
	delete active_textures.textures[i];
    }
    // Reset count
    active_textures.registered_count = 0;
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

int activeMeshesRegister(ActiveMeshes &active_meshes, AssetManager &asset_manager,
                         int entity_type, int asset_type)
{
    // Returns 1 on success, 0 on failure.
    
    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);
    
    // Get path
    c_char* path = asset_manager.asset_table_dir.table[entity_type][asset_type];
    if(!path) {return 0;}
    
    // Check that there is room to register
    if(!(active_meshes.registered_count < MAX_MESHES)) {return 0;}
    
    // Allocate new mesh and add pointer to activeMeshes registry
    active_meshes.meshes[active_meshes.registered_count] = new Mesh(path);
    if(!active_meshes.meshes[active_meshes.registered_count]) {return 0;}

    // Add ID to asset table ID
    asset_manager.asset_table_ID.table[entity_type][asset_type] = active_meshes.registered_count;

    // Update registered count
    active_meshes.registered_count++;

    return 1;
}

void activeMeshesUnregisterAll(ActiveMeshes &active_meshes)
{
    // Delete all pointers
    for(uint i = 0; i < active_meshes.registered_count; i++)
    {
	delete active_meshes.meshes[i];
    }
    // Reset count
    active_meshes.registered_count = 0;
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

int activeSoundsRegister(ActiveSounds &active_sounds, AssetManager &asset_manager, int entity_type,
			 int asset_type, SoundInterface* sound_interface_p)
{
    // Returns 1 on success, 0 on failure.
    
    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);

    // Get path
    c_char* path = asset_manager.asset_table_dir.table[entity_type][asset_type];
    if(!path) {return 0;}
    
    // Check that there is room to register
    if(!(active_sounds.registered_count < MAX_SOUNDS)) {return 0;}

    // Allocate new sound and add pointer to activeSounds registry
    active_sounds.sounds[active_sounds.registered_count] = new Sound(path, *sound_interface_p);
    if(!active_sounds.sounds[active_sounds.registered_count]) {return 0;}
    
    // Add ID to asset table ID
    asset_manager.asset_table_ID.table[entity_type][asset_type] = active_sounds.registered_count;

    // Update registered count
    active_sounds.registered_count++;

    return 1;
}

void activeSoundsUnregisterAll(ActiveSounds &active_sounds)
{
    // Delete all pointers
    for(uint i = 0; i < active_sounds.registered_count; i++)
    {
	delete active_sounds.sounds[i];
    }
    // Reset count
    active_sounds.registered_count = 0;
}

//////////////////////////
// Struct AssetManager //
//////////////////////////

AssetManager::~AssetManager()
{
    assetManagerUnregisterAll(*this);
}

int assetManagerRegister(AssetManager &asset_manager, int entity_type, int asset_type, void *sound_interface_p)
{
    // Returns 1 on success, 0 on failure
    
    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);
    
    switch(asset_type)
    {
    case TEXTURE_D:
	return activeTexturesRegister(asset_manager.active_textures_d,
				      asset_manager, entity_type, asset_type);
    case TEXTURE_N:
	return activeTexturesRegister(asset_manager.active_textures_n,
				      asset_manager, entity_type, asset_type);
    case TEXTURE_S:
	return activeTexturesRegister(asset_manager.active_textures_s,
				      asset_manager, entity_type, asset_type);
    case MESH01:
	return activeMeshesRegister(asset_manager.active_meshes,
				    asset_manager, entity_type, asset_type);
    case SOUND01:
	return activeSoundsRegister(asset_manager.active_sounds_01, asset_manager,
	                            entity_type, asset_type, (SoundInterface*)sound_interface_p);
    case SOUND02:
	return activeSoundsRegister(asset_manager.active_sounds_02, asset_manager,
	                            entity_type, asset_type, (SoundInterface*)sound_interface_p);
    case SOUND03:
	return activeSoundsRegister(asset_manager.active_sounds_03, asset_manager,
	                            entity_type, asset_type, (SoundInterface*)sound_interface_p);
    default:
	return 0;
    }
}

void assetManagerUnregisterAll(AssetManager &asset_manager)
{
    
    // Unregister Diffuse textures
    activeTexturesUnregisterAll(asset_manager.active_textures_d);

    // Unregister Normal textures
    activeTexturesUnregisterAll(asset_manager.active_textures_n);
    
    // Unregister Specular textures
    activeTexturesUnregisterAll(asset_manager.active_textures_s);
    
    // Delete all mesh pointers
    activeMeshesUnregisterAll(asset_manager.active_meshes);
    
    // Delete all sound01 pointers
    activeSoundsUnregisterAll(asset_manager.active_sounds_01);
    
    // Delete all sound02 pointers
    activeSoundsUnregisterAll(asset_manager.active_sounds_02);
    
    // Delete all sound03 pointers
    activeSoundsUnregisterAll(asset_manager.active_sounds_03);

    // Delete all shaders
    activeShadersUnregisterAll(asset_manager.active_shaders);
}

void* assetManagerGetAssetP(AssetManager &asset_manager, int entity_type,
			    int asset_type, void *sound_interface_p)
{
    // Returns void asset pointer on success, NULL on failure.

    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);
    
    // init return value
    void* return_p = NULL;
    
    int index = asset_manager.asset_table_ID.table[entity_type][asset_type];
    if(index < 0)
    {
	if(!assetManagerRegister(asset_manager, entity_type, asset_type, sound_interface_p))
	{
	    return return_p;
	}
	index = asset_manager.asset_table_ID.table[entity_type][asset_type];
    }
    
    switch(asset_type)
    {
    case TEXTURE_D:
	return_p = (void*)asset_manager.active_textures_d.textures[index];
	break;
    case TEXTURE_N:
        return_p = (void*)asset_manager.active_textures_n.textures[index];
	break;
    case TEXTURE_S:
	return_p = (void*)asset_manager.active_textures_s.textures[index];
	break;
    case MESH01:
	return_p = (void*)asset_manager.active_meshes.meshes[index];
	break;
    case SOUND01:
	return_p = (void*)asset_manager.active_sounds_01.sounds[index];
	break;
    case SOUND02:
	return_p = (void*)asset_manager.active_sounds_02.sounds[index];
	break;
    case SOUND03:
	return_p = (void*)asset_manager.active_sounds_03.sounds[index];
	break;
    }
    return return_p;
}

void* assetManagerGetShaderP(AssetManager &asset_manager, int program_type)
{
    // Returns void shader pointer on success, NULL on failure

    // Assert that program type is within acceptable range
    _assert(program_type >= 0 && program_type <= TOTAL_SHADER_PROGRAMS);

    int index = asset_manager.shader_table_ID.table[program_type];
    if(index < 0)
    {
	if(!activeShadersRegister(asset_manager.active_shaders, asset_manager, program_type))
	{
	    return NULL;
	}
	index = asset_manager.shader_table_ID.table[program_type];
    }

    return (void*)asset_manager.active_shaders.shaders[index];
}

//////////////////////////
// Struct ShaderTableID //
//////////////////////////

ShaderTableID::ShaderTableID()
{
    memset(table, -1, sizeof(int) * TOTAL_SHADER_PROGRAMS);
}

///////////////////////////
// Struct ShaderTableDir //
///////////////////////////

ShaderTableDir::ShaderTableDir()
{
    // Blinn-Phong Shader Program
    table[BLINNPHONG][VERTEX]   = "..\\assets\\shaders\\blinn_phong.vert";
    table[BLINNPHONG][FRAGMENT] = "..\\assets\\shaders\\blinn_phong.frag";

    // Post-Processing Shader Program
    table[POSTPROCESS][VERTEX]   = "..\\assets\\shaders\\pp.vert";
    table[POSTPROCESS][FRAGMENT] = "..\\assets\\shaders\\pp.frag";

    // Grid Shader Program
    table[GRID][VERTEX]   = "..\\assets\\shaders\\db_grid.vert";
    table[GRID][FRAGMENT] = "..\\assets\\shaders\\db_grid.frag";
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

int activeShadersRegister(ActiveShaders &active_shaders, AssetManager &asset_manager,
			   int program_type)
{
    // Returns 1 on success, 0 on failure

    // Assert that program type is within acceptable range
    _assert(program_type >= 0 && program_type <= TOTAL_SHADER_PROGRAMS);

    // Get paths
    c_char* vertex_path = asset_manager.shader_table_dir.table[program_type][VERTEX];
    if(!vertex_path) {return 0;}
    c_char* fragment_path = asset_manager.shader_table_dir.table[program_type][FRAGMENT];
    if(!fragment_path) {return 0;}
    
    // Check that there is room to register
    if(!(active_shaders.registered_count < MAX_SHADERS)) {return 0;}

    // Allocate new shader and add pointer to the activeShaders registry
    active_shaders.shaders[active_shaders.registered_count] = new Shader(vertex_path, fragment_path);
    if(!active_shaders.shaders[active_shaders.registered_count]) {return 0;}

    // Add ID to shader table ID
    asset_manager.shader_table_ID.table[program_type] = active_shaders.registered_count;

    // Update registered count
    active_shaders.registered_count++;

    return 1;
}

void activeShadersUnregisterAll(ActiveShaders &active_shaders)
{
    // Delete all shader structs
    for(uint i = 0; i < active_shaders.registered_count; i++)
    {
	delete active_shaders.shaders[i];
    }

    // Reset registered count
    active_shaders.registered_count = 0;
}


