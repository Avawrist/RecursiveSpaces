#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_norm;

out vec2 uv;
out vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(in_pos, 1.0);
	uv = in_uv;
	norm = in_norm;
}