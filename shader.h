#ifndef SHADER_H
#define SHADER_H
#include "util.h"
#include <unordered_map>

class Shader
{
public:
	Shader();

	void load(const string &vs_path, const string &fs_path);
	void dispose();
	void use();
	void unuse();

	GLint Shader::get_uniform_location(const string &name);
	GLint Shader::get_attribute_location(const string &name);

	void set_attribfv(const string &name, GLsizei num_components, GLsizei stride, GLsizei offset);
	void unset_attrib(const string &name);

	void set_uniform(const string &name, const mat4 &mat);
	void set_uniform(const string &name, const mat3 &mat);
	void set_uniform(const string &name, const mat2 &mat);
	void set_uniform(const string &name, const vec4 &vec);
	void set_uniform(const string &name, const vec3 &vec);
	void set_uniform(const string &name, const vec2 &vec);
	void set_uniform(const string &name, GLdouble d);
	void set_uniform(const string &name, GLfloat f);
	void set_uniform(const string &name, GLint i);
private:
	std::unordered_map<string, GLint> attributes;
	std::unordered_map<string, GLint> uniforms;
	GLuint program;
};

#endif