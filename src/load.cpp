// ======================================================================
// Title: load.cpp
// Description: The source file for loading utility functions
// ======================================================================

#include "load.hpp"

int loadObjToMesh(const char* path, Mesh* mesh_p)
{
    // Assumes a pointer to .obj file path is passed.
    float v_x;
    float v_y;
    float v_z;
    int   v_ind_x;
    int   v_ind_y;
    int   v_ind_z;
    int   t_ind_x;
    int   t_ind_y;
    int   t_ind_z;
    int   n_ind_x;
    int   n_ind_y;
    int   n_ind_z;
    
    FILE* file_p;
    file_p = fopen(path, "r");
    if(!file_p) {return 0;}
    
    const int SIZE = 3;
    char buf[SIZE];
    do
    {
	fgets(buf, SIZE, file_p);
	
	if(buf[0] == 'v' && buf[1] == ' ') // Line of geometry
	{
	    fscanf(file_p, "%f %f %f", &v_x, &v_y, &v_z);
	    mesh_p->vertices.push_back(v_x);
	    mesh_p->vertices.push_back(v_y);
	    mesh_p->vertices.push_back(v_z);
	}
	else if(buf[0] == 'v' && buf[1] == 't') // Line of texture coords
	{
	    fscanf(file_p, " %f %f", &v_x, &v_y);
	    mesh_p->uvs.push_back(v_x);
	    mesh_p->uvs.push_back(v_y);
	}
	else if(buf[0] == 'v' && buf[1] == 'n') // Line of vector normals
	{
	    fscanf(file_p, " %f %f %f", &v_x, &v_y, &v_z);
	    mesh_p->normals.push_back(v_x);
	    mesh_p->normals.push_back(v_y);
	    mesh_p->normals.push_back(v_z);
	}
	else if(buf[0] == 'f') // Line of face indices
	{
	    // v/t/n
	    fscanf(file_p,
		   "%i/%i/%i %i/%i/%i %i/%i/%i",
		   &v_ind_x, &t_ind_x, &n_ind_x,
		   &v_ind_y, &t_ind_y, &n_ind_y,
		   &v_ind_z, &t_ind_z, &n_ind_z);
	    // Add index value minus 1 to convert from Blender convention (1 to n)
	    // to OpenGl convention (0 to n-1):
	    mesh_p->vert_indices.push_back(v_ind_x - 1);
	    mesh_p->vert_indices.push_back(v_ind_y - 1);
	    mesh_p->vert_indices.push_back(v_ind_z - 1);
	    mesh_p->text_indices.push_back(t_ind_x - 1);
	    mesh_p->text_indices.push_back(t_ind_y - 1);
	    mesh_p->text_indices.push_back(t_ind_z - 1);
	    mesh_p->norm_indices.push_back(n_ind_x - 1);
	    mesh_p->norm_indices.push_back(n_ind_y - 1);
	    mesh_p->norm_indices.push_back(n_ind_z - 1);
	}
    }
    while(!feof(file_p));
    
    fclose(file_p);

    return 1;
}
