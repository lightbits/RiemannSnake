#version 140

in vec4 vert_color;
in vec3 v_bc;

out vec4 out_color;

// http://codeflow.org/entries/2012/aug/02/easy-wireframe-display-with-barycentric-coordinates/
// http://stackoverflow.com/questions/6800544/what-is-the-fwidth-glsl-function-actually-used-for
// http://http.developer.nvidia.com/Cg/fwidth.html
// Returns a smoothed measure of how close we are to an edge
float edgeFactor()
{
	vec3 d = fwidth(v_bc);
	float thickness = 1.5;
	vec3 a3 = smoothstep(vec3(0.0), d * thickness, v_bc);
	return min(min(a3.x, a3.y), a3.z);
}

void main()
{
	out_color.rgb = mix(vec3(1.0, 0.43, 0.35), vert_color.rgb, edgeFactor());
	// out_color = vec4(0.0, 0.0, 0.0, (1.0 - edgeFactor()) * 0.95);
}