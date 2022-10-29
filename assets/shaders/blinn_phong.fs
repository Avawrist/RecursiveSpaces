#version 330 core

in vec2 uv;
in vec3 norm;

out vec4 color;

Struct DirLight
{
	vec3 color;
	vec3 dir;
	float ambient_strength;
};
uniform dl_count;
uniform DirLight dirLights[dl_count];

uniform sampler2D diffuse_map;

void main()
{
	
}