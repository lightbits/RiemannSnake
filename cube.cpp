#include "cube.h"
#include "util.h"
#include <vector>

void delete_mesh(Mesh &mesh)
{
	glDeleteBuffers(1, &mesh.vbo);
	glDeleteBuffers(1, &mesh.ibo);
}

Mesh generate_color_cube(float size, 
						 const vec3 &color)
{
	Mesh mesh;
	float hs = size / 2.0f;
	float r = color.x;
	float g = color.g;
	float b = color.b;

	float vertices[] = {
		// Front
		-hs, -hs, +hs, r, g, b, 1.0f,
		+hs, -hs, +hs, r, g, b, 1.0f,
		+hs, +hs, +hs, r, g, b, 1.0f,
		-hs, +hs, +hs, r, g, b, 1.0f,

		// Back
		-hs, -hs, -hs, r, g, b, 1.0f,
		-hs, +hs, -hs, r, g, b, 1.0f,
		+hs, +hs, -hs, r, g, b, 1.0f,
		+hs, -hs, -hs, r, g, b, 1.0f,

		// Left
		-hs, -hs, -hs, r, g, b, 1.0f,
		-hs, -hs, +hs, r, g, b, 1.0f,
		-hs, +hs, +hs, r, g, b, 1.0f,
		-hs, +hs, -hs, r, g, b, 1.0f,

		// Right
		+hs, -hs, +hs, r, g, b, 1.0f,
		+hs, -hs, -hs, r, g, b, 1.0f,
		+hs, +hs, -hs, r, g, b, 1.0f,
		+hs, +hs, +hs, r, g, b, 1.0f,

		// Top
		-hs, +hs, +hs, r, g, b, 1.0f,
		+hs, +hs, +hs, r, g, b, 1.0f,
		+hs, +hs, -hs, r, g, b, 1.0f,
		-hs, +hs, -hs, r, g, b, 1.0f,

		// Bottom
		-hs, -hs, +hs, r, g, b, 1.0f,
		-hs, -hs, -hs, r, g, b, 1.0f,
		+hs, -hs, -hs, r, g, b, 1.0f,
		+hs, -hs, +hs, r, g, b, 1.0f
	};

	uint32 indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20,
	};

	mesh.vbo = gen_buffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	mesh.ibo = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);
	mesh.index_count = 6 * 6;
	return mesh;
}

Mesh generate_grid(int lines, 
				   float size, 
				   const vec3 &color)
{
	Mesh mesh;
	std::vector<float> vertex_buffer;
	std::vector<uint32> index_buffer;

	float hs = size / 2.0f;
	for (int i = 0; i <= lines; ++i)
	{
		float f = (i / float(lines)) * 2.0f - 1.0f;
		f *= hs;
		float vertices[] = {
			-hs, 0.0f, f, color.x, color.y, color.z, 1.0f,
			+hs, 0.0f, f, color.x, color.y, color.z, 1.0f,

			f, 0.0f, +hs, color.x, color.y, color.z, 1.0f,
			f, 0.0f, -hs, color.x, color.y, color.z, 1.0f
		};
		vertex_buffer.insert(vertex_buffer.end(), vertices, vertices + 28);

		uint32 j = index_buffer.size();
		uint32 indices[] = { j, j + 1, j + 2, j + 3 };
		index_buffer.insert(index_buffer.end(), indices, indices + 4);
	}

	mesh.vbo = gen_buffer(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), &vertex_buffer[0]);
	mesh.ibo = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(uint32), &index_buffer[0]);
	mesh.index_count = index_buffer.size();
	return mesh;
}

void render_pos_col(GLenum mode, Shader &shader, Mesh &mesh)
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	shader.set_attribfv("position", 3, 7, 0);
	shader.set_attribfv("color", 4, 7, 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glDrawElements(mode, mesh.index_count, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.unset_attrib("position");
	shader.unset_attrib("color");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}