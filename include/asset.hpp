// ======================================================================
// Title: asset.hpp
// Description: The header file for model, mesh & material objects
// ======================================================================

#ifndef ASSET_H
#define ASSET_H

// Win libs
#include <vector>
#include <stdio.h>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define VERT_INDEX 0
#define UV_INDEX   1
#define NORM_INDEX 2
#define ELE_INDEX  3

/////////////////
// Struct Mesh //
/////////////////

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
    Mesh();
    ~Mesh();
} Mesh;
int meshLoadObj(Mesh* mesh_p, const char* path);

/////////////////////
// Struct Material //
/////////////////////

#endif
