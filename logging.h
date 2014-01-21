#ifndef LOGGING_H
#define LOGGING_H
#include <iostream>
#include <sstream>

std::stringstream &get_log();
void dump_log_file(const char *filename);

#endif