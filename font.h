#ifndef FONT_H
#define FONT_H
#include "types.h"
#include "util.h"
#include "shader.h"
#include <unordered_map>
// http://en.cppreference.com/w/cpp/string/basic_string/to_string INFINITELY USEFUL!

struct Glyph
{
	float u_left, u_right, v_bottom, v_top;
	int width;
	int height;
};

struct Font
{
	std::unordered_map<char, Glyph> glyphs;
	int char_height;
	GLuint texture;
};

void delete_font(Font &font);
bool load_font(Font &font, const string &path, const string &char_set = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
vec2i measure_string(const Font &font, const string &text);
void draw_string(const Font &font, Shader &shader, 
				 float x, float y, 
				 const vec4 &color,
				 const string &text,
				 bool centered = false);
void draw_string(const Font &font, Shader &shader, 
				 float x, float y, 
				 const string &text,
				 bool centered = false);

#endif