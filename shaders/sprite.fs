#version 140

in vec4 vert_color;
in vec2 vert_texel;

uniform sampler2D tex;

out vec4 out_color;

void main()
{
	out_color = texture(tex, vert_texel) * vert_color;
}