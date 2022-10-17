// ======================================================================
// Title: asset.hpp
// Description: The header file for model, mesh & material objects
// ======================================================================

#ifndef ASSET_H
#define ASSET_H

#include <vector>

//////////////////
// Struct Model //
//////////////////

enum modelType
{
    test1 = 0
};

/////////////////
// Struct Mesh //
/////////////////

typedef struct Mesh
{
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::vector<int>   vert_indices;
    std::vector<int>   text_indices;
    std::vector<int>   norm_indices;
} Mesh;

/////////////////////
// Struct Material //
/////////////////////

#endif
