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
	PLAYFIELD_DRAW_FLAGS_LAYER2 = 1 << 5,
};

enum PlayFieldDrawFlagsOperation : uint8_t
{
	PLAYFIELD_DRAW_FLAGS_OPERATION_ADD = 0,
	PLAYFIELD_DRAW_FLAGS_OPERATION_REMOVE = 1,
	PLAYFIELD_DRAW_FLAGS_OPERATION_TOGGLE = 2,
	PLAYFIELD_DRAW_FLAGS_OPERATION_SET = 3,
};

#pragma region Flags operators
inline PlayFieldDrawFlags operator|=(PlayFieldDrawFlags& a, PlayFieldDrawFlags b)
{
	return a = static_cast<PlayFieldDrawFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline PlayFieldDrawFlags operator|(PlayFieldDrawFlags a, PlayFieldDrawFlags b)
{
	return static_cast<PlayFieldDrawFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline PlayFieldDrawFlags operator&(PlayFieldDrawFlags a, PlayFieldDrawFlags b)
{
	return static_cast<PlayFieldDrawFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline PlayFieldDrawFlags operator&=(PlayFieldDrawFlags& a, PlayFieldDrawFlags b)
{
	return a = static_cast<PlayFieldDrawFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline PlayFieldDrawFlags operator^(PlayFieldDrawFlags a, PlayFieldDrawFlags b)
{
	return static_cast<PlayFieldDrawFlags>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
}

inline PlayFieldDrawFlags operator^=(PlayFieldDrawFlags& a, PlayFieldDrawFlags b)
{
	return a = static_cast<PlayFieldDrawFlags>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
}
#pragma endregion

class PlayField
{
private:
	std::vector<ClipdataType> mClipdata;
	std::vector<uint16_t> mLayer2Tilemap;
	std::vector<uint16_t> mLayer1Tilemap;
	std::vector<uint16_t> mLayer0Tilemap;

	PlayFieldDrawFlags mDrawFlags;


	bool maxR;
	size_t r;
	TowerBarUI towerBarUI;
	
	void DrawClipdata();
	void DrawLayers();
	void DrawLines();

public:
	Texture mTileset;
	uint16_t mGridWidth;
	uint16_t mGridHeight;

	PlayField();
	~PlayField();

	void Resize(uint16_t width, uint16_t height);
	void SetDrawFlags(PlayFieldDrawFlagsOperation operation, PlayFieldDrawFlags flags);

	void Draw();
	void Save(std::string dst);
	void Load(std::string src);

	void SetClipdataTile(uint8_t x, uint8_t y, ClipdataType type);
	ClipdataType GetClipdataTile(uint8_t x, uint8_t y);

	void LoadTileset(const char* name);
	void SetLayertile(uint8_t x, uint8_t y, uint8_t layer, uint16_t value);

	ClipdataType* GetClipdataPointer();
	uint16_t* GetTilemapPointer(uint8_t layer);

	void GetGridPositionFromPixels(int32_t pixelX, int32_t pixelY, uint8_t& tileX, uint8_t& tileY);
	void GetPixelPositionFromGrid(uint8_t tileX, uint8_t tileY, int32_t& pixelX, int32_t& pixelY);
};
