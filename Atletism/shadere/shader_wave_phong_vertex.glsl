#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_normal;		

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform int time;
uniform float amplitude;
uniform float flag_width;

out vec3 world_pos;
out vec3 world_normal;

#define PI 3.1415

void main(){

	float frequency = 1;
	float intensity = sqrt((in_position.x / flag_width));
	float phase = (in_position.x * 2 * PI) / flag_width;
	float sine = sin(time * frequency + phase) * intensity * amplitude;

	vec3 new_in_position = vec3(in_position.x - sine / 2 , in_position.y + sine, in_position.z - sine / 2);

	world_pos = (model_matrix * vec4(new_in_position,1)).xyz;
	world_normal = normalize(mat3(model_matrix) *  in_normal );

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(new_in_position,1); 
}
