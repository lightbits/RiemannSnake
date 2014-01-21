#include "logging.h"
#include <fstream>

static std::stringstream common_log;

std::stringstream &get_log()
{
	return common_log;
}

void dump_log_file(const char *filename)
{
	if (common_log.str().size() > 0)
	{
#ifdef DEBUG
		std::cerr << common_log.str();
		std::cin.get();
#endif
		std::ofstream file(filename);
		if (file.is_open())
		{
		
			file << common_log.str();
			common_log.clear();
			file.close();
		}
		else
		{
			// Whoops
			std::cerr << "Failed to dump log file" << std::endl;
		}
	}
}