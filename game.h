#ifndef GAME_H
#define GAME_H
#include "util.h"

bool load_game();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void update_game(double dt);
void render_game();

#endif