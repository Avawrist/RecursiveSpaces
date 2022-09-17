// ======================================================================
// Title: shader.h
// Description: The header file for shader & shader program objects
// Last Modified: 9/17/2022 MDC
// ======================================================================

#ifndef SHADER_H
#define SHADER_H

typedef struct Shader {
    GLuint id;
    GLenum type;
    char*  pFileSource;
    Shader(GLenum _type);
} Shader;

#endif 
