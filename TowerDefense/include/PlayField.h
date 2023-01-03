#pragma once

#include <stdint.h>
#include "Entity.h"

#define GRID_WIDTH 50
#define GRID_HEIGHT 25
#define GRID_SQUARE_SIZE 26

#define GRID_LINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)

#define MAPS_PATH "maps\\"

enum SquareType : uint8_t
{
	SQUARE_TYPE_EMPTY,
	SQUARE_TYPE_NOTHING,
	SQUARE_TYPE_ENEMY_ONLY,
	SQUARE_TYPE_OCCUPIED,
	SQUARE_TYPE_PLAYER_ONLY,
};

class PlayField
{
private:
	SquareType m_grid[GRID_HEIGHT][GRID_WIDTH];

	bool maxR;
	size_t r;
	Entity e;

public:
	PlayField();
	~PlayField();

	void draw();
	void save(std::string dst);
	void load(std::string src);

	void setGridTile(uint8_t x, uint8_t y, SquareType type);
	SquareType getGridTile(uint8_t x, uint8_t y);
	void getGridPositionFromCoords(int32_t screenX, int32_t screenY, uint8_t& tileX, uint8_t& tileY);
};
