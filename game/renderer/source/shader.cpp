// ======================================================================
// Title: shader.cpp
// Description: The primary source file for shader & shader program objects.
// Last Modified: 9/6/2022 MDC
// ======================================================================

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>

Shader::Shader(GLenum _type) {
    type = _type;
    id   = glCreateShader(type);
};
