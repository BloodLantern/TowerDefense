#pragma once

#include "entity.hpp"
#include "tower_bar_ui.hpp"
#include "imgui_utils.hpp"
#include "path.hpp"
#include <stdint.h>

#define GRID_OFFSET_X 0
#define GRID_OFFSET_Y 18

#define GRID_SQUARE_SIZE 32
#define GRID_SQUARE_LINE_SIZE 1

#define PLAYFIELD_WIDTH GRID_WIDTH * (GRID_SQUARE_SIZE + GRID_SQUARE_LINE_SIZE)
#define PLAYFIELD_HEIGHT GRID_HEIGHT * (GRID_SQUARE_SIZE + GRID_SQUARE_LINE_SIZE)

#define GRID_LINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)

#define MAPS_PATH "data\\maps\\"

/// <summary>
/// Clipdata type, represents the collision/type of a tile
/// </summary>
enum ClipdataType : uint8_t
{
	CLIPDATA_TYPE_EMPTY,
	CLIPDATA_TYPE_NOTHING,
	CLIPDATA_TYPE_ENEMY_ONLY,
	CLIPDATA_TYPE_OCCUPIED,
	CLIPDATA_TYPE_PLAYER_ONLY,
};

/// <summary>
/// Draw flags of the playfield, used to manually enable/disable each part
/// </summary>
enum PlayFieldDrawFlags : uint32_t
{
	PLAYFIELD_DRAW_FLAGS_NONE = 0,
	PLAYFIELD_DRAW_FLAGS_CLIPDATA = 1 << 1,
	PLAYFIELD_DRAW_FLAGS_GRID_LINES = 1 << 2,
	PLAYFIELD_DRAW_FLAGS_LAYER0 = 1 << 3,
	PLAYFIELD_DRAW_FLAGS_LAYER1 = 1 << 4,
	PLAYFIELD_DRAW_FLAGS_LAYER2 = 1 << 5,
};

/// <summary>
/// Type of the operation to do on the playfield flags
/// </summary>
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
	// -- Internal grid state --

	// Collision
	std::vector<ClipdataType> mClipdata;
	std::vector<ClipdataType> mClipdataAtStart;
	
	// Layers tilemap, layer 2 is drawn first, then 1, then 0
	std::vector<uint16_t> mLayer2Tilemap;
	std::vector<uint16_t> mLayer1Tilemap;
	std::vector<uint16_t> mLayer0Tilemap;

	// Path nodes
	std::vector<PathNode> mPathNodes;

	// Draw flags
	PlayFieldDrawFlags mDrawFlags;

	// Tower bar UI, not sure if this is the best place?
	TowerBarUI towerBarUI;

	// Draw functions
	void DrawClipdata();
	void DrawLayers();
	void DrawLines();
	void DrawPath();

public:
	// Tileset and size
	Texture* tileset;
	uint16_t gridWidth;
	uint16_t gridHeight;

	PlayField();
	~PlayField();

	/// <summary>
	/// Resizes the playfield
	/// </summary>
	/// <param name="width">New width</param>
	/// <param name="height">New height</param>
	void Resize(uint16_t width, uint16_t height);

	/// <summary>
	/// Gets the draw flags
	/// </summary>
	/// <returns>Draw flags</returns>
	PlayFieldDrawFlags GetDrawFlags();

	/// <summary>
	/// Updates the draw flags with the requested operation and value
	/// </summary>
	/// <param name="operation">Operation</param>
	/// <param name="flags">Value</param>
	void SetDrawFlags(PlayFieldDrawFlagsOperation operation, PlayFieldDrawFlags flags);

	/// <summary>
	/// Draws the playfield
	/// </summary>
	void Draw();

	/// <summary>
	/// Saves the playfield into a file
	/// </summary>
	/// <param name="src">Destination file name</param>
	void Save(std::string dst);

	/// <summary>
	/// Loads a file into the playfield
	/// </summary>
	/// <param name="dst">Source file name</param>
	void Load(std::string src);

	/// <summary>
	/// Sets a clipdata tile
	/// </summary>
	/// <param name="x">Tile X position</param>
	/// <param name="y">Tile Y position</param>
	/// <param name="type">Clipdata value</param>
	void SetClipdataTile(uint8_t x, uint8_t y, ClipdataType type);

	/// <summary>
	/// Resets a clipdata tile to it's original state
	/// </summary>
	/// <param name="x">Tile X position</param>
	/// <param name="y">Tile Y position</param>
	void ResetClipdataTile(uint8_t x, uint8_t y);

	/// <summary>
	/// Resets the entire clipdata board
	/// </summary>
	void ResetEntireClipdata();

	/// <summary>
	/// Gets a clipdata tile
	/// </summary>
	/// <param name="x">Tile X position</param>
	/// <param name="y">Tile Y position</param>
	/// <returns>Clipdata tile value</returns>
	ClipdataType GetClipdataTile(uint8_t x, uint8_t y);

	/// <summary>
	/// Loads a tileset
	/// </summary>
	/// <param name="name">File name</param>
	void LoadTileset(const char* name);

	/// <summary>
	/// Set a layer tile
	/// </summary>
	/// <param name="x">Tile X position</param>
	/// <param name="y">Tile Y position</param>
	/// <param name="layer">Layer</param>
	/// <param name="value">Value</param>
	void SetLayertile(uint8_t x, uint8_t y, uint8_t layer, uint16_t value);

	/// <summary>
	/// Get a layer tile
	/// </summary>
	/// <param name="x">Tile X position</param>
	/// <param name="y">Tile Y position</param>
	/// <param name="layer">Layer</param>
	/// <returns>Value</returns>
	uint16_t GetLayerTile(uint8_t x, uint8_t y, uint8_t layer);

	/// <summary>
	/// Gets the raw clipdata pointer
	/// </summary>
	/// <returns>Clipdata pointer</returns>
	ClipdataType* GetClipdataPointer();

	/// <summary>
	/// Gets a raw tilemap pointer
	/// </summary>
	/// <param name="layer">Layer</param>
	/// <returns>Rzw tilemap pointer (can be null)</returns>
	uint16_t* GetTilemapPointer(uint8_t layer);

	/// <summary>
	/// Get a reference to the path nodes
	/// </summary>
	/// <returns>Path nodes</returns>
	std::vector<PathNode>& GetPathNodes();

	/// <summary>
	/// Get the next path node based on the parameters
	/// </summary>
	/// <param name="x">Node tile X position</param>
	/// <param name="y">Node tile Y position</param>
	/// <param name="direction">Node direction</param>
	/// <returns><Next path node/returns>
	PathNode GetNextPathNode(uint8_t x, uint8_t y, PathNodeDir direction);

	/// <summary>
	/// Converts a pixel position to a grid position
	/// </summary>
	/// <param name="pixelX">Pixel position X</param>
	/// <param name="pixelY">Pixel position Y</param>
	/// <param name="tileX">Tile position X</param>
	/// <param name="tileY">Tile position Y</param>
	void GetGridPositionFromPixels(int32_t pixelX, int32_t pixelY, uint8_t& tileX, uint8_t& tileY);

	/// <summary>
	/// Converts a grid position to a pixel position
	/// </summary>
	/// <param name="tileX">Tile position X</param>
	/// <param name="tileY">Tile position Y</param>
	/// <param name="pixelX">Pixel position X</param>
	/// <param name="pixelY">Pixel position Y</param>
	void GetPixelPositionFromGrid(uint8_t tileX, uint8_t tileY, int32_t& pixelX, int32_t& pixelY);
};
