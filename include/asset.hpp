// ======================================================================
// Title: asset.hpp
// Description: The header file for model, mesh & material objects
// ======================================================================

#ifndef ASSET_H
#define ASSET_H

// Win libs
#include <vector>
#include <stdio.h>
#include <windows.h>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"
#include "mdcla.hpp"
#include "shader.hpp"
#include "game_window.hpp"

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
void frameTextureRender(FrameTexture* ftexture_p, GameWindow& game_window, Shader* shader_p);

#endif
