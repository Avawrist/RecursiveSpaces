// ======================================================================
// Title: shader.hpp
// Description: The header file for shader program objects
// ======================================================================

#ifndef SHADER_H
#define SHADER_H

// Win libs
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>

// 3rd Party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// My libs
#include "typedefs.hpp"
#include "mdcla.hpp"

typedef struct Shader {
    GLuint program_id;
    GLuint vert_id;
    GLuint frag_id;
    Shader(c_char* _vertPath, c_char* _fragPath);
    ~Shader();
} Shader;
void shaderAddMat4Uniform(const Shader* shader_p, c_char* name, c_float* m);
void shaderAddVec3Uniform(const Shader* shader_p, c_char* name, float x, float y, float z);
void shaderAddFloatUniform(const Shader* shader_p, c_char* name, float f);

#endif 
