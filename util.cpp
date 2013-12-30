#include "util.h"
#include "fileio.h"
#include <iostream>

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
void set_uniform(GLint location, GLdouble d) { glUniform1f(location, float(d)); }
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
		out << "An OpenGL error occured: " << get_gl_error_msg(error) << std::endl;
		were_errors = true;
		error = glGetError();
	}
	return were_errors;
}

vec2i get_window_size(GLFWwindow *window)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	return vec2i(width, height);
}

vec4 to_rgb(uint32 hex)
{
	return vec4(
		((hex>>24) & 0xff) / 255.0f,
		((hex>>16) & 0xff) / 255.0f,
		((hex>>8) & 0xff) / 255.0f,
		(hex & 0xff) / 255.0f);
}

bool is_close(const vec3 &a, const vec3 &b, float ra, float rb)
{
	vec3 ab = a - b;
	float dist_sqrd = glm::dot(ab, ab);
	float d = ra + rb;
	float d_sqrd = d * d;
	return dist_sqrd <= d_sqrd;
}