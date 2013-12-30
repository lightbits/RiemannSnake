#version 140

in vec2 position;
in vec4 color;
in vec2 texel;

out vec4 vert_color;
out vec2 vert_texel;

uniform mat4 projection;
uniform mat4 view;
uniform float time;

void main()
{
	vert_color = color;
	vert_texel = texel;
	float offset = 2.0 * sin(time);
	offset = floor(offset);
	gl_Position = projection * view * vec4(position.x, position.y + offset, 0.0, 1.0);
}