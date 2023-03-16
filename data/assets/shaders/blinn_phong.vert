#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_norm;
layout (location = 3) in vec3 in_tang;

out vec2 uv;
out vec3 frag_pos;
out vec3 vert_norm;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	////////////////////////
	// Prep Outgoing Data //
	////////////////////////

	// TBN Matrix
	vec3 t = normalize(vec3(model * vec4(in_tang, 0.0))); // tangent vec to world space
	vec3 n = normalize(vec3(model * vec4(in_norm, 0.0))); // normal vec to world space
	vec3 b = normalize(cross(t, n)); // calc bitangent vec from tangent and normal
	TBN = mat3(t, b, n); // TBN matrix to convert vectors from tangent to world space

	// Positions and Coords
	uv          = in_uv;
	frag_pos    = vec3(model * vec4(in_pos, 1.0));
	vert_norm   = in_norm;
	gl_Position = projection * view * model * vec4(in_pos, 1.0);
}