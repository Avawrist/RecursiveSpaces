// ======================================================================
// Title: shader.cpp
// Description: The primary source file for shader & shader program objects.
// Last Modified: 9/17/2022 MDC
// ======================================================================

#include <shader.hpp>

// # Struct Shader
// A basic struct that holds the vertex, fragment and shader program ids.
// On creation, the struct will create the required vertex and fragment shaders,
// set the shader source code, compile and link the shader program.

Shader::Shader(const char* _vertPath, const char* _fragPath)
{
    // Create Shader Program
    program_id = glCreateProgram();

    //////////////////////////
    // Create Vertex Shader //
    //////////////////////////

    // Init values & create shader
    vert_id = glCreateShader(GL_VERTEX_SHADER);
    
    // Convert file into NULL terminated const char* from path
    const char*       pShaderCodeV;
    std::string       sShaderCodeV;
    std::ifstream     shaderFileV;
    std::stringstream shaderStreamV;
    try
    {
	shaderFileV.open(_vertPath, std::ifstream::in);
	shaderStreamV << shaderFileV.rdbuf();
	shaderFileV.close();
	sShaderCodeV = shaderStreamV.str();
	pShaderCodeV = sShaderCodeV.c_str();
    }
    catch(std::ifstream::failure e)
    {
	OutputDebugStringA("ERROR: VERTEX SHADER FILE NOT READ\n");
    }
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
    const char*       pShaderCodeF;
    std::string       sShaderCodeF;
    std::ifstream     shaderFileF;
    std::stringstream shaderStreamF;
    try
    {
	shaderFileF.open(_fragPath, std::ifstream::in);
	shaderStreamF << shaderFileF.rdbuf();
	shaderFileF.close();
	sShaderCodeF = shaderStreamF.str();
	pShaderCodeF = sShaderCodeF.c_str();
    }
    catch(std::ifstream::failure e)
    {
	OutputDebugStringA("ERROR: FRAGMENT SHADER FILE NOT READ\n");
    }
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

void shaderAddMat4Uniform(const Shader* shader_p, const char* name, const float* m)
{
    int loc = glGetUniformLocation(shader_p->program_id, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, m);
}
