#include "font.h"
#include "lodepng/lodepng.h"
#include <iostream>

void delete_font(Font &font)
{
	font.glyphs.clear();
	glDeleteTextures(1, &font.texture);
}

typedef std::unordered_map<char, Glyph> GlyphMap;

bool is_border_pixel(const std::vector<uint8> &pixels, 
					 uint32 x, uint32 y, uint32 width)
{
	uint32 offset = (y * width + x) * 4;
	return pixels[offset] == 0 && 
		pixels[offset + 1] == 0 && 
		pixels[offset + 2] == 0 && 
		pixels[offset + 3] == 255;
}

GlyphMap generate_glyphs(Font &font,
	const std::vector<uint8> &pixels, 
	uint32 width, uint32 height)
{
	GlyphMap glyphs;
	string char_set = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	uint32 num_chars = char_set.size();
	
	uint32 index = 0;
	for (uint32 y = 0; y < height; ++y)
	{
		uint32 skip_y = 0;
		for (uint32 x = 0; x < width; ++x)
		{
			if (is_border_pixel(pixels, x, y, width) && index < num_chars)
			{
				// Determine width of the glyph
				int w = 1;
				while (x + w < width && is_border_pixel(pixels, x + w, y, width))
					++w;

				// Determine height of the glyph
				int h = 1;
				while (y + h < height && is_border_pixel(pixels, x, y + h, width))
					++h;

				Glyph glyph;
				glyph.width = w - 2;
				glyph.height = h - 2;
				glyph.u_left = (x + 1) / float(width);
				glyph.u_right = (x + w - 1) / float(width);
				glyph.v_bottom = (y + 1) / float(height);
				glyph.v_top = (y + h - 1) / float(height);

				glyphs[char_set[index++]] = glyph;

				x += w;
				if (h > skip_y)
					skip_y = h;

				if (h > font.char_height)
					font.char_height = h;
			}
		}

		// Skip pixels (downwards) if we found any in glyphs the last horizontal-scan
		y += skip_y;
	}

	return glyphs;
}

bool load_font(Font &font, const string &path)
{
	//http://lodev.org/lodepng/example_decode.cpp
	//if (!font_shader_loaded)

	std::vector<uint8> pixels;
	uint32 width, height;

	uint32 error = lodepng::decode(pixels, width, height, path);

	if (error)
	{
		std::cerr<<"Decoder error "<<error<<": "<<lodepng_error_text(error)<<std::endl;
		return false;
	}

	// The pixels are now in pixels vector, 4 bytes per pixel, ordered RGBARGBA...
	// Row-wise, top-left to bottom-right

	delete_font(font);

	font.glyphs.clear();
	font.glyphs = generate_glyphs(font, pixels, width, height);

	glGenTextures(1, &font.texture);
	glBindTexture(GL_TEXTURE_2D, font.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void draw_string(const Font &font, Shader &shader, float x, float y, float r, float g, float b, float a, const string &text)
{
	std::vector<float> v_buffer;
	std::vector<uint32> i_buffer;

	float cx = x;
	float cy = y;
	uint32 i = 0;
	for (const char &c : text) {
		Glyph glyph = font.glyphs.find(c)->second;
		if (c == '\n')
		{
			cy += font.char_height;
			cx = x;
			continue; // Forgetting this causes weird stuff, investigate it
		}

		float w = float(glyph.width);
		float h = float(glyph.height);
		float quad[] = {
			// Position		// Color		// Texel
			cx, cy,			r, g, b, a,		glyph.u_left, glyph.v_bottom,
			cx + w, cy,		r, g, b, a,		glyph.u_right, glyph.v_bottom,
			cx + w, cy + h, r, g, b, a,		glyph.u_right, glyph.v_top,
			cx, cy + h,		r, g, b, a,		glyph.u_left, glyph.v_top
		};
		uint32 indices[] = { i, i + 1, i + 2, i + 2, i + 3, i };

		v_buffer.insert(v_buffer.end(), quad, quad + 32);
		i_buffer.insert(i_buffer.end(), indices, indices + 6);

		cx += w;
		i += 4;
	}

	uint32 index_count = i_buffer.size();

	GLuint vbo = gen_buffer(GL_ARRAY_BUFFER, v_buffer.size() * sizeof(float), &v_buffer[0]);
	GLuint ibo = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer.size() * sizeof(float), &i_buffer[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	shader.set_attribfv("position", 2, 8, 0);
	shader.set_attribfv("color", 4, 8, 2);
	shader.set_attribfv("texel", 2, 8, 6);
	shader.set_uniform("tex", 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindTexture(GL_TEXTURE_2D, font.texture);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.unset_attrib("position");
	shader.unset_attrib("color");
	shader.unset_attrib("texel");
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void draw_string(const Font &font, Shader &shader, float x, float y, const string &text)
{
	draw_string(font, shader, x, y, 1.0f, 1.0f, 1.0f, 1.0f, text);
}