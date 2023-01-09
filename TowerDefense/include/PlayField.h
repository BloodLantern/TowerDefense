#pragma once

#include "Entity.h"
#include "TowerBarUI.h"
#include "imgui_utils.hpp"
#include <stdint.h>

#define GRID_OFFSET_X 0
#define GRID_OFFSET_Y 18

#define GRID_SQUARE_SIZE 32
#define GRID_SQUARE_LINE_SIZE 1

#define PLAYFIELD_WIDTH GRID_WIDTH * (GRID_SQUARE_SIZE + GRID_SQUARE_LINE_SIZE)
#define PLAYFIELD_HEIGHT GRID_HEIGHT * (GRID_SQUARE_SIZE + GRID_SQUARE_LINE_SIZE)

#define GRID_LINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)

#define MAPS_PATH "maps\\"

enum ClipdataType : uint8_t
{
	CLIPDATA_TYPE_EMPTY,
	CLIPDATA_TYPE_NOTHING,
	CLIPDATA_TYPE_ENEMY_ONLY,
	CLIPDATA_TYPE_OCCUPIED,
	CLIPDATA_TYPE_PLAYER_ONLY,
};

enum PlayFieldDrawFlags : uint32_t
{
	PLAYFIELD_DRAW_FLAGS_NONE = 0,
	PLAYFIELD_DRAW_FLAGS_CLIPDATA = 1 << 1,
	PLAYFIELD_DRAW_FLAGS_GRID_LINES = 1 << 2,
	PLAYFIELD_DRAW_FLAGS_LAYER0 = 1 << 3,
	PLAYFIELD_DRAW_FLAGS_LAYER1 = 1 << 4,
};

enum PlayFieldDrawFlagsOperation : uint8_t
{
	PLAYFIELD_DRAW_FLAGS_OPERATION_ADD = 0,
	PLAYFIELD_DRAW_FLAGS_OPERATION_REMOVE = 1,
	PLAYFIELD_DRAW_FLAGS_OPERATION_TOGGLE = 2,
	PLAYFIELD_DRAW_FLAGS_OPERATION_SET = 3,
};

class PlayField
{
private:
	std::vector<ClipdataType> m_clipdata;
	std::vector<uint16_t> m_layer0Tilemap;
	std::vector<uint16_t> m_layer1Tilemap;

	uint16_t m_drawFlags;


	bool maxR;
	size_t r;
	TowerBarUI towerBarUI;
	
	void drawClipdata();
	void drawLayers();
	void drawLines();

public:
	Texture m_tileset;
	uint16_t m_gridWidth;
	uint16_t m_gridHeight;

	PlayField();
	~PlayField();

	void resize(uint16_t width, uint16_t height);
	void setDrawFlags(PlayFieldDrawFlagsOperation operation, PlayFieldDrawFlags flags);

	void draw();
	void save(std::string dst);
	void load(std::string src);

	void setClipdataTile(uint8_t x, uint8_t y, ClipdataType type);
	ClipdataType getClipdataTile(uint8_t x, uint8_t y);

	void loadTileset(const char* name);
	void setLayertile(uint8_t x, uint8_t y, uint8_t layer, uint16_t value);

	ClipdataType* getClipdataPointer();
	uint16_t* getTilemapPointer(uint8_t layer);

	void getGridPositionFromPixels(int32_t screenX, int32_t screenY, uint8_t& tileX, uint8_t& tileY);
	void getPixelPositionFromGrid(uint8_t tileX, uint8_t tileY, int32_t& screenX, int32_t& screenY);
};
