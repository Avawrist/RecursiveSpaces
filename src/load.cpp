// ======================================================================
// Title: load.cpp
// Description: The source file for loading utility functions
// ======================================================================

#include "load.hpp"

void loadObjToMesh(const char* path, Mesh& mesh)
{
    // Assumes a pointer to .obj file path is passed. 
    
    FILE* file_p;
    file_p = fopen(path, "r");
    
    const int SIZE = 3;
    char buf[SIZE];
    do
    {
	fgets(buf, SIZE, file_p);

	float v_x;
	float v_y;
	float v_z;
	
	if(buf[0] == 'v') // Line of geometry
	{
	    fscanf(file_p, "%f %f %f", &v_x, &v_y, &v_z);
	    mesh.vertices.push_back(v_x);
	    mesh.vertices.push_back(v_y);
	    mesh.vertices.push_back(v_z);
	}
	else if(buf[0] == 'v' && buf[1] == 't') // Line of texture coords
	{
	    fscanf(file_p, "%f %f", &v_x, &v_y);
	    mesh.uvs.push_back(v_x);
	    mesh.uvs.push_back(v_y);
	}
	else if(buf[0] == 'v' && buf[1] == 'n') // Line of vector normals
	{
	    fscanf(file_p, "%f %f %f", &v_x, &v_y, &v_z);
	    mesh.normals.push_back(v_x);
	    mesh.normals.push_back(v_y);
	    mesh.normals.push_back(v_z);
	}
    }
    while(!feof(file_p));
    
    fclose(file_p);
}
