#ifndef UTIL_H
#define UTIL_H
#include <glload/gl_3_1.h> // OpenGL ver. 3.2, core profile, C-style functions
#include <glload/gll.h> // Loading interface
#include <GLFW/glfw3.h> // GLFW windowing library
#include "types.h"

GLuint gen_buffer(GLenum target, GLsizei size, const void *data);
GLuint make_shader(GLenum type, string source);
GLuint load_shader(GLenum type, string path);
GLuint make_program(GLuint vs_shader, GLuint fs_shader);
GLuint load_program(string vs_path, string fs_path);

// Converts a GLenum to a text error
const char *get_gl_error_msg(GLenum code);

// Polls for OpenGL errors and writes them to the output stream.
// Returns true if there were errors.
bool check_gl_errors(std::ostream &out);

#endif