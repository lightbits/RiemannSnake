#ifndef CUBE_H
#define CUBE_H
#include "util.h"
#include "shader.h"

struct Mesh
{
	GLuint vbo, ibo;
	int index_count;
};

void delete_mesh(Mesh &mesh);

Mesh generate_color_cube(float size, float r, float g, float b);
//Mesh generate_tex_cube(float size, float r, float g, float b);

Mesh generate_grid(int lines, float size, 
				   float r, float g, float b);

// Renders indexed meshes of layout
// x y z r g b a, x y z r g b a, ...
void render_pos_col(GLenum mode, Shader &shader, Mesh &mesh);

#endif

//void draw_buffer(
//	GLsizei indices, GLuint element_buffer,
//	GLuint position_buffer, GLuint color_buffer, GLuint normal_buffer, GLuint texel_buffer,
//	GLuint position_loc, GLuint color_loc, GLuint normal_loc, GLuint texel_loc)
//{
//	glEnableVertexAttribArray(position_loc);
//	glEnableVertexAttribArray(color_loc);
//	glEnableVertexAttribArray(normal_loc);
//	glEnableVertexAttribArray(texel_loc);
//	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
//	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
//	glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
//	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, texel_buffer);
//	glVertexAttribPointer(texel_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
//	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glDisableVertexAttribArray(position_loc);
//	glDisableVertexAttribArray(color_loc);
//	glDisableVertexAttribArray(normal_loc);
//	glDisableVertexAttribArray(texel_loc);
//}