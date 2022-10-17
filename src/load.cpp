// ======================================================================
// Title: load.cpp
// Description: The source file for loading utility functions
// ======================================================================

#include "load.hpp"

int loadObjToMesh(const char* path, Mesh& mesh)
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
	    mesh.vertices.push_back(v_x);
	    mesh.vertices.push_back(v_y);
	    mesh.vertices.push_back(v_z);
	}
	else if(buf[0] == 'v' && buf[1] == 't') // Line of texture coords
	{
	    fscanf(file_p, " %f %f", &v_x, &v_y);
	    mesh.uvs.push_back(v_x);
	    mesh.uvs.push_back(v_y);
	}
	else if(buf[0] == 'v' && buf[1] == 'n') // Line of vector normals
	{
	    fscanf(file_p, " %f %f %f", &v_x, &v_y, &v_z);
	    mesh.normals.push_back(v_x);
	    mesh.normals.push_back(v_y);
	    mesh.normals.push_back(v_z);
	}
	else if(buf[0] == 'f') // Line of face indices
	{
	    // v/t/n
	    fscanf(file_p,
		   "%i/%i/%i %i/%i/%i %i/%i/%i",
		   &v_ind_x, &t_ind_x, &n_ind_x,
		   &v_ind_y, &t_ind_y, &n_ind_y,
		   &v_ind_z, &t_ind_z, &n_ind_z);
	    mesh.vert_indices.push_back(v_ind_x);
	    mesh.vert_indices.push_back(v_ind_y);
	    mesh.vert_indices.push_back(v_ind_z);
	    mesh.text_indices.push_back(t_ind_x);
	    mesh.text_indices.push_back(t_ind_y);
	    mesh.text_indices.push_back(t_ind_z);
	    mesh.norm_indices.push_back(n_ind_x);
	    mesh.norm_indices.push_back(n_ind_y);
	    mesh.norm_indices.push_back(n_ind_z);
	}
    }
    while(!feof(file_p));
    
    fclose(file_p);

    return 1;
}
