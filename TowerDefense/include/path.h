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
};

class Path
{
};

