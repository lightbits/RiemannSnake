#version 140

in vec3 position;
in vec3 barycentric;
in vec4 color;

out vec4 vert_color;
out vec3 v_bc;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{	
	vert_color = color;
	v_bc = barycentric; // interpolate barycentric coordinates
	gl_Position = projection * view * model * vec4(position, 1.0f);
}