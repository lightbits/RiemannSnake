#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "types.h"

mat4 rotate_x(float rad)
{
	float co = cosf(rad); float si = sinf(rad);
	mat4 m(1.0f);
	m[1][1] = co; m[1][2] = -si; m[2][1] = si; m[2][2] = co;
	return m;
}

mat4 rotate_y(float rad)
{
	float co = cosf(rad); float si = sinf(rad);
	mat4 m(1.0f);
	m[0][0] = co; m[0][2] = si; m[2][0] = -si; m[2][2] = co;
	return m;
}

mat4 rotate_z(float rad)
{
	float co = cosf(rad); float si = sinf(rad);
	mat4 m(1.0f);
	m[0][0] = co; m[1][0] = -si; m[0][1] = si; m[1][1] = co;
	return m;
}

mat4 translate(float x, float y, float z)
{
	mat4 m(1.0f);
	m[3][0] = x; m[3][1] = y; m[3][2] = z; m[3][3] = 1.0f;
	return m;
}

mat4 translate(const vec3 &v)
{
	mat4 m(1.0f);
	m[3] = vec4(v, 1.0f); // the fourth column
	return m;
}

mat4 scale(float x, float y, float z)
{
	mat4 m(1.0f);
	m[0][0] = x; m[1][1] = y; m[2][2] = z;
	return m;
}

mat4 scale(float s)
{
	return scale(s, s, s);
}

#endif