// ======================================================================
// Title: asset.cpp
// Description: The source file for model, mesh & material objects
//              All assets are built on platform dependent code.
// ======================================================================

#include "asset.hpp"

///////////////////////////
// Struct SoundInterface //
///////////////////////////

SoundInterface::SoundInterface()
{
    interface_p    = NULL;
    master_voice_p = NULL;
    
    // Init xaudio interface
    XAudio2Create(&interface_p, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if(!interface_p)
    {
	OutputDebugStringA("Failed to initialize XAudio2 interface.\n");
    }
	
    // Init sound master
    interface_p->CreateMasteringVoice(&master_voice_p);
    if(!master_voice_p)
    {
	OutputDebugStringA("Failed to initialize XAudio2 Master Voice\n");
    }

    // Start XAudio2 engine
    interface_p->StartEngine();
}

SoundInterface::~SoundInterface()
{
    // Uninitialize COM library
    CoUninitialize();

    // Stop the XAudio2 engine
    interface_p->StopEngine();
}

//////////////////
// Struct Sound //
//////////////////

Sound::Sound(c_char* wav_path, SoundInterface& sound_interface)
{
    // Load audio data to buffer
    if(soundLoadWav(this, wav_path))
    {
	// Create source voice
	sound_interface.interface_p->CreateSourceVoice(&source_voice_p,
						      &waveFormat);
	if(!source_voice_p)
	{
	    OutputDebugStringA("ERROR: Failed to create source voice.\n");
	}

	return; // Success
    }
    
    OutputDebugStringA("\nERROR: Failed to load .wav file. Asset not found.\n"); // Failure
}

Sound::~Sound()
{
    soundStop(this);
}

int soundLoadWav(Sound* sound, c_char* wav_path)
{
    // Open file
    FILE* file_p = NULL;
    fopen_s(&file_p, wav_path, "rb");
    if(!file_p) {return 0;}
    
    // Verify first ChunkID is "RIFF" format
    char chunk_ID[5];
    chunk_ID[4] = '\0';
    fread(chunk_ID, sizeof(char), 4, file_p);
    if(strcmp(chunk_ID, "RIFF") != 0) {return 0;}

    //////////////////////////////////
    // Populate WAVEFORMATEX struct //
    //////////////////////////////////

    // Skip ChunkSize (4), Format (4), SubchunkID (4),
    // Subchunk1Size (4), AudioFormat (2)
    fseek(file_p, 18, SEEK_CUR);
    // Read NumChannels
    fread(&sound->waveFormat.nChannels, sizeof(WORD), 1, file_p);
    // Read SampleRate
    fread(&sound->waveFormat.nSamplesPerSec, sizeof(DWORD), 1, file_p);
    // Read ByteRate
    fread(&sound->waveFormat.nAvgBytesPerSec, sizeof(DWORD), 1, file_p);
    // Read BlockAlign
    fread(&sound->waveFormat.nBlockAlign, sizeof(WORD), 1, file_p);
    // Read Bits Per Sample
    fread(&sound->waveFormat.wBitsPerSample, sizeof(WORD), 1, file_p);
    // FormatTag
    sound->waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    // cbSize
    sound->waveFormat.cbSize = 0;

    ////////////////////////////////////
    // Populate XAUDIO2_BUFFER struct //
    ////////////////////////////////////
    
    // Skip Subchunk2ID (4)
    fseek(file_p, 4, SEEK_CUR);
    // Read Subchunk2Size
    fread(&sound->buffer.AudioBytes, sizeof(uint), 1, file_p);
    // Read Data
    BYTE *buffer = new BYTE[sound->buffer.AudioBytes]; 
    fread((void *)buffer, sizeof(char), sound->buffer.AudioBytes, file_p);
    sound->buffer.pAudioData = buffer;
    sound->buffer.Flags = XAUDIO2_END_OF_STREAM;
    sound->buffer.PlayBegin = 0;
    sound->buffer.PlayLength = sound->waveFormat.nSamplesPerSec *
	                       (sound->buffer.AudioBytes / sound->waveFormat.nAvgBytesPerSec);
    // Close file
    fclose(file_p);
    
    return 1;
}

int soundPlay(Sound* sound)
{
    // Check buffer state
    XAUDIO2_VOICE_STATE state_p; 
    sound->source_voice_p->GetState(&state_p);
    if(!&state_p)
    {
	OutputDebugStringA("\nERROR: Failed to get buffer state from XAudio2.\n");
	return 0;
    }

    if(state_p.BuffersQueued == 0)
    {
	// Submit buffer to the source voice
	HRESULT hr;
	hr = sound->source_voice_p->SubmitSourceBuffer(&(sound->buffer));
	if(FAILED(hr))
	{
	    OutputDebugStringA("\nERROR: Failed to submit audio buffer to source voice.\n");
	    return 0;
	}
    }
    
    sound->source_voice_p->Start(0);
    return 1;
}

void soundPause(Sound* sound)
{
    sound->source_voice_p->Stop();
}

void soundStop(Sound* sound)
{
    sound->source_voice_p->Stop();
    sound->source_voice_p->FlushSourceBuffers();
}

void soundSetVolume(Sound* sound, int volume)
{
    // Assert value is >= 0 and <= 100
    _assert(volume >= 0 && volume <= 100);
    
    // Takes a value between 0 and 100, converts it to a float scale of 0 to 1.
    float value = (float)(volume * 0.01); 
    sound->source_voice_p->SetVolume(value);
}

////////////////////////
// Struct SoundStream //
////////////////////////

SoundStream::SoundStream(c_char* wav_path, SoundInterface& sound_interface)
{
    // Init member variables
    bytes_read = 0;
    cw_buffer  = 0;
    file_p = NULL;
    
    //Populate XAudio 2 structs:
    if(!soundStreamReadWavHeader(this, wav_path))
    {
	OutputDebugStringA("ERROR: Unable to populate XAudio2 structs.\n");
    }
    
    // Create source voice
    sound_interface.interface_p->CreateSourceVoice(&source_voice_p,
						  &waveFormat);
    if(!source_voice_p)
    {
	OutputDebugStringA("ERROR: Failed to create source voice.\n");
    }
}

SoundStream::~SoundStream()
{
    fclose(file_p);
    soundStreamStop(this);
}

int soundStreamReadWavHeader(SoundStream* sound_stream, c_char* wav_path)
{
    // Open file
    fopen_s(&sound_stream->file_p, wav_path, "rb");
    if(!sound_stream->file_p) {return 0;}
    
    // Verify first ChunkID is "RIFF" format
    char chunk_ID[5];
    chunk_ID[4] = '\0';
    fread(chunk_ID, sizeof(char), 4, sound_stream->file_p);
    if(strcmp(chunk_ID, "RIFF") != 0) {return 0;}

    //////////////////////////////////
    // Populate WAVEFORMATEX struct //
    //////////////////////////////////

    // Skip ChunkSize (4), Format (4), SubchunkID (4),
    // Subchunk1Size (4), AudioFormat (2)
    fseek(sound_stream->file_p, 18, SEEK_CUR);
    // Read NumChannels
    fread(&sound_stream->waveFormat.nChannels, sizeof(shint), 1, sound_stream->file_p);
    // Read SampleRate
    fread(&sound_stream->waveFormat.nSamplesPerSec, sizeof(uint), 1, sound_stream->file_p);
    // Read ByteRate
    fread(&sound_stream->waveFormat.nAvgBytesPerSec, sizeof(uint), 1, sound_stream->file_p);
    // Read BlockAlign
    fread(&sound_stream->waveFormat.nBlockAlign, sizeof(shint), 1, sound_stream->file_p);
    // Read Bits Per Sample
    fread(&sound_stream->waveFormat.wBitsPerSample, sizeof(shint), 1, sound_stream->file_p);
    // FormatTag
    sound_stream->waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    // cbSize
    sound_stream->waveFormat.cbSize = 0;

    ////////////////////////////////////
    // Populate XAUDIO2_BUFFER struct //
    ////////////////////////////////////
    
    // Skip Subchunk2ID (4)
    fseek(sound_stream->file_p, 4, SEEK_CUR);
    // Read Subchunk2Size into total_bytes
    fread(&sound_stream->total_bytes, sizeof(uint), 1, sound_stream->file_p);
    // Set AudioBytes to correct buffer size
    sound_stream->buffer.AudioBytes = BUFFER_SIZE;

    // Upate bytes_read to account for header
    sound_stream->bytes_read = 44;

    return 1;
}

int soundStreamUpdate(SoundStream* sound_stream)
{
    // Get sound state from XAudio2
    XAUDIO2_VOICE_STATE state_p; 
    sound_stream->source_voice_p->GetState(&state_p);
    if(!&state_p)
    {
	OutputDebugStringA("\nERROR: Failed to get buffer state from XAudio2.\n");
	return 0;
    }
    
    // Fill and submit buffers if they are unqueued
    if(state_p.BuffersQueued < NUM_BUFFERS && sound_stream->buffer.Flags != XAUDIO2_END_OF_STREAM)
    {
	// Offset file_p by amount of data already read
	fseek(sound_stream->file_p, sound_stream->bytes_read, SEEK_SET);
	
	// Determine amount to read on this pass
	uint bytes_left = sound_stream->total_bytes - sound_stream->bytes_read;
	uint bytes_to_read = min(bytes_left, BUFFER_SIZE);

	// If final pass, fill buffer with 0s and mark end of stream
	if(bytes_to_read < BUFFER_SIZE)
	{
	    memset(sound_stream->buffers[sound_stream->cw_buffer], 0, BUFFER_SIZE);
	    sound_stream->buffer.Flags = XAUDIO2_END_OF_STREAM;
	}
	
        // Fill unqueued buffer with data from the file
	fread((void *)&(sound_stream->buffers[sound_stream->cw_buffer]),
	      bytes_to_read, 1, sound_stream->file_p);

	// Submit buffer to queue
	sound_stream->buffer.pAudioData = sound_stream->buffers[sound_stream->cw_buffer];
        HRESULT hr;
	hr = sound_stream->source_voice_p->SubmitSourceBuffer(&(sound_stream->buffer));
	if(FAILED(hr))
	{
	    OutputDebugStringA("\nERROR: Failed to submit audio buffer to source voice.\n");
	    return 0;
	}

	// Update bytes_read for next pass
	sound_stream->bytes_read += bytes_to_read;

	// Update current write buffer for next pass
	sound_stream->cw_buffer++;
	sound_stream->cw_buffer %= NUM_BUFFERS;
    }
    else if(state_p.BuffersQueued == 0)
    {
	soundStreamStop(sound_stream);
    }

    return 1;
}

void soundStreamPlay(SoundStream* sound_stream)
{
    sound_stream->buffer.Flags = 0;
    sound_stream->source_voice_p->Start(0);
}

void soundStreamPause(SoundStream* sound_stream)
{
    // TODO: test
    sound_stream->source_voice_p->Stop();
}

void soundStreamStop(SoundStream* sound_stream)
{
    // TODO: test
    sound_stream->source_voice_p->Stop();
    // Clear XAudio2's queued buffers
    sound_stream->source_voice_p->FlushSourceBuffers();
    // Clear our temp buffers
    for(int i = 0; i < NUM_BUFFERS; i++)
    {
	memset(sound_stream->buffers[i], 0, BUFFER_SIZE);
    }
    // Reset our tracking values
    sound_stream->bytes_read = 44;
    sound_stream->cw_buffer  = 0;
}

void soundSetVolume(SoundStream* sound_stream, int volume)
{
    // Assert value is >= 0 and <= 100
    _assert(volume >= 0 && volume <= 100);    

    // Takes a value between 0 and 100, converts it to a float scale of 0 to 1.
    float value = (float)(volume * 0.01); 
    sound_stream->source_voice_p->SetVolume(value);
}

///////////////////
// Struct Shader //
///////////////////

Shader::Shader(c_char* _vert_path, c_char* _frag_path)
{
    // Create Shader Program
    program_id = glCreateProgram();

    //////////////////////////
    // Create Vertex Shader //
    //////////////////////////

    // Init values & create shader
    vert_id = glCreateShader(GL_VERTEX_SHADER);
    
    // Convert file into NULL terminated const char* from path
    c_char*           pShaderCodeV = NULL;
    std::string       sShaderCodeV;
    std::ifstream     shaderFileV;
    std::stringstream shaderStreamV;

    shaderFileV.open(_vert_path, std::ifstream::in);
    shaderStreamV << shaderFileV.rdbuf();
    shaderFileV.close();
    sShaderCodeV = shaderStreamV.str();
    pShaderCodeV = sShaderCodeV.c_str();
    if(!pShaderCodeV) {OutputDebugStringA("ERROR: VERTEX SHADER FILE NOT READ\n");}

    glShaderSource(vert_id, 1, &pShaderCodeV, NULL); // Assign the shader code with the shader obj

    // Compile
    glCompileShader(vert_id);
    GLint success = 0;
    glGetShaderiv(vert_id, GL_COMPILE_STATUS, &success);
    if(success)
	OutputDebugStringA("SUCCESS: VERTEX SHADER COMPILED\n");
    else
    {
	// TO-DO: Output error log info from glGetInfoLog 
	OutputDebugStringA("ERROR: VERTEX SHADER FAILED TO COMPILE\n");
	glDeleteShader(vert_id);
    }
    
    ////////////////////////////
    // Create Fragment Shader //
    ////////////////////////////

    // Init values & create shader
    frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Convert file into NULL terminated const char* from path
    c_char*           pShaderCodeF;
    std::string       sShaderCodeF;
    std::ifstream     shaderFileF;
    std::stringstream shaderStreamF;

    shaderFileF.open(_frag_path, std::ifstream::in);
    shaderStreamF << shaderFileF.rdbuf();
    shaderFileF.close();
    sShaderCodeF = shaderStreamF.str();
    pShaderCodeF = sShaderCodeF.c_str();

    if(!pShaderCodeF) {OutputDebugStringA("ERROR: FRAGMENT SHADER FILE NOT READ\n");}
    glShaderSource(frag_id, 1, &pShaderCodeF, NULL); // Assign the shader code with the shader obj

    // Compile
    glCompileShader(frag_id);
    success = 0;
    glGetShaderiv(frag_id, GL_COMPILE_STATUS, &success);
    if(success)
	OutputDebugStringA("SUCCESS: FRAGMENT SHADER COMPILED\n");
    else
    {
	// TO-DO: Output error log info from glGetInfoLog 
	OutputDebugStringA("ERROR: FRAGMENT SHADER FAILED TO COMPILE\n");
	glDeleteShader(frag_id);
    }

    ///////////////////////////////////
    // Attach shaders & link program //
    ///////////////////////////////////
    
    glAttachShader(program_id, vert_id);
    glAttachShader(program_id, frag_id);
    glLinkProgram(program_id);
    GLint linked = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
    if(linked)
	OutputDebugStringA("SUCCESS: SHADER PROGRAM LINKED\n");
    else
	// TO-DO: Output error info from glGetProgramInfoLog
	OutputDebugStringA("ERROR: SHADER PROGRAM FAILED TO LINK\n");

    /////////////
    // Cleanup //
    /////////////
    
    glDeleteShader(vert_id);
    glDeleteShader(frag_id);
}

Shader::~Shader()
{
    glDetachShader(program_id, vert_id);
    glDetachShader(program_id, frag_id);
    glDeleteProgram(program_id);
}

void shaderAddMat4Uniform(const Shader* shader_p, c_char* name, c_float* m)
{
    int loc = glGetUniformLocation(shader_p->program_id, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, m);
}

void shaderAddVec3Uniform(const Shader* shader_p, c_char* name, const Vec3F& v)
{
    int loc = glGetUniformLocation(shader_p->program_id, name);
    glUniform3fv(loc, 1, &v[0]);
}

void shaderAddIntUniform(const Shader* shader_p, c_char* name, int i)
{
    int loc = glGetUniformLocation(shader_p->program_id, name);
    glUniform1i(loc, i);    
}

void shaderAddFloatUniform(const Shader* shader_p, c_char* name, float f)
{
    int loc = glGetUniformLocation(shader_p->program_id, name);
    glUniform1f(loc, f);    
}

/////////////////
// Struct Mesh //
/////////////////

Mesh::Mesh(c_char* obj_path)
{
    // Generate OpenGL objects
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
   
    // Load mesh data
    if(meshLoadObj(this, obj_path))
    {
	// Add tangents to Mesh data vector
	meshCalcTangents(this);

	// Copy mesh data to the GPU
	meshDataToGPU(this);
	return; // Success
    }

    OutputDebugStringA("ERROR: Failed to load mesh. Asset not found.\n"); // Failure
}

Mesh::Mesh(float* vertices, uint arr_size)
{
    // Generate OpenGL objects
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Copy vertex array into data vector
    data.insert(data.end(), vertices, vertices + arr_size);

    ///////////////////////////////
    // Copy mesh data to the GPU //
    ///////////////////////////////
    
    // Always bind VAO first
    glBindVertexArray(vao);

    // Mesh VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
		 data.size() * sizeof(float),
		 data.data(),
		 GL_STATIC_DRAW);

    // Vertex Attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Unbind buffer and VAO:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    // Delete OpenGL objects:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

int meshLoadObj(Mesh* mesh_p, c_char* path)
{
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

    int   triangle_count = int(mesh_p->data.size() / 24); // 24 elements per triangle
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
    // Init member variables
    width  = 0;
    height = 0;
    
    // Create OpenGL Texture object
    glGenTextures(1, &texture_id);
    
    // Load texture data
    if(textureLoadBmp(this, bmp_path))
    {
	// Copy texture data to GPU
	textureDataToGPU(this);
	return; // Success
    }
    
    OutputDebugStringA("ERROR: Failed to load texture. Asset not found.\n"); // Failure
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

FrameTexture::FrameTexture(int _width, int _height, bool is_depth_map)
{
    //////////////////////////////
    // Initialize FBO & Texture //
    //////////////////////////////

    // FBO:
    glGenFramebuffers(1, &fbo);

    // Texture:
    if(is_depth_map)
    {
	glGenTextures(1, &depth_text_id);
    }
    else
    {
	glGenTextures(1, &color_text_id);
	glGenTextures(1, &depth_stencil_text_id);
    }

    // Dimensions
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
    glDeleteTextures(1, &depth_text_id);
    
    // Delete quad VAO
    glDeleteVertexArrays(1, &quad_vao);
    
    // Delete quad VBO
    glDeleteBuffers(1, &quad_vbo);
}

void frameTextureDataToGPU(FrameTexture* ftexture_p, bool is_depth_map)
{
    //////////////////////////////////////
    // Config Framebuffer with Textures //
    //////////////////////////////////////
    
    glBindFramebuffer(GL_FRAMEBUFFER, ftexture_p->fbo);

    if(is_depth_map)
    {
	///////////////////
	// Depth Texture //
	///////////////////

	glBindTexture(GL_TEXTURE_2D, ftexture_p->depth_text_id);
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_DEPTH_COMPONENT,
		     ftexture_p->width,
		     ftexture_p->height,
		     0,
		     GL_DEPTH_COMPONENT,
		     GL_FLOAT,
		     NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER,
			       GL_DEPTH_ATTACHMENT,
			       GL_TEXTURE_2D,
			       ftexture_p->depth_text_id,
			       0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
    }
    else
    {
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
    
    }
    
    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    /////////////////////////
    // Configure VBO & VAO //
    /////////////////////////
    
    // Bind VAO first
    glBindVertexArray(ftexture_p->quad_vao);

    // Move quad data to GPU
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

/////////////////////////
// Struct AssetTableID //
/////////////////////////

AssetTableID::AssetTableID()
{
    memset(table, -1, sizeof(int) * TOTAL_ENTITY_TYPES * TOTAL_ASSET_TYPES);
}

//////////////////////////
// Struct AssetTableDir //
//////////////////////////

AssetTableDir::AssetTableDir()
{
    // Chest Entity
    table[CHEST][TEXTURE_D] = "..\\data\\assets\\textures\\chest.bmp";
    table[CHEST][TEXTURE_N] = "..\\data\\assets\\textures\\brickwall_normal.bmp";
    table[CHEST][TEXTURE_S] = NULL;
    table[CHEST][MESH01]    = "..\\data\\assets\\meshes\\chest.obj";
    table[CHEST][SOUND01]   = "..\\data\\assets\\sfx\\taunt.wav";
    table[CHEST][SOUND02]   = NULL;
    table[CHEST][SOUND03]   = NULL;

    // Debug Grid Entity
    table[GRID][TEXTURE_D] = NULL;
    table[GRID][TEXTURE_N] = NULL;
    table[GRID][TEXTURE_S] = NULL;
    table[GRID][MESH01]    = "..\\data\\assets\\meshes\\grid.obj";
    table[GRID][SOUND01]   = NULL;
    table[GRID][SOUND02]   = NULL;
    table[GRID][SOUND03]   = NULL;

    // Player Entity
    table[PLAYER][TEXTURE_D] = "..\\data\\assets\\textures\\green.bmp";
    table[PLAYER][TEXTURE_N] = "..\\data\\assets\\textures\\brickwall_normal.bmp";
    table[PLAYER][TEXTURE_S] = NULL;
    table[PLAYER][MESH01]    = "..\\data\\assets\\meshes\\block.obj";
    table[PLAYER][SOUND01]   = NULL;
    table[PLAYER][SOUND02]   = NULL;
    table[PLAYER][SOUND03]   = NULL;
    
    // Block Entity
    table[BLOCK][TEXTURE_D] = "..\\data\\assets\\textures\\white.bmp";
    table[BLOCK][TEXTURE_N] = "..\\data\\assets\\textures\\brickwall_normal.bmp";
    table[BLOCK][TEXTURE_S] = NULL;
    table[BLOCK][MESH01]    = "..\\data\\assets\\meshes\\block.obj";
    table[BLOCK][SOUND01]   = NULL;
    table[BLOCK][SOUND02]   = NULL;
    table[BLOCK][SOUND03]   = NULL;
    
    // Special Block Entity
    table[SPECIAL_BLOCK][TEXTURE_D] = "..\\data\\assets\\textures\\chest.bmp";
    table[SPECIAL_BLOCK][TEXTURE_N] = "..\\data\\assets\\textures\\brickwall_normal.bmp";
    table[SPECIAL_BLOCK][TEXTURE_S] = NULL;
    table[SPECIAL_BLOCK][MESH01]    = "..\\data\\assets\\meshes\\block.obj";
    table[SPECIAL_BLOCK][SOUND01]   = NULL;
    table[SPECIAL_BLOCK][SOUND02]   = NULL;
    table[SPECIAL_BLOCK][SOUND03]   = NULL;

    // Small Dog Entity
    table[SMALL_DOG][TEXTURE_D] = "..\\data\\assets\\textures\\orange.bmp";
    table[SMALL_DOG][TEXTURE_N] = "..\\data\\assets\\textures\\brickwall_normal.bmp";
    table[SMALL_DOG][TEXTURE_S] = NULL;
    table[SMALL_DOG][MESH01]    = "..\\data\\assets\\meshes\\small_dog.obj";
    table[SMALL_DOG][SOUND01]   = NULL;
    table[SMALL_DOG][SOUND02]   = NULL;
    table[SMALL_DOG][SOUND03]   = NULL;

    // Medium Dog Entity
    table[MEDIUM_DOG][TEXTURE_D] = "..\\data\\assets\\textures\\black.bmp";
    table[MEDIUM_DOG][TEXTURE_N] = "..\\data\\assets\\textures\\brickwall_normal.bmp";
    table[MEDIUM_DOG][TEXTURE_S] = NULL;
    table[MEDIUM_DOG][MESH01]    = "..\\data\\assets\\meshes\\medium_dog.obj";
    table[MEDIUM_DOG][SOUND01]   = NULL;
    table[MEDIUM_DOG][SOUND02]   = NULL;
    table[MEDIUM_DOG][SOUND03]   = NULL;

    // Large Dog Entity
    table[LARGE_DOG][TEXTURE_D] = "..\\data\\assets\\textures\\yellow.bmp";
    table[LARGE_DOG][TEXTURE_N] = "..\\data\\assets\\textures\\brickwall_normal.bmp";
    table[LARGE_DOG][TEXTURE_S] = NULL;
    table[LARGE_DOG][MESH01]    = "..\\data\\assets\\meshes\\large_dog.obj";
    table[LARGE_DOG][SOUND01]   = NULL;
    table[LARGE_DOG][SOUND02]   = NULL;
    table[LARGE_DOG][SOUND03]   = NULL;
}

///////////////////////////
// Struct ActiveTextures //
///////////////////////////

ActiveTextures::ActiveTextures()
{
    registered_count = 0;

    // Initialize array to contain all NULL
    memset(textures, 0, sizeof(textures));
}

int activeTexturesRegister(ActiveTextures &active_textures, AssetManager &asset_manager,
			   int entity_type, int asset_type)
{
    // Returns 1 on success, 0 on failure.

    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);

    // Get path
    c_char* path = asset_manager.asset_table_dir.table[entity_type][asset_type];
    if(!path) {return 0;}
    
    // Check that there is room to register
    if(!(active_textures.registered_count < MAX_TEXTURES)) {return 0;}

    // Allocate new texture and add pointer to activeTextures registry
    active_textures.textures[active_textures.registered_count] = new Texture(path);
    if(!active_textures.textures[active_textures.registered_count]) {return 0;}

    // Add ID to asset table ID
    asset_manager.asset_table_ID.table[entity_type][asset_type] = active_textures.registered_count;

    // Update registered count
    active_textures.registered_count++;
    
    return 1;
}

void activeTexturesUnregisterAll(ActiveTextures &active_textures)
{
    // Delete all pointers
    for(uint i = 0; i < active_textures.registered_count; i++)
    {
	delete active_textures.textures[i];
    }
    // Reset count
    active_textures.registered_count = 0;
}

/////////////////////////
// Struct ActiveMeshes //
/////////////////////////

ActiveMeshes::ActiveMeshes()
{
    registered_count = 0;

    // Init array to contain all NULLs
    memset(meshes, 0, sizeof(meshes));
}

int activeMeshesRegister(ActiveMeshes &active_meshes, AssetManager &asset_manager,
                         int entity_type, int asset_type)
{
    // Returns 1 on success, 0 on failure.
    
    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);
    
    // Get path
    c_char* path = asset_manager.asset_table_dir.table[entity_type][asset_type];
    if(!path) {return 0;}
    
    // Check that there is room to register
    if(!(active_meshes.registered_count < MAX_MESHES)) {return 0;}
    
    // Allocate new mesh and add pointer to activeMeshes registry
    active_meshes.meshes[active_meshes.registered_count] = new Mesh(path);
    if(!active_meshes.meshes[active_meshes.registered_count]) {return 0;}

    // Add ID to asset table ID
    asset_manager.asset_table_ID.table[entity_type][asset_type] = active_meshes.registered_count;

    // Update registered count
    active_meshes.registered_count++;

    return 1;
}

void activeMeshesUnregisterAll(ActiveMeshes &active_meshes)
{
    // Delete all pointers
    for(uint i = 0; i < active_meshes.registered_count; i++)
    {
	delete active_meshes.meshes[i];
    }
    // Reset count
    active_meshes.registered_count = 0;
}

/////////////////////////
// Struct ActiveSounds //
/////////////////////////

ActiveSounds::ActiveSounds()
{
    registered_count = 0;

    // Init sound pointer array to NULL
    memset(sounds, 0, sizeof(sounds));
}

int activeSoundsRegister(ActiveSounds &active_sounds, AssetManager &asset_manager, int entity_type,
			 int asset_type, SoundInterface* sound_interface_p)
{
    // Returns 1 on success, 0 on failure.
    
    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);

    // Get path
    c_char* path = asset_manager.asset_table_dir.table[entity_type][asset_type];
    if(!path) {return 0;}
    
    // Check that there is room to register
    if(!(active_sounds.registered_count < MAX_SOUNDS)) {return 0;}

    // Allocate new sound and add pointer to activeSounds registry
    active_sounds.sounds[active_sounds.registered_count] = new Sound(path, *sound_interface_p);
    if(!active_sounds.sounds[active_sounds.registered_count]) {return 0;}
    
    // Add ID to asset table ID
    asset_manager.asset_table_ID.table[entity_type][asset_type] = active_sounds.registered_count;

    // Update registered count
    active_sounds.registered_count++;

    return 1;
}

void activeSoundsUnregisterAll(ActiveSounds &active_sounds)
{
    // Delete all pointers
    for(uint i = 0; i < active_sounds.registered_count; i++)
    {
	delete active_sounds.sounds[i];
    }
    // Reset count
    active_sounds.registered_count = 0;
}

/////////////////////////
// Struct AssetManager //
/////////////////////////

AssetManager::~AssetManager()
{
    assetManagerUnregisterAll(*this);
}

int assetManagerRegister(AssetManager &asset_manager, int entity_type, int asset_type, void *sound_interface_p)
{
    // Returns 1 on success, 0 on failure
    
    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);
    
    switch(asset_type)
    {
    case TEXTURE_D:
	return activeTexturesRegister(asset_manager.active_textures_d,
				      asset_manager, entity_type, asset_type);
    case TEXTURE_N:
	return activeTexturesRegister(asset_manager.active_textures_n,
				      asset_manager, entity_type, asset_type);
    case TEXTURE_S:
	return activeTexturesRegister(asset_manager.active_textures_s,
				      asset_manager, entity_type, asset_type);
    case MESH01:
	return activeMeshesRegister(asset_manager.active_meshes,
				    asset_manager, entity_type, asset_type);
    case SOUND01:
	return activeSoundsRegister(asset_manager.active_sounds_01, asset_manager,
	                            entity_type, asset_type, (SoundInterface*)sound_interface_p);
    case SOUND02:
	return activeSoundsRegister(asset_manager.active_sounds_02, asset_manager,
	                            entity_type, asset_type, (SoundInterface*)sound_interface_p);
    case SOUND03:
	return activeSoundsRegister(asset_manager.active_sounds_03, asset_manager,
	                            entity_type, asset_type, (SoundInterface*)sound_interface_p);
    default:
	return 0;
    }
}

void assetManagerUnregisterAll(AssetManager &asset_manager)
{
    
    // Unregister Diffuse textures
    activeTexturesUnregisterAll(asset_manager.active_textures_d);

    // Unregister Normal textures
    activeTexturesUnregisterAll(asset_manager.active_textures_n);
    
    // Unregister Specular textures
    activeTexturesUnregisterAll(asset_manager.active_textures_s);
    
    // Delete all mesh pointers
    activeMeshesUnregisterAll(asset_manager.active_meshes);
    
    // Delete all sound01 pointers
    activeSoundsUnregisterAll(asset_manager.active_sounds_01);
    
    // Delete all sound02 pointers
    activeSoundsUnregisterAll(asset_manager.active_sounds_02);
    
    // Delete all sound03 pointers
    activeSoundsUnregisterAll(asset_manager.active_sounds_03);

    // Delete all shaders
    activeShadersUnregisterAll(asset_manager.active_shaders);
}

void* assetManagerGetAssetP(AssetManager &asset_manager, int entity_type,
			    int asset_type, void *sound_interface_p)
{
    // Returns void asset pointer on success, NULL on failure.

    // Assert that object and asset types are within acceptable range
    _assert(entity_type >= 0 && entity_type <= TOTAL_ENTITY_TYPES);
    _assert(asset_type >= 0 && asset_type <= TOTAL_ASSET_TYPES);
    
    // init return value
    void* return_p = NULL;
    
    int index = asset_manager.asset_table_ID.table[entity_type][asset_type];
    if(index < 0)
    {
	if(!assetManagerRegister(asset_manager, entity_type, asset_type, sound_interface_p))
	{
	    return return_p;
	}
	index = asset_manager.asset_table_ID.table[entity_type][asset_type];
    }
    
    switch(asset_type)
    {
    case TEXTURE_D:
	return_p = (void*)asset_manager.active_textures_d.textures[index];
	break;
    case TEXTURE_N:
        return_p = (void*)asset_manager.active_textures_n.textures[index];
	break;
    case TEXTURE_S:
	return_p = (void*)asset_manager.active_textures_s.textures[index];
	break;
    case MESH01:
	return_p = (void*)asset_manager.active_meshes.meshes[index];
	break;
    case SOUND01:
	return_p = (void*)asset_manager.active_sounds_01.sounds[index];
	break;
    case SOUND02:
	return_p = (void*)asset_manager.active_sounds_02.sounds[index];
	break;
    case SOUND03:
	return_p = (void*)asset_manager.active_sounds_03.sounds[index];
	break;
    }
    return return_p;
}

void* assetManagerGetShaderP(AssetManager &asset_manager, int program_type)
{
    // Returns void shader pointer on success, NULL on failure

    // Assert that program type is within acceptable range
    _assert(program_type >= 0 && program_type <= TOTAL_SHADER_PROGRAMS);

    int index = asset_manager.shader_table_ID.table[program_type];
    if(index < 0)
    {
	if(!activeShadersRegister(asset_manager.active_shaders, asset_manager, program_type))
	{
	    return NULL;
	}
	index = asset_manager.shader_table_ID.table[program_type];
    }

    return (void*)asset_manager.active_shaders.shaders[index];
}

//////////////////////////
// Struct ShaderTableID //
//////////////////////////

ShaderTableID::ShaderTableID()
{
    memset(table, -1, sizeof(int) * TOTAL_SHADER_PROGRAMS);
}

///////////////////////////
// Struct ShaderTableDir //
///////////////////////////

ShaderTableDir::ShaderTableDir()
{
    // Shadow Map Shader Program
    table[SHADOWMAP][VERTEX]   = "..\\data\\assets\\shaders\\shadowmap.vert";
    table[SHADOWMAP][FRAGMENT] = "..\\data\\assets\\shaders\\shadowmap.frag";

    // Shadow Map Debug Shader Program
    table[SMDEBUG][VERTEX]   = "..\\data\\assets\\shaders\\shadowmap_db.vert";
    table[SMDEBUG][FRAGMENT] = "..\\data\\assets\\shaders\\shadowmap_db.frag";
    
    // Blinn-Phong Shader Program
    table[BLINNPHONG][VERTEX]   = "..\\data\\assets\\shaders\\blinn_phong.vert";
    table[BLINNPHONG][FRAGMENT] = "..\\data\\assets\\shaders\\blinn_phong.frag";

    // Post-Processing Shader Program
    table[POSTPROCESS][VERTEX]   = "..\\data\\assets\\shaders\\pp.vert";
    table[POSTPROCESS][FRAGMENT] = "..\\data\\assets\\shaders\\pp.frag";

    // Grid Shader Program
    table[GRID][VERTEX]   = "..\\data\\assets\\shaders\\db_grid.vert";
    table[GRID][FRAGMENT] = "..\\data\\assets\\shaders\\db_grid.frag";
}

//////////////////////////
// Struct ActiveShaders //
//////////////////////////

ActiveShaders::ActiveShaders()
{
    registered_count = 0;
    
    // Initialize array to contain all NULL
    memset(shaders, 0, sizeof(shaders));
}

int activeShadersRegister(ActiveShaders &active_shaders, AssetManager &asset_manager,
			   int program_type)
{
    // Returns 1 on success, 0 on failure

    // Assert that program type is within acceptable range
    _assert(program_type >= 0 && program_type <= TOTAL_SHADER_PROGRAMS);

    // Get paths
    c_char* vertex_path = asset_manager.shader_table_dir.table[program_type][VERTEX];
    if(!vertex_path) {return 0;}
    c_char* fragment_path = asset_manager.shader_table_dir.table[program_type][FRAGMENT];
    if(!fragment_path) {return 0;}
    
    // Check that there is room to register
    if(!(active_shaders.registered_count < MAX_SHADERS)) {return 0;}

    // Allocate new shader and add pointer to the activeShaders registry
    active_shaders.shaders[active_shaders.registered_count] = new Shader(vertex_path, fragment_path);
    if(!active_shaders.shaders[active_shaders.registered_count]) {return 0;}

    // Add ID to shader table ID
    asset_manager.shader_table_ID.table[program_type] = active_shaders.registered_count;

    // Update registered count
    active_shaders.registered_count++;

    return 1;
}

void activeShadersUnregisterAll(ActiveShaders &active_shaders)
{
    // Delete all shader structs
    for(uint i = 0; i < active_shaders.registered_count; i++)
    {
	delete active_shaders.shaders[i];
    }

    // Reset registered count
    active_shaders.registered_count = 0;
}
