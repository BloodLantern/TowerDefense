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

	PathNode()
		: x(0), y(0), direction(PathNodeDir::DIR_RIGHT)
	{ }

	PathNode(uint8_t _x, uint8_t _y, PathNodeDir dir)
		: x(_x), y(_y), direction(dir)
	{ }

	bool operator==(const PathNode& n)
	{
		return x == n.x && y == n.y;
	}
};

class Path
{
};

