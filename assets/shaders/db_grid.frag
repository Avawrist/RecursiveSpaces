#version 330 core

// Out Variables
out vec4 final_color;

// Uniforms
uniform vec3 color;

void main()
{
	final_color = vec4(color, 1.0);
}