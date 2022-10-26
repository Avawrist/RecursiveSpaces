// ======================================================================
// Title: asset.hpp
// Description: The header file for model, mesh & material objects
// ======================================================================

#ifndef ASSET_H
#define ASSET_H

// Win libs
#include <map>
#include <vector>
#include <stdio.h>
#include <windows.h>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// My libs
#include "mdcla.hpp"

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
    std::vector<unsigned char> map;
    int    width;
    int    height;
    GLuint texture_id; 
    Texture(const char* bmp_path);
    ~Texture();
} Texture;
int  textureLoadBmp(Texture* texture_p, const char* path);
void textureDataToGPU(Texture* texture_p);

GLenum glCheckError_(const char *file, int line);

#endif
