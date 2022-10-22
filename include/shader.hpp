// ======================================================================
// Title: shader.hpp
// Description: The header file for shader program objects
// ======================================================================

#ifndef SHADER_H
#define SHADER_H

#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct Shader {
    GLuint program_id;
    GLuint vert_id;
    GLuint frag_id;
    Shader(const char* _vertPath, const char* _fragPath);
    ~Shader();
    void addMat4Uniform(const char* name, const float* m);
} Shader;

#endif 
