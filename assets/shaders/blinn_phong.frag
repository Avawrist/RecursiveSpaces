#version 330 core

// In Variables
in vec2 uv;
in vec3 frag_pos;
in mat3 TBN;

// Out Variables
out vec4 final_color;

// Structs
struct DirLight
{
	vec3  color;
	vec3  dir;
	float ambient_strength;
};

// Uniforms
uniform DirLight  dirLight;
uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform vec3      cam_pos;

// Function Prototypes
vec3 getDirLight(DirLight dirLight, vec3 light_dir, vec3 view_dir, vec3 norm);
float reduceGradient(float n_value_f);

// Function Definitions
void main()
{
	///////////////
	// Prep Data //
	///////////////

	// Directions
	vec3 n_light_dir = normalize(-dirLight.dir);      // world space
	vec3 n_view_dir  = normalize(cam_pos - frag_pos); // world space

	// Fragment Normal (In range -1 to 1)
	vec3 frag_norm = (vec3(texture(normal_map, uv)) * 2.0) - 1.0;
	frag_norm      = normalize(TBN * frag_norm); // Convert from tangent space to world space

	// Object Color
	vec3 object_color = vec3(texture(diffuse_map, uv));

	////////////////
	// Add Lights //
	////////////////

	// Add DirLight
	object_color *= getDirLight(dirLight, n_light_dir, n_view_dir, frag_norm);

	// Add PointLight

	// Add SpotLight

	////////////
	// Output //
	////////////
	final_color = vec4(object_color, 1.0);
}

vec3 getDirLight(DirLight dirLight, vec3 light_dir, vec3 view_dir, vec3 norm)
{
	// Blinn-Phong method. Takes normalized vectors as input.
		
	// Prep Data
	vec3 n_half = normalize(view_dir + light_dir);

	// Ambient Component
	vec3 ambient_comp = dirLight.ambient_strength * dirLight.color;

	// Diffuse Component
	vec3 diffuse_comp = max(dot(light_dir, norm), 0.0) * dirLight.color;

	// Specular Component
	// TODO: Get shininess from material (currently 128)
	vec3 specular_comp = pow(reduceGradient(max(dot(norm, n_half), 0.0)), 128) * dirLight.color;
	
	return (ambient_comp + diffuse_comp + specular_comp);
}

float reduceGradient(float n_value_f)
{
	// Takes a float value between 0 and 1 as input
	n_value_f *= 256;              // range is now 0 - 256 (float)
	n_value_f = floor(n_value_f);  // range is now 0 - 256 (rounded float)
	return n_value_f * 0.00391;    // returned range is now 0 - 1 (float)
}