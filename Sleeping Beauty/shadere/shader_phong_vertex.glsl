#version 330

layout(location = 0) in vec3 in_position_a;		
layout(location = 1) in vec3 in_normal_a;
layout(location = 2) in vec2 in_texcoord_a;

layout(location = 3) in vec3 in_position_b;		
layout(location = 4) in vec3 in_normal_b;
layout(location = 5) in vec2 in_texcoord_b;  // asta e inutil ca oricum e acelasi, dar simplifca lucrurile

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform float blend_factor;

out vec3 world_pos;
out vec3 world_normal;
out vec2 texcoord;

// Formula pentru interpolare
vec3 linear_interpolation(vec3 from, vec3 to, float blend_factor)
{
	return (1-blend_factor) * from + blend_factor * to;
}

void main()
{
	vec3 blendedPosition = linear_interpolation(in_position_a, in_position_b, blend_factor);
	vec3 blendedNormal = linear_interpolation(in_normal_a, in_normal_b, blend_factor);
	blendedNormal = normalize(blendedNormal); // normalele trebuie normalizate, ca interpolarea a 2 normale nu are lungime 1

	world_pos = (model_matrix * vec4(blendedPosition,1)).xyz;
	world_normal = normalize(mat3(model_matrix) * blendedNormal);
	texcoord = in_texcoord_a;

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(blendedPosition, 1); 
}
