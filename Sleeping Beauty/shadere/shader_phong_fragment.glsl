#version 330
layout(location = 0) out vec4 out_color;

uniform sampler2D textura;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform vec3 material_kd;
uniform vec3 material_ks;

in vec3 world_pos;
in vec3 world_normal;
in vec2 texcoord;

vec3 L;
vec3 V;
vec3 H;

// Algorithmul pentru iluminare Phong (ca la laborator)
void main()
{	
	vec3 tex = texture(textura, texcoord).xyz;
	vec3 kd = tex;
	vec3 ks = tex;
	
	L = normalize (light_position - world_pos); 
	V = normalize  (eye_position - world_pos);
	H = normalize(L + V);

	vec3 ambient = vec3(0.1, 0.1, 0.1);
	int get_color = (dot(world_normal, L)> 0) ? 1 : 0;
	vec3 dif = kd * max(dot(world_normal, L), 0);
	vec3 spec = ks * get_color * pow((max(dot(world_normal, H), 0)), material_shininess);

	vec3 color = ambient + dif + spec;

	out_color = vec4(color.x, color.y, color.z, 1);
} 