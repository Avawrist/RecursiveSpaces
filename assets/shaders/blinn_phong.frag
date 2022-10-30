#version 330 core

// In Variables
in vec2 uv;
in vec3 norm;

// Out Variables
out vec4 final_color;

// Structs
struct DirLight
{
	vec3 color;
	vec3 dir;
	float ambient_strength;
};

// Uniforms
uniform DirLight  dirLight;
uniform sampler2D diffuse_map;

// Function Prototypes
vec3 getDirLight(DirLight dirLight, vec3 dir, vec3 norm);

// Function Definitions
void main()
{
	// Prep Data
	vec3 n_dir  = normalize(dirLight.dir);
	vec3 n_norm = normalize(norm);

	// Object Color
	vec3 object_color = vec3(texture(diffuse_map, uv));

	// Add DirLight
	object_color *= getDirLight(dirLight, n_dir, n_norm);

	// Add PointLight

	// Add SpotLight

	// Output
	final_color = vec4(object_color, 1.0);
}

vec3 getDirLight(DirLight dirLight, vec3 dir, vec3 norm)
{
	// Ambient Component
	vec3 ambient_comp = dirLight.ambient_strength * dirLight.color;

	// Diffuse Component
	vec3 diffuse_comp = max(dot(-dirLight.dir, norm), 0.0) * dirLight.color;

	// Specular Component
	// TODO

	return (ambient_comp + diffuse_comp);
}