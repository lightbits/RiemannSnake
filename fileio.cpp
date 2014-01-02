#include "fileio.h"
#include <fstream>
#include <iostream>

static std::stringstream log_stream("");

bool read_file(const std::string &path, std::string &dest)
{
	std::ifstream in(path, std::ios::in | std::ios::binary);
	if(!in.is_open())
	{
		log_msg("Could not open file: " + path);
		return false;
	}

	if(in.good())
	{
		in.seekg(0, std::ios::end);			// Set get position to end
		dest.resize(in.tellg());			// Resize string to support enough bytes
		in.seekg(0, std::ios::beg);			// Set get position to beginning
		in.read(&dest[0], dest.size());		// Read file to string
		in.close();
	}

	return true;
}

void log_msg(const std::string &msg)
{
	log_stream << msg << std::endl;
}

void dump_log()
{
	if (log_stream.str().size() > 0)
	{
#ifdef DEBUG
		std::cerr << log_stream.str();
		std::cin.get();
#endif
		std::ofstream file("./log.txt");
		file<<log_stream.str();
		file.close();
		log_stream.clear();
	}
}

std::stringstream &get_log_stream()
{
	return log_stream;
}

int read_int(std::stringstream &ss)
{
	int i; ss>>i;
	return i;
}

bool read_bool(std::stringstream &ss)
{
	bool i; ss>>i;
	return i;
}

std::string read_word(std::stringstream &ss)
{
	std::string word; ss>>word;
	return word;
}