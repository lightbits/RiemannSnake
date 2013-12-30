#ifndef AUDIO_H
#define AUDIO_H
#include <string>

bool load_audio();
bool load_sound(const std::string &filename);
void free_audio();

void play_sound(const std::string &track_name, bool looped);
void stop_all_sounds();

#endif