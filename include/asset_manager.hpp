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

//////////////////
// AssetManager //
//////////////////

typedef struct AssetManager
{
    AssetTableID   assetTableID;
    AssetTableDir  assetTableDir;
    ActiveTextures activeTexturesD;
    ActiveTextures activeTexturesN;
    ActiveTextures activeTexturesS;
    ActiveMeshes   activeMeshes;
    ActiveSounds   activeSounds01;
    ActiveSounds   activeSounds02;
    ActiveSounds   activeSounds03;
    ~AssetManager();
} AssetManager;
int  assetManagerRegister(AssetManager &assetManager, int object_type, int asset_type, void *soundInterfaceP);
void assetManagerUnregisterAll(AssetManager &assetManager);
void* assetManagerGetAssetP(AssetManager &assetManager, int object_type,
			    int asset_type, void *soundInterfaceP);

// ActiveTextures function prototypes
int activeTexturesRegister(ActiveTextures &activeTextures, AssetManager &assetManager,
			   int object_type, int asset_type);
void activeTexturesUnregisterAll(ActiveTextures &activeTextures);
// ActiveMeshes function prototypes
int activeMeshesRegister(ActiveMeshes &activeMeshes, AssetManager &assetManager,
                         int object_type, int asset_type);
void activeMeshesUnregisterAll(ActiveMeshes &activeMeshes);
// ActiveMeshes function prototypes
int activeSoundsRegister(ActiveSounds &activeSounds, AssetManager &assetManager,
                         int object_type, int asset_type);
// void activeSoundsUnregisterAll(ActiveSounds &activeSounds);

//////////////////////////
// Struct ShaderTableID //
//////////////////////////

#define BLINNPHONG            0
#define POSTPROCESS           1
#define GRID                  2
#define TOTAL_SHADER_PROGRAMS 3

#define VERTEX             0
#define FRAGMENT           1
#define TOTAL_SHADER_TYPES 2

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

//////////////////////////
// Struct ShaderManager //
//////////////////////////

typedef struct ShaderManager
{
    ShaderTableID  shaderTableID;
    ShaderTableDir shaderTableDir;
    ActiveShaders  activeShaders;
    ~ShaderManager();
} ShaderManager;
void* shaderManagerGetShaderP(ShaderManager &shaderManager, int program_type);

// ActiveShaders function prototypes
int activeShadersRegister(ActiveShaders &activeShaders, ShaderManager &shaderManager,
			   int program_type);
void activeShadersUnregisterAll(ActiveShaders &activeShaders);

#endif
