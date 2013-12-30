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
						 const string &char_set,
						 const std::vector<uint8> &pixels, 
						 uint32 width, uint32 height)
{
	GlyphMap glyphs;
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

bool load_font(Font &font, const string &path, const string &char_set)
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
	font.glyphs = generate_glyphs(font, char_set, pixels, width, height);

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

vec2i measure_string(const Font &font, const string &text)
{
	int height = font.char_height;
	int width = 0;
	int curr_width = 0;

	for (const char &c : text) {
		Glyph glyph = font.glyphs.find(c)->second;
		if (c == '\n')
		{
			height += font.char_height;
			curr_width = 0;
		}
		curr_width += glyph.width;
		if (curr_width > width)
			width = curr_width;
	}

	return vec2i(width, height);
}

void draw_string(const Font &font, Shader &shader, float x, float y, const vec4 &color, const string &text, bool centered)
{
	// Nothing to draw
	if (text.size() == 0)
		return;

	if (centered)
	{
		vec2i size = measure_string(font, text);
		x -= size.x / 2.0f;
		y -= size.y / 2.0f;
	}

	// Draw at integer coordinates
	x = glm::round(x);
	y = glm::round(y);

	std::vector<float> v_buffer;
	std::vector<uint32> i_buffer;

	float r = color.x;
	float g = color.y;
	float b = color.z;
	float a = color.w;

	float cx = x;
	float cy = y;
	uint32 i = 0;
	float letter = 0.0f;
	for (const char &c : text) {
		Glyph glyph = font.glyphs.find(c)->second;
		if (c == '\n')
		{
			cy += font.char_height;
			cx = x;
			continue;
		}

		float w = float(glyph.width);
		float h = float(glyph.height);
		float quad[] = {
			// Position		// Color		// Texel
			cx, cy,			r, g, b, a,		glyph.u_left, glyph.v_bottom, letter,
			cx + w, cy,		r, g, b, a,		glyph.u_right, glyph.v_bottom, letter,
			cx + w, cy + h, r, g, b, a,		glyph.u_right, glyph.v_top, letter,
			cx, cy + h,		r, g, b, a,		glyph.u_left, glyph.v_top, letter
		};
		uint32 indices[] = { i, i + 1, i + 2, i + 2, i + 3, i };

		v_buffer.insert(v_buffer.end(), quad, quad + 36);
		i_buffer.insert(i_buffer.end(), indices, indices + 6);

		cx += w;
		i += 4;
		letter += 1.0f;
	}

	uint32 index_count = i_buffer.size();

	GLuint vbo = gen_buffer(GL_ARRAY_BUFFER, v_buffer.size() * sizeof(float), &v_buffer[0]);
	GLuint ibo = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer.size() * sizeof(float), &i_buffer[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	shader.set_attribfv("position", 2, 9, 0);
	shader.set_attribfv("color", 4, 9, 2);
	shader.set_attribfv("texel", 2, 9, 6);
	shader.set_attribfv("letter", 2, 9, 8);
	shader.set_uniform("tex", 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindTexture(GL_TEXTURE_2D, font.texture);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.unset_attrib("position");
	shader.unset_attrib("color");
	shader.unset_attrib("texel");
	shader.unset_attrib("letter");
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void draw_string(const Font &font, Shader &shader, float x, float y, const string &text, bool centered)
{
	draw_string(font, shader, x, y, vec4(1.0f), text, centered);
}