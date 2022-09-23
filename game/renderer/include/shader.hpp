// ======================================================================
// Title: shader.hpp
// Description: The header file for shader & shader program objects
// Last Modified: 9/17/2022 MDC
// ======================================================================

#ifndef SHADER_H
#define SHADER_H

typedef struct Shader {
    GLuint program_id;
    GLuint vert_id;
    GLuint frag_id;
    Shader(const char* _vertPath, const char* _fragPath);
    ~Shader();
    void addMat4Uniform(const char* name, const float* m);
} Shader;

#endif 
