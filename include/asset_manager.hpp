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

/////////////////////////
// Struct AssetTableID //
/////////////////////////

#define TEXTURE_D         0
#define TEXTURE_N         1
#define TEXTURE_S         2
#define MESH01            3
#define SOUND01           4
#define SOUND02           5
#define SOUND03           6
#define TOTAL_ASSET_TYPES 7

#define TEST               0
#define CHEST              1
#define TOTAL_OBJECT_TYPES 2

typedef struct AssetTableID
{
    int table[TOTAL_OBJECT_TYPES][TOTAL_ASSET_TYPES];
    AssetTableID();
} AssetTableID;

//////////////////////////
// Struct AssetTableDir //
//////////////////////////

typedef struct AssetTableDir
{
    c_char* table[TOTAL_OBJECT_TYPES][TOTAL_ASSET_TYPES];
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

#define BLINNPHONG           0
#define POSTPROCESS          1
#define TOTAL_SHADER_OBJECTS 2

#define VERTEX             0
#define GEOMETRY           1
#define FRAGMENT           2
#define TOTAL_SHADER_TYPES 3

typedef struct ShaderTableID
{
    int table[TOTAL_SHADER_OBJECTS][TOTAL_SHADER_TYPES];
    ShaderTableID();
} ShaderTableID;

///////////////////////////
// Struct ShaderTableDir //
///////////////////////////

typedef struct ShaderTableDir
{
    c_char* table[TOTAL_SHADER_OBJECTS][TOTAL_SHADER_TYPES];
    ShaderTableDir();
} ShaderTableDir;

//////////////////////////
// Struct ActiveShaders //
//////////////////////////

#define MAX_SHADERS 60

typedef struct ActiveShaders
{
    uint registered_count;
    Shader* shaders[MAX_SHADERS];
    ActiveShaders();
} ActiveShaders;

//////////////////////////
// Struct ShaderManager //
//////////////////////////

#endif
