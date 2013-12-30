#include "noise.h"

// http://en.wikipedia.org/wiki/Xorshift
// Returns a random number with a period of 2^128 - 1
unsigned int xor128()
{
	static unsigned int x = 123456789;
	static unsigned int y = 362436069;
	static unsigned int z = 521288629;
	static unsigned int w = 88675123;
	unsigned int t;

	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >>8));
}

// Returns a single precision floating-point value uniformly over the interval [0.0, 1.0]
float frand()
{
	return xor128() / float(4294967295.0f);
}