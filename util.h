#ifndef UTIL_H
#define UTIL_H
#include <glload/gl_3_1.h> // OpenGL ver. 3.2, core profile, C-style functions
#include <glload/gll.h> // Loading interface
#include <GLFW/glfw3.h> // GLFW windowing library
#include "types.h"
#define M_PI 3.14159265359
#define M_TWO_PI 6.28318530718
#define M_PI_TWO 1.57079632679

GLuint gen_buffer(GLenum target, GLsizei size, const void *data);
bool make_shader(GLuint &shader, GLenum type, string source);
bool make_program(GLuint &program, GLuint vs_shader, GLuint fs_shader);
bool load_shader(GLuint &shader, GLenum type, string path);
bool load_program(GLuint &program, string vs_path, string fs_path);
vec2i get_window_size(GLFWwindow *window);

vec4 to_rgb(uint32 hex);
bool is_close(const vec3 &a, const vec3 &b, float ra, float rb);

// Converts a GLenum to a text error
const char *get_gl_error_msg(GLenum code);

// Polls for OpenGL errors and writes them to the output stream.
// Returns true if there were errors.
bool check_gl_errors(std::ostream &out);

#endif