// ==========================================================================
// Title: draw.hpp
// Description: The header file for the draw functions, built on OpenGL
// ==========================================================================

#ifndef DRAW_H
#define DRAW_H

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"
#include "asset.hpp"
#include "render.hpp"
#include "mdcla.hpp"

//////////////////////
// Struct DebugLine //
//////////////////////

typedef struct DebugLine
{
    Mesh* mesh_p = NULL;
    Vec3F a;
    Vec3F b;
    DebugLine(Vec3F start, Vec3F end);
    ~DebugLine();
} DebugLine;
void debugLineDraw(DebugLine* line_p, Shader* shader_p, Vec3F color, float size);

/////////////////
// Struct Grid //
/////////////////
/*
typedef struct Grid
{
    Vec3F color;
    float line_size;
    float square_width;
    uint  width_ct;
    uint  length_ct;
    DebugLine* w_line_arr;
    DebugLine* l_line_arr;
    Grid(Vec3F _color, float _square_width, uint _width_ct, uint _length_ct, float _line_size);
    ~Grid();
} Grid;
*/
#endif
