#pragma once

#include <stdint.h>

enum class PathNodeDir : uint8_t
{
	DIR_RIGHT,
	DIR_LEFT,
	DIR_UP,
	DIR_DOWN,
};

struct PathNode
{
	uint8_t x;
	uint8_t y;
	PathNodeDir direction;

	PathNode(uint8_t _x, uint8_t _y, PathNodeDir dir)
		: x(_x), y(_y), direction(dir)
	{ }
};

class Path
{
};

