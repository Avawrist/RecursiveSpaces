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

/////////////////
// Struct Mesh //
/////////////////

#define VERT_INDEX 0
#define UV_INDEX   1
#define NORM_INDEX 2
#define ELE_INDEX  3

typedef struct Mesh
{
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::vector<int>   vert_indices;
    std::vector<int>   text_indices; // TODO: Single index array for all attributes
    std::vector<int>   norm_indices;
    GLuint vao;
    GLuint buffers[4];
    Mesh(const char* obj_path);
    ~Mesh();
} Mesh;
int  meshLoadObj(Mesh* mesh_p, const char* path);
void meshDataToGPU(Mesh* mesh_p);

////////////////////
// Struct Texture //
////////////////////

typedef struct Texture
{
    unsigned int width;
    unsigned int height;
    GLuint texture; 
    std::vector<float> map;
    Texture(const char* bmp_path);
    ~Texture();
} Texture;
int  textureLoadBmp(Texture* texture_p, const char* path);
void textureDataToGPU(Texture* texture_p);

#endif
