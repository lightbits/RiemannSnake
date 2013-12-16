#include "timer.h"
#include <chrono>
#include <thread>

void sleep(double seconds)
{
	sleep_ms((unsigned int)(seconds * 1000.0));
}

void sleep_ms(unsigned int ms)
{
	std::chrono::milliseconds duration(ms);
	std::this_thread::sleep_for(duration);
}