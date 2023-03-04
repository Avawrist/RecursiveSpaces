// ==========================================================================
// Title: draw.cpp
// Description: The source file for the draw functions, built on OpenGL
// ==========================================================================

#include "draw.hpp"

//////////////////////
// Struct DebugLine //
//////////////////////

DebugLine::DebugLine(Vec3F start, Vec3F end)
{
    a = start;
    b = end;
  
    // Convert Vec3Fs to array
    uint  n = 6;
    float arr[] = {
	a.x, a.y, a.z,
	b.x, b.y, b.z
    };
    
    // Create mesh object using array
    mesh_p = new Mesh(arr, n);
    if(!mesh_p) {OutputDebugStringA("ERROR: Failed to create DebugLine mesh.\n");}
}

DebugLine::~DebugLine()
{
    delete mesh_p;
}

void debugLineDraw(DebugLine* line_p, Shader* shader_p, Vec3F color, float size)
{
    glUseProgram(shader_p->program_id);
    shaderAddVec3Uniform(shader_p, "color", color);
    glLineWidth(size);
    glBindVertexArray(line_p->mesh_p->vao);
    glDrawArrays(GL_LINES, 0, (GLsizei)line_p->mesh_p->data.size());
}

/////////////////
// Struct Grid //
/////////////////
/*
Grid::~Grid()
{
    delete w_line_arr;
    delete l_line_arr;
    } */
