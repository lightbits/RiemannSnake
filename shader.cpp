#include "shader.h"
#include <iostream>

Shader *Shader::active_shader = nullptr;
Shader *get_active_shader() { return Shader::active_shader; }

Shader::Shader() : program(0) { }

bool Shader::load(const string &vs_path, const string &fs_path)
{
	if(!load_program(program, vs_path, fs_path))
	{
		std::cerr << "Error loading shader: " << vs_path << ", " << fs_path << std::endl;
		return false;
	}
	return true;
}

void Shader::dispose()
{
	attributes.clear();
	uniforms.clear();
	if (active_shader == this)
		active_shader = nullptr;
	glDeleteProgram(program);
}

void Shader::use()
{
	active_shader = this;
	glUseProgram(program);
}

void Shader::unuse()
{
	active_shader = nullptr;
	glUseProgram(0);
}

GLint Shader::get_uniform_location(const string &name)
{
	std::unordered_map<std::string, GLint>::iterator it = uniforms.find(name);
	if(it != uniforms.end())
	{
		return it->second;
	}
	else
	{
		GLint location = glGetUniformLocation(program, name.c_str());
		if(location < 0)
			throw std::runtime_error("Invalid shader uniform [" + name + "] (" + "not used or bad name)");
		uniforms[name] = location;
		return location;
	}
}

GLint Shader::get_attribute_location(const string &name)
{
	std::unordered_map<std::string, GLint>::iterator it = attributes.find(name);
	if(it != attributes.end())
	{
		return it->second;
	}
	else
	{
		GLint location = glGetAttribLocation(program, name.c_str());
		if(location < 0)
			throw std::runtime_error("Invalid shader attribute [" + name + "] (" + "not used or bad name)");
		attributes[name] = location;
		return location;
	}
}

void Shader::set_attribfv(const string &name, GLsizei num_components, 
						  GLsizei stride, GLsizei offset)
{
	GLint loc = get_attribute_location(name);
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(
		loc, 
		num_components, // in the attribute,
		GL_FLOAT, // each component is a float,
		GL_FALSE, // not normalized,
		stride * sizeof(GLfloat), // the attribs are seperated by this much,
		(void*)(offset * sizeof(GLfloat)) // beginning at this offset in the array.
		);
}

void Shader::unset_attrib(const string &name)
{
	glDisableVertexAttribArray(get_attribute_location(name));
}

void Shader::set_uniform(const string &name, const mat4 &mat) { glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)); }
void Shader::set_uniform(const string &name, const mat3 &mat) { glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)); }
void Shader::set_uniform(const string &name, const mat2 &mat) { glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)); }
void Shader::set_uniform(const string &name, const vec4 &vec) { glUniform4f(get_uniform_location(name), vec.x, vec.y, vec.z, vec.w); }
void Shader::set_uniform(const string &name, const vec3 &vec) { glUniform3f(get_uniform_location(name), vec.x, vec.y, vec.z); }
void Shader::set_uniform(const string &name, const vec2 &vec) { glUniform2f(get_uniform_location(name), vec.x, vec.y); }
void Shader::set_uniform(const string &name, GLdouble d) { glUniform1f(get_uniform_location(name), float(d)); }
void Shader::set_uniform(const string &name, GLfloat f) { glUniform1f(get_uniform_location(name), f); }
void Shader::set_uniform(const string &name, GLint i) { glUniform1i(get_uniform_location(name), i); }