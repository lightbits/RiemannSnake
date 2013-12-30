#version 140

in vec2 position;
in vec4 color;
in vec2 texel;
in float letter;

out vec4 vert_color;
out vec2 vert_texel;

uniform mat4 projection;
uniform mat4 view;
uniform float time;

void main()
{
	vert_color = color;
	vert_texel = texel;
	float offset = sin(letter * 0.5 + time * 2.0);
	offset = round(offset);
	gl_Position = projection * view * vec4(position.x, position.y + offset, 0.0, 1.0);
}