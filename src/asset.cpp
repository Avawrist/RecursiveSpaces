// ======================================================================
// Title: asset.cpp
// Description: The header file for model, mesh & material objects
// ======================================================================

#include <asset.hpp>

/////////////////
// Struct Mesh //
/////////////////

Mesh::Mesh(const char* obj_path)
{
    // Generate OpenGL objects
    glGenVertexArrays(1, &vao);
    glGenBuffers(4, buffers);
   
    // Load mesh data
    if(!meshLoadObj(this, obj_path))
    {
	OutputDebugStringA("ERROR: Failed to load mesh.\n");
    }
 }

Mesh::~Mesh()
{
    // Delete OpenGL objects:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(4, buffers);
}

int meshLoadObj(Mesh* mesh_p, const char* path)
{
    // Note: Assumes a pointer to .obj file path is passed.

    ///////////////////
    // Get File Data //
    ///////////////////    
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

void meshDataToGPU(Mesh* mesh_p)
{
    ///////////////////////////////////
    // Configure OpenGL Mesh Objects //
    ///////////////////////////////////

    // Always bind VAO first
    glBindVertexArray(mesh_p->vao);

    // Vertex VBO
    glBindBuffer(GL_ARRAY_BUFFER, mesh_p->buffers[VERT_INDEX]);
    glBufferData(GL_ARRAY_BUFFER,
		 mesh_p->vertices.size() * sizeof(float),
		 mesh_p->vertices.data(),
		 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // UV VBO
    glBindBuffer(GL_ARRAY_BUFFER, mesh_p->buffers[UV_INDEX]);
    glBufferData(GL_ARRAY_BUFFER,
		 mesh_p->uvs.size() * sizeof(float),
		 mesh_p->uvs.data(),
		 GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal VBO
    glBindBuffer(GL_ARRAY_BUFFER, mesh_p->buffers[NORM_INDEX]);
    glBufferData(GL_ARRAY_BUFFER,
		 mesh_p->normals.size() * sizeof(float),
		 mesh_p->normals.data(),
		 GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // EBO // TODO: Consolidate all index buffers into single buffer here,
    //              i.e., replace vert_indices with indices array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_p->buffers[ELE_INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 mesh_p->vert_indices.size() * sizeof(int),
		 mesh_p->vert_indices.data(),
		 GL_STATIC_DRAW);

    // Unbind buffers and VAO:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

////////////////////
// Struct Texture //
////////////////////

Texture::Texture(const char* bmp_path)
{
    // Create OpenGL Texture object
    glGenTextures(1, textures); // TODO: Add generation for normal & spec maps

    // Load texture data
    if(!textureLoadBmp(this, bmp_path))
    {
	OutputDebugStringA("ERROR: Failed to load texture.\n");
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, textures); // TODO: add deletion for specular & normal maps
}

int textureLoadBmp(Texture* texture_p, const char* path)
{
    ////////////////////////////
    // Get bmp data from file //
    ////////////////////////////
    unsigned char header[54];
    unsigned int  data_pos; // Position in the file where the data begins
    unsigned int  size;     // width * height * 3

    
    FILE* file_p = fopen(path, "rb");
    if(!file_p) {return 0;}

    if(fread(header, 1, 54, file_p) != 54) {return 0;}
    if(header[0] != 'B' || header[1] != 'M') {return 0;}

    data_pos = ;
    size     = ;
    width    = ;
    height   = ;
}

void textureDataToGPU(Texture* texture_p)
{
    /////////////////////////////////////////////
    // Configure Diffuse Map Texture in OpenGL //
    /////////////////////////////////////////////
    glBindTexture(GL_TEXTURE_2D, texture_p->texture[D_MAP_INDEX]);
    glTexImage2D(GL_TEXTURE_2D,
		 0,
		 GL_RGB,
		 texture_p->width,
		 texture_p->height,
		 0,
		 GL_BGR, // .bmp stores RGB dat as BGR
		 GL_UNSIGNED_BYTE,
		 texture_p->diffuse_map.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

