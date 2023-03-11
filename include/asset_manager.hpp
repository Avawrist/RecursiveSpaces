// =======================================================
// Title: asset_manager.hpp
// Description: The header file for the structs storing
//              pointers to mesh, texture and sfx assets
// =======================================================

#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

// Win libs
#include <stdlib.h>
#include <cstring>

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"
#include "sound.hpp"
#include "asset.hpp"
#include "entity.hpp"

/////////////////////////
// Struct AssetTableID //
/////////////////////////

typedef enum AssetType
{
    TEXTURE_D         = 0,
    TEXTURE_N         = 1,
    TEXTURE_S         = 2,
    MESH01            = 3,
    SOUND01           = 4,
    SOUND02           = 5,
    SOUND03           = 6,
    TOTAL_ASSET_TYPES = 7
} AssetType;

typedef struct AssetTableID
{
    int table[TOTAL_ENTITY_TYPES][TOTAL_ASSET_TYPES];
    AssetTableID();
} AssetTableID;

//////////////////////////
// Struct AssetTableDir //
//////////////////////////

typedef struct AssetTableDir
{
    c_char* table[TOTAL_ENTITY_TYPES][TOTAL_ASSET_TYPES];
    AssetTableDir();
} AssetTableDir;

///////////////////////////
// Struct ActiveTextures //
///////////////////////////

#define MAX_TEXTURES 60
typedef struct ActiveTextures
{
    uint     registered_count;
    Texture* textures[MAX_TEXTURES];
    ActiveTextures();
} ActiveTextures;

/////////////////////////
// Struct ActiveMeshes //
/////////////////////////

#define MAX_MESHES 20
typedef struct ActiveMeshes
{
    uint registered_count;
    Mesh* meshes[MAX_MESHES];
    ActiveMeshes();
} ActiveMeshes;

/////////////////////////
// Struct ActiveSounds //
/////////////////////////

#define MAX_SOUNDS 60
typedef struct ActiveSounds
{
    uint registered_count;
    Sound* sounds[MAX_SOUNDS];
    ActiveSounds();
} ActiveSounds;

//////////////////////////
// Struct ShaderTableID //
//////////////////////////

typedef enum ShaderProgram
{
    BLINNPHONG            = 0,
    POSTPROCESS           = 1,
    DEBUG                 = 2,
    TOTAL_SHADER_PROGRAMS = 3
} ShaderProgram;

typedef enum ShaderType
{
    VERTEX             = 0,
    FRAGMENT           = 1,
    TOTAL_SHADER_TYPES = 2
} ShaderType;

typedef struct ShaderTableID
{
    int table[TOTAL_SHADER_PROGRAMS];
    ShaderTableID();
} ShaderTableID;

///////////////////////////
// Struct ShaderTableDir //
///////////////////////////

typedef struct ShaderTableDir
{
    c_char* table[TOTAL_SHADER_PROGRAMS][TOTAL_SHADER_TYPES];
    ShaderTableDir();
} ShaderTableDir;

//////////////////////////
// Struct ActiveShaders //
//////////////////////////

#define MAX_SHADERS 20

typedef struct ActiveShaders
{
    uint registered_count;
    Shader* shaders[MAX_SHADERS];
    ActiveShaders();
} ActiveShaders;

//////////////////
// AssetManager //
//////////////////

typedef struct AssetManager
{
    AssetTableID   asset_table_ID;
    AssetTableDir  asset_table_dir;
    ActiveTextures active_textures_d;
    ActiveTextures active_textures_n;
    ActiveTextures active_textures_s;
    ActiveMeshes   active_meshes;
    ActiveSounds   active_sounds_01;
    ActiveSounds   active_sounds_02;
    ActiveSounds   active_sounds_03;
    ShaderTableID  shader_table_ID;
    ShaderTableDir shader_table_dir;
    ActiveShaders  active_shaders;
    ~AssetManager();
} AssetManager;

/////////////////////////
// Function Prototypes //
/////////////////////////

// AssetManager function prototypes

int assetManagerRegister(AssetManager& asset_manager, int entity_type,
			  int asset_type, void* sound_interface_p);

void assetManagerUnregisterAll(AssetManager& asset_manager);

void* assetManagerGetAssetP(AssetManager& asset_manager, int entity_type,
			    int asset_type, void* sound_interface_p);
    
// ActiveTextures function prototypes

int activeTexturesRegister(ActiveTextures& active_textures, AssetManager& asset_manager,
			   int entity_type, int asset_type);

void activeTexturesUnregisterAll(ActiveTextures& active_textures);

// ActiveMeshes function prototypes

int activeMeshesRegister(ActiveMeshes& active_meshes, AssetManager& asset_manager,
                         int entity_type, int asset_type);

void activeMeshesUnregisterAll(ActiveMeshes& active_meshes);

// ActiveMeshes function prototypes

int activeSoundsRegister(ActiveSounds& active_sounds, AssetManager& asset_manager,
                         int entity_type, int asset_type);

// void activeSoundsUnregisterAll(ActiveSounds &activeSounds);

void* assetManagerGetShaderP(AssetManager& asset_manager, int program_type);

// ActiveShaders function prototypes

int activeShadersRegister(ActiveShaders& active_shaders, AssetManager& asset_manager,
			   int program_type);

void activeShadersUnregisterAll(ActiveShaders& active_shaders);


#endif
