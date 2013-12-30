#version 140

in vec2 v_position;

uniform float time;
uniform float scroll_speed;

out vec4 out_color;

void main()
{
	vec3 shade1 = vec3(0.79, 0.84, 0.53);
	vec3 shade2 = vec3(1.0, 0.75, 0.66);
	vec3 shade3 = vec3(1.0, 0.52, 0.60);
	float alpha = mod(v_position.y * 0.5 + 0.5 + time * 0.8, 1.0);

	vec3 color = shade1;
	if (alpha < 0.33)
		color = shade1;
	else if (alpha < 0.66)
		color = shade2;
	else if (alpha < 1.0)
		color = shade3;

	color += color * 0.4;
	out_color = vec4(color, 1.0);
}