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
#include "game_window.hpp"
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

//////////////////////
// Struct DebugGrid //
//////////////////////

typedef struct DebugGrid
{
    float sq_width;
    uint  width_ct;
    uint  length_ct;
    Vec3F center;
    DebugLine** w_line_arr = NULL;
    DebugLine** l_line_arr = NULL;
    DebugGrid(float _square_width, uint _width_ct, uint _length_ct, Vec3F ul_corner);
    ~DebugGrid();
} DebugGrid;
void debugGridDraw(DebugGrid* grid_p, Shader* shader_p, Vec3F color, float size);

#endif
