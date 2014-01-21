#include "cube.h"
#include "util.h"
#include <vector>
#include <iostream>

void delete_mesh(Mesh &mesh)
{
	glDeleteBuffers(1, &mesh.vbo);
	glDeleteBuffers(1, &mesh.ibo);
}

Mesh generate_wireframe_sphere(float radius,
							   int t_samples,
							   int s_samples,
							   const vec4 &color)
{
	std::vector<float> vertex_buffer;
	std::vector<uint32> index_buffer;

	float dtheta = TWO_PI / float(t_samples);
	float dphi = PI / float(s_samples);
	float rho = radius;
	int vertex_count = 0;
	for (int t = 0; t < t_samples; ++t)
	{
		for (int s = 0; s < s_samples; ++s)
		{
			float theta = t * dtheta;
			float phi = s * dphi;

			float r0 = rho * sin(phi);
			float r1 = rho * sin(phi + dphi);

			vec3 v00(r0 * cos(theta), rho * cos(phi), r0 * sin(theta));
			vec3 v10(r0 * cos(theta + dtheta), rho * cos(phi), r0 * sin(theta + dtheta));
			vec3 v01(r1 * cos(theta), rho * cos(phi + dphi), r1 * sin(theta));
			vec3 v11(r1 * cos(theta + dtheta), rho * cos(phi + dphi), r1 * sin(theta + dtheta));
			
			float vertices[] = {
				v00.x, v00.y, v00.z, 0.0f, 0.0f, 1.0f, color.r, color.g, color.b, color.a,
				v01.x, v01.y, v01.z, 0.0f, 1.0f, 0.0f, color.r, color.g, color.b, color.a,
				v11.x, v11.y, v11.z, 1.0f, 0.0f, 0.0f, color.r, color.g, color.b, color.a,
				v10.x, v10.y, v10.z, 0.0f, 1.0f, 0.0f, color.r, color.g, color.b, color.a
			};

			uint32 j = vertex_count;
			uint32 indices[] = { j, j + 1, j + 2, j + 2, j + 3, j };
			vertex_count += 4;
			vertex_buffer.insert(vertex_buffer.end(), vertices, vertices + 40);
			index_buffer.insert(index_buffer.end(), indices, indices + 6);
		}
	}

	Mesh mesh;
	mesh.vbo = gen_buffer(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), &vertex_buffer[0]);
	mesh.ibo = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(uint32), &index_buffer[0]);
	mesh.index_count = index_buffer.size();
	return mesh;
}

Mesh generate_sphere(float radius, 
					 int t_samples,
					 int s_samples,
					 const vec4 &color)
{
	std::vector<float> vertex_buffer;
	std::vector<uint32> index_buffer;

	float dtheta = TWO_PI / float(t_samples);
	float dphi = PI / float(s_samples);
	float rho = radius;
	int vertex_count = 0;
	for (int t = 0; t < t_samples; ++t)
	{
		for (int s = 0; s < s_samples; ++s)
		{
			float theta = t * dtheta;
			float phi = s * dphi;

			float r0 = rho * sin(phi);
			float r1 = rho * sin(phi + dphi);

			vec3 v00(r0 * cos(theta), rho * cos(phi), r0 * sin(theta));
			vec3 v10(r0 * cos(theta + dtheta), rho * cos(phi), r0 * sin(theta + dtheta));
			vec3 v01(r1 * cos(theta), rho * cos(phi + dphi), r1 * sin(theta));
			vec3 v11(r1 * cos(theta + dtheta), rho * cos(phi + dphi), r1 * sin(theta + dtheta));
			
			float vertices[] = {
				v00.x, v00.y, v00.z, color.r, color.g, color.b, color.a,
				v01.x, v01.y, v01.z, color.r, color.g, color.b, color.a,
				v11.x, v11.y, v11.z, color.r, color.g, color.b, color.a,
				v10.x, v10.y, v10.z, color.r, color.g, color.b, color.a
			};

			uint32 j = vertex_count;
			uint32 indices[] = { j, j + 1, j + 2, j + 2, j + 3, j };
			vertex_count += 4;
			vertex_buffer.insert(vertex_buffer.end(), vertices, vertices + 28);
			index_buffer.insert(index_buffer.end(), indices, indices + 6);
		}
	}

	Mesh mesh;
	mesh.vbo = gen_buffer(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), &vertex_buffer[0]);
	mesh.ibo = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(uint32), &index_buffer[0]);
	mesh.index_count = index_buffer.size();
	return mesh;
}

Mesh generate_color_cube(float size, 
						 const vec4 &color)
{
	Mesh mesh;
	float hs = size / 2.0f;
	float r = color.x;
	float g = color.g;
	float b = color.b;
	float a = color.a;

	float vertices[] = {
		// Front
		-hs, -hs, +hs, r, g, b, a,
		+hs, -hs, +hs, r, g, b, a,
		+hs, +hs, +hs, r, g, b, a,
		-hs, +hs, +hs, r, g, b, a,

		// Back
		-hs, -hs, -hs, r, g, b, a,
		-hs, +hs, -hs, r, g, b, a,
		+hs, +hs, -hs, r, g, b, a,
		+hs, -hs, -hs, r, g, b, a,

		// Left
		-hs, -hs, -hs, r, g, b, a,
		-hs, -hs, +hs, r, g, b, a,
		-hs, +hs, +hs, r, g, b, a,
		-hs, +hs, -hs, r, g, b, a,

		// Right
		+hs, -hs, +hs, r, g, b, a,
		+hs, -hs, -hs, r, g, b, a,
		+hs, +hs, -hs, r, g, b, a,
		+hs, +hs, +hs, r, g, b, a,

		// Top
		-hs, +hs, +hs, r, g, b, a,
		+hs, +hs, +hs, r, g, b, a,
		+hs, +hs, -hs, r, g, b, a,
		-hs, +hs, -hs, r, g, b, a,

		// Bottom
		-hs, -hs, +hs, r, g, b, a,
		-hs, -hs, -hs, r, g, b, a,
		+hs, -hs, -hs, r, g, b, a,
		+hs, -hs, +hs, r, g, b, a
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

Mesh generate_quad()
{
	float vertices[] = {
		-1.0f, -1.0f,
		+1.0f, -1.0f,
		+1.0f, +1.0f,
		-1.0f, +1.0f
	};

	uint32 indices[] = { 0, 1, 2, 2, 3, 0 };

	Mesh mesh;
	mesh.vbo = gen_buffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	mesh.ibo = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);
	mesh.index_count = 6;
	return mesh;
}

void render_colored(GLenum mode, Shader &shader, Mesh &mesh)
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

void render_wireframe(GLenum mode, Shader &shader, Mesh &mesh)
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	shader.set_attribfv("position", 3, 10, 0);
	shader.set_attribfv("barycentric", 3, 10, 3);
	shader.set_attribfv("color", 4, 10, 6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glDrawElements(mode, mesh.index_count, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.unset_attrib("position");
	shader.unset_attrib("color");
	shader.unset_attrib("barycentric");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}