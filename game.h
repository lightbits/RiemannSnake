#ifndef GAME_H
#define GAME_H
#include "util.h"

enum State { MenuState, PlayState };

bool load_game(GLFWwindow *window);
void on_key(GLFWwindow *window, int key, int scancode, int action, int mods);
void update_game(GLFWwindow *window, double dt);
void render_game();

#endif