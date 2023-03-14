// ======================================================================
// Title: asset.hpp
// Description: The header file for model, mesh & material objects
// ======================================================================

#ifndef ASSET_H
#define ASSET_H

// Win/C libs
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <xaudio2.h>
#include <algorithm>
#include <comdef.h>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// My libs
#include "utility.hpp"
#include "mdcla.hpp"
#include "entity.hpp"

///////////////////////////
// Struct SoundInterface //
///////////////////////////

typedef struct SoundInterface
{
    IXAudio2*               interface_p;
    IXAudio2MasteringVoice* master_voice_p;
    SoundInterface();
    ~SoundInterface();
} SoundInterface;
int soundInterfaceLoadXAudio2();

//////////////////
// Struct Sound //
//////////////////

typedef struct Sound
{
    WAVEFORMATEX         waveFormat = {0};
    XAUDIO2_BUFFER       buffer = {0};
    IXAudio2SourceVoice* source_voice_p;
    Sound(c_char* wav_path, SoundInterface& sound_interface);
    ~Sound();
} Sound;
int  soundLoadWav(Sound* sound, c_char* wav_path);
int  soundPlay(Sound* sound);
void soundPause(Sound* sound);
void soundStop(Sound* sound);
void soundSetVolume(Sound* sound, int volume);

////////////////////////
// Struct SoundStream //
////////////////////////

#define NUM_BUFFERS 3
#define BUFFER_SIZE 65536

typedef struct SoundStream
{
    WAVEFORMATEX         waveFormat = {0};
    XAUDIO2_BUFFER       buffer = {0};
    IXAudio2SourceVoice* source_voice_p;
    FILE*                file_p;
    BYTE                 buffers[NUM_BUFFERS][BUFFER_SIZE];
    uint                 bytes_read;
    uint                 total_bytes;
    uint                 cw_buffer;
    SoundStream(c_char* wav_path, SoundInterface& sound_interface);
    ~SoundStream();
} SoundStream;
int  soundStreamReadWavHeader(SoundStream* sound_stream, c_char* wav_path);
int  soundStreamUpdate(SoundStream* sound_stream);
void soundStreamPlay(SoundStream* sound_stream);
void soundStreamPause(SoundStream* sound_stream);
void soundStreamStop(SoundStream* sound_stream);
void soundSetVolume(SoundStream* sound, int volume);

///////////////////
// Struct Shader //
///////////////////

typedef struct Shader {
    GLuint program_id;
    GLuint vert_id;
    GLuint frag_id;
    Shader(c_char* _vert_path, c_char* _frag_path);
    ~Shader();
} Shader;
void shaderAddMat4Uniform(const Shader* shader_p, c_char* name, c_float* m);
void shaderAddVec3Uniform(const Shader* shader_p, c_char* name, const Vec3F& v);
void shaderAddIntUniform(const Shader* shader_p, c_char* name, int i);
void shaderAddFloatUniform(const Shader* shader_p, c_char* name, float f);

/////////////////
// Struct Mesh //
/////////////////

#define MESH_INDEX 0
#define ELE_INDEX  1

typedef struct Mesh
{
    std::vector<float> data;
    GLuint vao;
    GLuint vbo;
    Mesh(c_char* obj_path);
    Mesh(float* vertices, uint arr_size);
    ~Mesh();
} Mesh;
int  meshLoadObj(Mesh* mesh_p, c_char* path);
void meshCalcTangents(Mesh* mesh_p);
void meshDataToGPU(Mesh* mesh_p);

////////////////////
// Struct Texture //
////////////////////

typedef struct Texture
{
    std::vector<uchar> map;
    int    width;
    int    height;
    GLuint texture_id; 
    Texture(c_char* bmp_path);
    ~Texture();
} Texture;
int  textureLoadBmp(Texture* texture_p, c_char* path);
void textureDataToGPU(Texture* texture_p);

/////////////////////////
// Struct FrameTexture //
/////////////////////////

typedef struct FrameTexture
{
    GLuint fbo;
    GLuint color_text_id;
    GLuint depth_stencil_text_id;
    int    width;
    int    height;
    GLuint quad_vbo;
    GLuint quad_vao;
    float  quad_data[24] = {
	// Position  // UV coord
	-1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 1.0f, 1.0f
    };
    FrameTexture(int _width, int _height);
    ~FrameTexture();
} FrameTexture;
void frameTextureDataToGPU(FrameTexture* ftexture_p);

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
