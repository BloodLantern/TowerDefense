#pragma once

#include "Entity.h"
#include "TowerBarUI.h"
#include <stdint.h>

#define GRID_WIDTH 42
#define GRID_HEIGHT 18
#define GRID_SQUARE_SIZE 32

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
	SquareType m_clipdata[GRID_HEIGHT][GRID_WIDTH];
	uint8_t m_tilemap[GRID_HEIGHT][GRID_WIDTH];

	bool maxR;
	size_t r;
	TowerBarUI towerBarUI;

public:
	PlayField();
	~PlayField();

	void draw();
	void save(std::string dst);
	void load(std::string src);

	void setClipdataTile(uint8_t x, uint8_t y, SquareType type);
	SquareType getClipdataTile(uint8_t x, uint8_t y);
	void getGridPositionFromCoords(int32_t screenX, int32_t screenY, uint8_t& tileX, uint8_t& tileY);
};
