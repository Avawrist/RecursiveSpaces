// =======================================================
// Title: asset_manager.hpp
// Description: The header file for the structs storing
//              pointers to mesh, texture and sfx assets
// =======================================================

#ifndef MEMORY_H
#define MEMORY_H

// Win libs
#include <stdlib.h>
#include <cstring>

// My libs
#include "sound.hpp"
#include "asset.hpp"
#include "typedefs.hpp"

#define TEXTURE01         0
#define TEXTURE02         1
#define MESH01            2
#define SFX01             3
#define SFX02             4
#define SFX03             5
#define TOTAL_ASSET_TYPES 6

#define TEST               0
#define CHEST              1
#define TOTAL_OBJECT_TYPES 2

/////////////////////////
// Struct AssetTableID //
/////////////////////////

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

#define MAX_TEXTURES 20
typedef struct ActiveTextures
{
    uint     registered_count;
    Texture* textures[MAX_TEXTURES];
    ActiveTextures();
} ActiveTextures;

/////////////////////////
// Struct ActiveMeshes //
/////////////////////////

///////////////////////
// Struct ActiveSFX //
///////////////////////

//////////////////
// AssetManager //
//////////////////

typedef struct AssetManager
{
    AssetTableID   assetTableID;
    AssetTableDir  assetTableDir;
    ActiveTextures activeTextures01;
    ActiveTextures activeTextures02;
    //ActiveMeshes activeMeshes;
    //ActiveSFX    activeSFX01;
    //ActiveSFX    activeSFX02;
    //ActiveSFX    activeSFX03;
    ~AssetManager();
} AssetManager;
int  assetManagerRegister(AssetManager &assetManager, int object_type, int asset_type);
void assetManagerUnregisterAll(AssetManager &assetManager);
void* assetManagerGetAssetP(AssetManager &assetManager, int object_type, int asset_type);

#endif
