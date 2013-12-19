#include "util.h"
#include <fstream>
#include <iostream>

bool read_file(string path, string &dest)
{
	std::ifstream in(path, std::ios::in | std::ios::binary);
	if(!in.is_open())
	{
		std::cerr << "Error reading file: " << path << std::endl;
		return false;
	}

	if(in.good())
	{
		in.seekg(0, std::ios::end);			// Set get position to end
		dest.resize(in.tellg());			// Resize string to support enough bytes
		in.seekg(0, std::ios::beg);			// Set get position to beginning
		in.read(&dest[0], dest.size());		// Read file to string
		in.close();
	}

	return true;
}

GLuint gen_buffer(GLenum target, GLsizei size, const void *data)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, size, data, GL_STATIC_DRAW);
	glBindBuffer(target, 0);
	return buffer;
}

bool make_shader(GLuint &shader, GLenum type, string source)
{
	shader = glCreateShader(type);
	const char *src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        GLchar *info = new GLchar[length];
        glGetShaderInfoLog(shader, length, NULL, info);
        fprintf(stderr, "Error compiling shader:\n%s\n", info);
        delete[] info;
		return false;
    }
    return true;
}

bool load_shader(GLuint &shader, GLenum type, string path)
{
	string source;
	if(!read_file(path, source))
		return false;
	if(!make_shader(shader, type, source))
		return false;
	return true;
}

bool make_program(GLuint &program, GLuint vs_shader, GLuint fs_shader)
{
	program = glCreateProgram();
    glAttachShader(program, vs_shader);
    glAttachShader(program, fs_shader);
    glLinkProgram(program);

    glDetachShader(program, vs_shader);
    glDetachShader(program, fs_shader);
    glDeleteShader(vs_shader);
    glDeleteShader(fs_shader);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        GLchar *info = new GLchar[length];
        glGetProgramInfoLog(program, length, NULL, info);
        fprintf(stderr, "Error linking program: %s\n", info);
        delete[] info;
		return false;
    }
    return true;
}

bool load_program(GLuint &program, string vs_path, string fs_path)
{
	GLuint vs_shader, fs_shader;
	if(!load_shader(vs_shader, GL_VERTEX_SHADER, vs_path) ||
		!load_shader(fs_shader, GL_FRAGMENT_SHADER, fs_path))
		return false;
	if(!make_program(program, vs_shader, fs_shader))
		return false;
	return true;
}

void set_uniform(GLint location, const mat4 &mat) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)); }
void set_uniform(GLint location, const mat3 &mat) { glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat)); }
void set_uniform(GLint location, const mat2 &mat) { glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(mat)); }
void set_uniform(GLint location, const vec4 &vec) { glUniform4f(location, vec.x, vec.y, vec.z, vec.w); }
void set_uniform(GLint location, const vec3 &vec) { glUniform3f(location, vec.x, vec.y, vec.z); }
void set_uniform(GLint location, const vec2 &vec) { glUniform2f(location, vec.x, vec.y); }
void set_uniform(GLint location, GLdouble d) { glUniform1f(location, d); }
void set_uniform(GLint location, GLfloat f) { glUniform1f(location, f); }
void set_uniform(GLint location, GLint i) { glUniform1i(location, i); }

const char *get_gl_error_msg(GLenum code)
{
	switch(code)
	{
	case 0: return "NO_ERROR";
	case 0x0500: return "INVALID_ENUM";
	case 0x0501: return "INVALID_VALUE";
	case 0x0502: return "INVALID_OPERATION";
	case 0x0503: return "STACK_OVERFLOW";
	case 0x0504: return "STACK_UNDERFLOW";
	case 0x0505: return "OUT_OF_MEMORY";
	case 0x0506: return "INVALID_FRAMEBUFFER_OPERATION";
	default: return "UNKNOWN";
	}
}

bool check_gl_errors(std::ostream &out)
{
	bool were_errors = false;
	GLenum error = glGetError();
	while(error != GL_NO_ERROR)
	{
		out<<get_gl_error_msg(error)<<std::endl;
		were_errors = true;
		error = glGetError();
	}
	return were_errors;
}