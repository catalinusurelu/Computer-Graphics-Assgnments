#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_normal;		
layout(location = 2) in vec2 in_texcoord;		

uniform mat4 model_matrix, view_matrix, projection_matrix;

uniform int time;
uniform vec3 velocity;
uniform float gravity;

uniform vec3 center;
uniform float width;
uniform float height;

out vec2 texcoord;

// Aplicam ecuatia proiectilului - echivalent cu aplicarea unui model matrix
vec3 displace_center(vec3 center, float time, vec3 velocity, float gravity)
{
	vec3 new_center;
	new_center.x = center.x + velocity.x * time;
	new_center.z = center.z + velocity.z * time;
	new_center.y = center.y + velocity.y * time - 0.5 * gravity * time * time;

	return new_center;
}

void main()
{
	vec3 new_center = displace_center(center, time, velocity, gravity);

	// facem ca in enunt, trecem in view space ca sa putem caclula noua coordonata
	// pentru vertex-ul curent al billbordului
	new_center = (view_matrix*model_matrix*vec4(new_center, 1)).xyz;

	vec3 new_in_position;

	// vedem cum se situeaza coordonata initiala fata de centru
	// (stanga-sus, dreapta-sus, dreapta-jos etc.) si 
	// dupa ce calculam noul centru, setam coordonata noua
	// corespunzator (adica tot cum era inainte)

	// stanga sus
	if(0 > in_position.x && 0 < in_position.y)
	{
		new_in_position = new_center + vec3(- width / 2, + height / 2, 0);
	}
	// dreapata sus
	else if(0 < in_position.x && 0 < in_position.y)
	{
		new_in_position = new_center + vec3(+ width / 2, + height / 2, 0);
	}
	// stanga jos
	else if(0 > in_position.x && 0 > in_position.y)
	{
		new_in_position = new_center + vec3(- width / 2, - height / 2, 0);
	}
	// dreapta jos
	else if(0 < in_position.x && 0 > in_position.y)
	{
		new_in_position = new_center + vec3(+ width / 2, - height / 2, 0);
	}
	
	texcoord = in_texcoord;
	gl_Position = projection_matrix*vec4(new_in_position,1); 
}
