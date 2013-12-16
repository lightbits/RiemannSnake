#ifndef GAME_H
#define GAME_H
#include "util.h"

enum State { MenuState, PlayState };

struct Performance
{
	Performance() : render_time(0), update_time(0) { }
	double render_time;
	double update_time;
};

// Global performance info
extern Performance perf;

bool load_game(GLFWwindow *window);
void on_key(GLFWwindow *window, int key, int scancode, int action, int mods);
void update_game(GLFWwindow *window, double dt);
void render_game();

#endif