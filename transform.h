#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "types.h"

mat4 rotate_x(float rad);
mat4 rotate_y(float rad);
mat4 rotate_z(float rad);
mat4 translate(float x, float y, float z);
mat4 translate(const vec3 &v);
mat4 scale(float x, float y, float z);
mat4 scale(float s);

#endif