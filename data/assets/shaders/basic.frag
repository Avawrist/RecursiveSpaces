#version 330 core

in vec2 uv;
in vec3 norm;

out vec4 color;

uniform sampler2D diffuse_map;

void main()
{
	color = texture(diffuse_map, uv);
}