#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D color_texture;

void main()
{
	color = texture(color_texture, uv);
}