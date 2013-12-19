#ifndef GAME_H
#define GAME_H
#include "util.h"

bool load_game(GLFWwindow *window);
void init_game(GLFWwindow *window);
void free_game(GLFWwindow *window);
void update_game(GLFWwindow *window, double dt);
void render_game(GLFWwindow *window, double dt);
void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif