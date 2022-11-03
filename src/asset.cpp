// ======================================================================
// Title: asset.cpp
// Description: The header file for model, mesh & material objects
// ======================================================================

#include "asset.hpp"

/////////////////
// Struct Mesh //
/////////////////

Mesh::Mesh(c_char* obj_path)
{
    // Generate OpenGL objects
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
   
    // Load mesh data
    if(!meshLoadObj(this, obj_path))
    {
	OutputDebugStringA("ERROR: Failed to load mesh.\n");
    }

    // Add tangents to Mesh data vector
    meshCalcTangents(this);
}

Mesh::~Mesh()
{
    // Delete OpenGL objects:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

int meshLoadObj(Mesh* mesh_p, c_char* path)
{
    // Note: Assumes a pointer to .obj file path is passed.

    ///////////////////
    // Get File Data //
    ///////////////////    
    float v_x;
    float v_y;
    float v_z;
    int   v_ind;
    int   t_ind;
    int   n_ind;
    
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    
    c_int SIZE = 3;
    char  buf[SIZE];
    
    FILE* file_p;
    fopen_s(&file_p, path, "r");
    if(!file_p) {return 0;}
    
    do
    {
	// Get line identifier
	fgets(buf, SIZE, file_p);

	/////////////////////////////////////////////////
	// Read all data into 3 separate vectors first //
	/////////////////////////////////////////////////
	if(buf[0] == 'v' && buf[1] == ' ') // Line of geometry
	{
	    fscanf_s(file_p, "%f %f %f", &v_x, &v_y, &v_z);
	    vertices.push_back(v_x);
	    vertices.push_back(v_y);
	    vertices.push_back(v_z);
	}
	else if(buf[0] == 'v' && buf[1] == 't') // Line of texture coords
	{
	    fscanf_s(file_p, " %f %f", &v_x, &v_y);
	    uvs.push_back(v_x);
	    uvs.push_back(v_y);
	}
	else if(buf[0] == 'v' && buf[1] == 'n') // Line of vector normals
	{
	    fscanf_s(file_p, " %f %f %f", &v_x, &v_y, &v_z);
	    normals.push_back(v_x);
	    normals.push_back(v_y);
	    normals.push_back(v_z);
	}

	///////////////////////////
	// Fill mesh data vector //
	///////////////////////////
	else if(buf[0] == 'f') // Line of face indices
	{
	    for(int i = 0; i < 3; i++)
	    {
		// v/t/n
		fscanf_s(file_p, "%i/%i/%i", &v_ind, &t_ind, &n_ind);

		// Remove 1 from each index value to match OpenGL conventions:
		v_ind -= 1;
		t_ind -= 1;
		n_ind -= 1;
	        
		// Massage vertices, uvs and normals together into single data vector:
		
		// Vertices
		mesh_p->data.push_back(vertices[v_ind * 3]);
		mesh_p->data.push_back(vertices[(v_ind * 3) + 1]);
		mesh_p->data.push_back(vertices[(v_ind * 3) + 2]);
		// UVs
		mesh_p->data.push_back(uvs[t_ind * 2]);
		mesh_p->data.push_back(uvs[(t_ind * 2) + 1]);
		// Normals
		mesh_p->data.push_back(normals[n_ind * 3]);
		mesh_p->data.push_back(normals[(n_ind * 3) + 1]);
		mesh_p->data.push_back(normals[(n_ind * 3) + 2]);
	    }
	}
    }
    while(!feof(file_p));
    fclose(file_p);
    
    return 1;
}

void meshCalcTangents(Mesh* mesh_p)
{
    //   0       1       2      3     4      5       6      7
    //(vert.x, vert.y, vert.z, uv.x, uv.y, norm.x, norm.y, norm.z, 

    //  8       9       10     11    12     13     14      15    
    //vert.x, vert.y, vert.z, uv.x, uv.y, norm.x, norm.y, norm.z, 

    //  16      17      18     19    20     21      22      23
    //vert.x, vert.y, vert.z, uv.x, uv.y, norm.x, norm.y, norm.z)

    int   triangle_count = mesh_p->data.size() / 24; // 24 elements per triangle
    int   stride = 33; // 33 elements after insertion
    float tangent_vals[3];
    int   index;
	
    for(int i = 0; i < triangle_count; i++)
    {
	index = i * stride;
        
	// Prep Data
	Vec3F pos_1 = Vec3F(mesh_p->data[index],    mesh_p->data[index+1],  mesh_p->data[index+2]);
	Vec3F pos_2 = Vec3F(mesh_p->data[index+8],  mesh_p->data[index+9],  mesh_p->data[index+10]);
	Vec3F pos_3 = Vec3F(mesh_p->data[index+16], mesh_p->data[index+17], mesh_p->data[index+18]);

	Vec2F uv_1 = Vec2F(mesh_p->data[index+3],  mesh_p->data[index+4]);
	Vec2F uv_2 = Vec2F(mesh_p->data[index+11], mesh_p->data[index+12]);
	Vec2F uv_3 = Vec2F(mesh_p->data[index+19], mesh_p->data[index+20]);
	
	Vec3F edge_1     = pos_2 - pos_1;
	Vec3F edge_2     = pos_3 - pos_1;
	Vec2F delta_uv_1 = uv_2 - uv_1;
	Vec2F delta_uv_2 = uv_3 - uv_1;

	float f = 1.0f / (delta_uv_1.x * delta_uv_2.y - delta_uv_2.x * delta_uv_1.y);

	// Calculate Tangent
	tangent_vals[0] = f * (delta_uv_2.y * edge_1.x - delta_uv_1.y * edge_2.x);
	tangent_vals[1] = f * (delta_uv_2.y * edge_1.y - delta_uv_1.y * edge_2.y);
	tangent_vals[2] = f * (delta_uv_2.y * edge_1.z - delta_uv_1.y * edge_2.z);

	// Insert Tangent in Data
	mesh_p->data.insert(mesh_p->data.begin() + index + 8, tangent_vals, tangent_vals + 3);
	mesh_p->data.insert(mesh_p->data.begin() + index + 19, tangent_vals, tangent_vals + 3);
	mesh_p->data.insert(mesh_p->data.begin() + index + 30, tangent_vals, tangent_vals + 3);
    }
}

void meshDataToGPU(Mesh* mesh_p)
{
    ///////////////////////////////////
    // Configure OpenGL Mesh Objects //
    ///////////////////////////////////

    // Always bind VAO first
    glBindVertexArray(mesh_p->vao);

    // Mesh VBO
    glBindBuffer(GL_ARRAY_BUFFER, mesh_p->vbo);
    glBufferData(GL_ARRAY_BUFFER,
		 mesh_p->data.size() * sizeof(float),
		 mesh_p->data.data(),
		 GL_STATIC_DRAW);

    // Vertex Attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), 0);

    // UV Attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));

    // Normal Attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));

    // Tangent Attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    
    // Unbind buffer and VAO:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

////////////////////
// Struct Texture //
////////////////////

Texture::Texture(c_char* bmp_path)
{
    // Create OpenGL Texture object
    glGenTextures(1, &texture_id);
    
    // Load texture data
    if(!textureLoadBmp(this, bmp_path))
    {
	OutputDebugStringA("ERROR: Failed to load texture.\n");
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture_id); // TODO: add deletion for specular & normal maps
}

int textureLoadBmp(Texture* texture_p, c_char* path)
{
    ////////////////////////////
    // Get bmp data from file //
    ////////////////////////////
    uchar header[54];
    uint  data_pos; // Position in the file where the data begins
    uint  size;

    FILE* file_p;
    fopen_s(&file_p, path, "rb");
    if(!file_p) {return 0;}

    // Get Header 
    if(fread(header, 1, 54, file_p) != 54) {return 0;}
    if(header[0] != 'B' || header[1] != 'M') {return 0;}

    data_pos          = *(uint*)&(header[10]);
    texture_p->width  = *(int*)&(header[18]);
    texture_p->height = *(int*)&(header[22]);
    size              = texture_p->width * texture_p->height * 3;
    
    // Safety check if data_pos was not provided:
    if(data_pos == 0) {data_pos = 54;}

    // Read the image data from the bmp file:
    texture_p->map.resize(size);
    fread(texture_p->map.data(), 1, size, file_p);
    fclose(file_p);

    return 1;
}

void textureDataToGPU(Texture* texture_p)
{
    /////////////////////////////////////////////
    // Configure Diffuse Map Texture in OpenGL //
    /////////////////////////////////////////////
    glBindTexture(GL_TEXTURE_2D, texture_p->texture_id);
    glTexImage2D(GL_TEXTURE_2D,
		 0, // OpenGL to generate mipmaps at runtime
		 GL_RGB,
		 texture_p->width,
		 texture_p->height,
		 0,
		 GL_BGR, // .bmp stores RGB dat as BGR
		 GL_UNSIGNED_BYTE,
		 texture_p->map.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

/////////////////////////
// Struct FrameTexture //
/////////////////////////

FrameTexture::FrameTexture(int _width, int _height)
{
    //////////////////////////////
    // Initialize FBO & Texture //
    //////////////////////////////

    // FBO:
    glGenFramebuffers(1, &fbo);
    
    // Texture:
    glGenTextures(1, &color_text_id);
    glGenTextures(1, &depth_stencil_text_id);
    width  = _width;
    height = _height;
    
    ////////////////////
    // Initalize Quad //
    ////////////////////

    // Quad VAO:
    glGenVertexArrays(1, &quad_vao);

    // Quad VBO:
    glGenBuffers(1, &quad_vbo);
}

FrameTexture::~FrameTexture()
{
    // Delete FBO
    glDeleteFramebuffers(1, &fbo);
    
    // Delete texture
    glDeleteTextures(1, &color_text_id);
    glDeleteTextures(1, &depth_stencil_text_id);
    
    // Delete quad VAO
    glDeleteVertexArrays(1, &quad_vao);
    
    // Delete quad VBO
    glDeleteBuffers(1, &quad_vbo);
}

void frameTextureDataToGPU(FrameTexture* ftexture_p)
{
    //////////////////////////////////////
    // Config Framebuffer with Textures //
    //////////////////////////////////////
    
    glBindFramebuffer(GL_FRAMEBUFFER, ftexture_p->fbo);

    ///////////////////
    // Color Texture //
    ///////////////////
    
    // Allocate memory for color texture on GPU
    glBindTexture(GL_TEXTURE_2D, ftexture_p->color_text_id);
    glTexImage2D(GL_TEXTURE_2D,
		 0,
		 GL_RGB,
		 ftexture_p->width,
		 ftexture_p->height,
		 0,
		 GL_RGB,
		 GL_UNSIGNED_BYTE,
		 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach texture to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER,
			   GL_COLOR_ATTACHMENT0,
			   GL_TEXTURE_2D,
			   ftexture_p->color_text_id,
			   0);

    ///////////////////////////
    // Depth/Stencil Texture //
    ///////////////////////////
    
    // Allocate memory for depth texture on GPU
    glBindTexture(GL_TEXTURE_2D, ftexture_p->depth_stencil_text_id);
    glTexImage2D(GL_TEXTURE_2D,
		 0,
		 GL_DEPTH24_STENCIL8,
		 ftexture_p->width,
		 ftexture_p->height,
		 0,
		 GL_DEPTH_STENCIL,
		 GL_UNSIGNED_INT_24_8,
		 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach texture to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER,
			   GL_DEPTH_STENCIL_ATTACHMENT,
			   GL_TEXTURE_2D,
			   ftexture_p->depth_stencil_text_id,
			   0);
    			   
    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /////////////////////////
    // Configure VBO & VAO //
    /////////////////////////
    
    // Bind VAO first
    glBindVertexArray(ftexture_p->quad_vao);

    // Move quad data to QPU
    glBindBuffer(GL_ARRAY_BUFFER, ftexture_p->quad_vbo);
    glBufferData(GL_ARRAY_BUFFER,
		 sizeof(ftexture_p->quad_data),
		 ftexture_p->quad_data,
		 GL_STATIC_DRAW);

    // Vertex Attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    
    // UV Attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    // Unbind buffer and VAO:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void frameTextureDraw(FrameTexture* ftexture_p, Shader* shader_p)
{
    // Set proper state to render
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // bind the default framebuffer
    glClear(GL_COLOR_BUFFER_BIT); // clear screen
    glDisable(GL_DEPTH_TEST); // Disable so quad is visible
    glUseProgram(shader_p->program_id); // Set post-process shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ftexture_p->color_text_id); // Bind color texture to post-process shader
    glBindVertexArray(ftexture_p->quad_vao); // Bind quad vao

    glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices in the quad data
}
