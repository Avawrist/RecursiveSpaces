#version 330 core

// In Variables
in vec2 uv;
in vec3 frag_pos;
in vec3 vert_norm;
in mat3 TBN;
in vec4 frag_pos_light_space;

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
uniform sampler2D spec_map;
uniform sampler2D shadow_map;
uniform vec3      cam_pos;

// Function Prototypes
vec3 getDirLight(DirLight dirLight,
		 vec2 uv,
		 vec3 light_dir,
		 vec3 view_dir,
		 vec3 norm,
		 vec4 frag_pos_light_space);
		 
float calculateShadow(vec4 frag_pos_light_space,
		      vec3 norm, vec3 light_dir);

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
	object_color *= getDirLight(dirLight, uv, n_light_dir, n_view_dir, frag_norm, frag_pos_light_space);

	// Add PointLight

	// Add SpotLight

	////////////
	// Output //
	////////////
	final_color = vec4(object_color, 1.0);
}

vec3 getDirLight(DirLight dirLight,
     		 vec2 uv,
		 vec3 light_dir,
		 vec3 view_dir,
		 vec3 norm,
		 vec4 frag_pos_light_space)
{
	// Blinn-Phong method. Takes normalized vectors as input.
		
	// Prep Data
	vec3 n_half = normalize(view_dir + light_dir);

	// Ambient Component
	vec3 ambient_comp = dirLight.ambient_strength * dirLight.color;

	// Diffuse Component
	vec3 diffuse_comp = max(dot(light_dir, norm), 0.0) * dirLight.color;

	// Specular Component
	vec3 specular_comp = pow(max(dot(norm, n_half), 0.0), 128.0) * dirLight.color * vec3(texture(spec_map,uv));

	// Shadow Component
	float shadow = calculateShadow(frag_pos_light_space, norm, light_dir);
	
	return (ambient_comp + (shadow) * (diffuse_comp + specular_comp));
}

float calculateShadow(vec4 frag_pos_light_space, vec3 norm, vec3 light_dir)
{
	// Perform perspective divide
	vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
	// Transform to [0,1] range
	proj_coords = (proj_coords * 0.5) + 0.5;
	// Get depth of current fragment
	float current_depth = proj_coords.z;
	// Check whether current frag pos is in shadow using PCF filtering for higher resolution
	//float bias = max(0.05 * (1.0 - dot(norm, light_dir)), 0.005);
	float shadow = 0.0;
	vec2 texel_size = 1.0 / textureSize(shadow_map, 0);

	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			float neighbor_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
			shadow += current_depth > neighbor_depth ? 0.0 : 1.0;
		}
	}
	shadow /= 9.0;

	return shadow;
}