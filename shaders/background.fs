#version 140

in vec2 v_position;

uniform float blend_factor;

out vec4 out_color;

void main()
{
	vec3 shade1 = vec3(45 / 255.0, 179 / 255.0, 204 / 255.0);
	vec3 shade2 = vec3(1.0, 0.75, 0.66);
	vec3 color = mix(shade1, shade2, clamp(blend_factor, 0.0, 1.0));
	out_color = vec4(color, 1.0);
}