// ==========================================================================
// Title: draw.cpp
// Description: The source file for the draw functions, built on OpenGL
// ==========================================================================

#include "draw.hpp"

// Struct DebugLine //

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

void
debugLineDraw(DebugLine* line_p, Shader* shader_p, Vec3F color, float size)
{
    glUseProgram(shader_p->program_id);
    shaderAddVec3Uniform(shader_p, "color", color);
    glLineWidth(size);
    glBindVertexArray(line_p->mesh_p->vao);
    glDrawArrays(GL_LINES, 0, (GLsizei)line_p->mesh_p->data.size());
}

// Struct DebugGrid //

DebugGrid::DebugGrid(float _square_width, uint _width_ct, uint _length_ct, Vec3F ul_corner)
{
    sq_width  = _square_width;
    width_ct  = _width_ct;
    length_ct = _length_ct;
    
    // Allocate w_line_arr with width_ct lines, with sq_width between them
    int w_arr_size = width_ct * sizeof(DebugLine*);
    w_line_arr = (DebugLine**)malloc(w_arr_size);
    memset(w_line_arr, 0, w_arr_size);	
    if(!w_line_arr) {OutputDebugStringA("ERROR - Failed to allocate DebugGrid memory.\n");}

    for(uint i = 0; i < width_ct; i++)
    {
	Vec3F a = Vec3F(ul_corner.x + (i * sq_width), ul_corner.y, ul_corner.z);
	Vec3F b = Vec3F(ul_corner.x + (i * sq_width), ul_corner.y, ul_corner.z + ((length_ct - 1) * sq_width));
        w_line_arr[i] = new DebugLine(a, b);
	if(!w_line_arr[i])
	{
	    OutputDebugStringA("ERROR - Failed to allocate DebugLine for DebugGrid.\n");
	}
    }

    // Allocate l_line_arr with length_ct lines, with sq_width between them.
    int l_arr_size = length_ct * sizeof(DebugLine*);
    l_line_arr = (DebugLine**)malloc(l_arr_size);
    memset(l_line_arr, 0, l_arr_size);
    if(!l_line_arr) {OutputDebugStringA("ERROR - Failed to allocate DebugGrid memory\n");}

    for(uint i = 0; i < length_ct; i++)
    {
	Vec3F a = Vec3F(ul_corner.x, ul_corner.y, ul_corner.z + (i * sq_width));
	Vec3F b = Vec3F(ul_corner.x + ((width_ct - 1) * sq_width), ul_corner.y, ul_corner.z + (i * sq_width));
	l_line_arr[i] = new DebugLine(a, b);
	if(!l_line_arr[i])
	{
	    OutputDebugStringA("ERROR - Failed to allocate DebugLine for DebugGrid.\n");
	}
    }
}

DebugGrid::~DebugGrid()
{
    for(uint i = 0; i < width_ct; i++)
    {
	delete w_line_arr[i];
    }
    delete w_line_arr;
    
    for(uint i = 0; i < length_ct; i++)
    {
	delete l_line_arr[i];
    }
    delete l_line_arr;
}

void
debugGridDraw(DebugGrid* grid_p, Shader* shader_p, Vec3F color, float size)
{
    glUseProgram(shader_p->program_id);
    shaderAddVec3Uniform(shader_p, "color", color);
    glLineWidth(size);

    // Draw width lines:
    for(uint i = 0; i < grid_p->width_ct; i ++)
    {
	glBindVertexArray(grid_p->w_line_arr[i]->mesh_p->vao);
	glDrawArrays(GL_LINES, 0, (GLsizei)grid_p->w_line_arr[i]->mesh_p->data.size());
    }
    
    // Draw length lines:
    for(uint i = 0; i < grid_p->length_ct; i++)
    {
	glBindVertexArray(grid_p->l_line_arr[i]->mesh_p->vao);
	glDrawArrays(GL_LINES, 0, (GLsizei)grid_p->l_line_arr[i]->mesh_p->data.size());
    }
}
