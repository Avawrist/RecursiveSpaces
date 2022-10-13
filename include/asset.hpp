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
    test = 0
};

/////////////////
// Struct Mesh //
/////////////////

typedef struct Mesh
{
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
} Mesh;

/////////////////////
// Struct Material //
/////////////////////

#endif
