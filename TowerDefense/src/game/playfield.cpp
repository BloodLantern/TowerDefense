#include "playfield.hpp"
#include "globals.hpp"
#include "a_star.hpp"
#include "rle.hpp"

#include "round.hpp"

#include <cstdlib>
#include <iostream>

PlayField::PlayField()
{
	// Load forest tileset
	LoadTileset("forest.png");
	
	// Try to load the default level
	if (!RLE::DecompressLevel(this, MAPS_PATH "Level1.bin"))
	{
		std::cout << "Failed to open file" << std::endl;

		// Failsafe, reset everything
		Resize(42, 18);
		for (int32_t x = 0; x < gridWidth; x++)
		{
			for (int32_t y = 0; y < gridHeight; y++)
			{
				uint32_t i = y * gridWidth + x;
				mClipdata[i] = CLIPDATA_TYPE_EMPTY;
				mLayer0Tilemap[i] = 0;
				mLayer1Tilemap[i] = 0;
				mLayer2Tilemap[i] = 0;
			}
		}
	}

	// Set gameplay draw flags (every graphical layer)
	SetDrawFlags(PLAYFIELD_DRAW_FLAGS_OPERATION_SET, PLAYFIELD_DRAW_FLAGS_LAYER0 |
		PLAYFIELD_DRAW_FLAGS_LAYER1 | PLAYFIELD_DRAW_FLAGS_LAYER2);
}


PlayField::~PlayField()
{
}

void PlayField::Resize(uint16_t width, uint16_t height)
{
	// Set new sizes
	gridWidth = width;
	gridHeight = height;

	// Resize every tilemap (ISSUE, data in linear in the vectors, causing issues when resizing)
	size_t size = (size_t)(gridWidth * gridHeight);
	mClipdata.resize(size);
	mLayer0Tilemap.resize(size);
	mLayer1Tilemap.resize(size);
	mLayer2Tilemap.resize(size);
}

PlayFieldDrawFlags PlayField::GetDrawFlags()
{
	return mDrawFlags;
}

void PlayField::SetDrawFlags(PlayFieldDrawFlagsOperation operation, PlayFieldDrawFlags flags)
{
	// Bit manipulation
	switch (operation)
	{
		case PLAYFIELD_DRAW_FLAGS_OPERATION_ADD:
			mDrawFlags |= flags;
			break;

		case PLAYFIELD_DRAW_FLAGS_OPERATION_REMOVE:
			mDrawFlags &= static_cast<PlayFieldDrawFlags>(~flags);
			break;

		case PLAYFIELD_DRAW_FLAGS_OPERATION_TOGGLE:
			mDrawFlags ^= flags;
			break;

		case PLAYFIELD_DRAW_FLAGS_OPERATION_SET:
			mDrawFlags = flags;
	}
}

/// <summary>
/// Draws the clipdata as a semi-transparent layer
/// </summary>
void PlayField::DrawClipdata()
{
	for (int32_t y = 0; y < gridHeight; y++)
	{
		for (int32_t x = 0; x < gridWidth; x++)
		{
			// Get colo
			ImU32 color;
			switch (mClipdata[y * gridWidth + x])
			{
			case CLIPDATA_TYPE_EMPTY:
				// WHITE
				color = IM_COL32(0xFF, 0xFF, 0xFF, 0x40);
				break;
			case CLIPDATA_TYPE_NOTHING:
				// BLACK
				color = IM_COL32(0x0, 0x0, 0x0, 0x40);
				break;
			case CLIPDATA_TYPE_ENEMY_ONLY:
				// RED
				color = IM_COL32(0xFF, 0x0, 0x0, 0x40);
				break;
			case CLIPDATA_TYPE_OCCUPIED:
				// BLUE
				color = IM_COL32(0x0, 0x0, 0xFF, 0x40);
				break;
			case CLIPDATA_TYPE_PLAYER_ONLY:
				// GREEN
				color = IM_COL32(0x0, 0xFF, 0x0, 0x40);
				break;
			default:
				// Weird grey
				color = IM_COL32(0x50, 0x50, 0x50, 0x50);
			}

			// Get positions
			ImVec2 posMin(Globals::gGridX + x * GRID_SQUARE_SIZE, Globals::gGridY + y * GRID_SQUARE_SIZE);
			ImVec2 posMax(posMin.x + GRID_SQUARE_SIZE, posMin.y + GRID_SQUARE_SIZE);

			// Draw
			Globals::gDrawList->AddRectFilled(posMin, posMax, color);
		}
	}
}

/// <summary>
/// Draws the graphical layers
/// </summary>
void PlayField::DrawLayers()
{
	// Get texture info
	ImTextureID texId = tileset->id;
	int32_t texWidth = tileset->width;
	int32_t texHeight = tileset->height;

	// Get number of tiles horizontally in the tileset
	uint32_t rowWidth = texWidth / GRID_SQUARE_SIZE;
	// Get number of tiles vertically in the tileset
	uint32_t colHeight = texHeight / GRID_SQUARE_SIZE;

	// Calculate UV size for a single tile
	float_t uvxSize = 1.f / rowWidth;
	float_t uvySize = 1.f / colHeight;

	for (int32_t y = 0; y < gridHeight; y++)
	{
		for (int32_t x = 0; x < gridWidth; x++)
		{
			// Get positions
			ImVec2 posMin(Globals::gGridX + x * GRID_SQUARE_SIZE, Globals::gGridY + y * GRID_SQUARE_SIZE);
			ImVec2 posMax(posMin.x + GRID_SQUARE_SIZE, posMin.y + GRID_SQUARE_SIZE);

			// Declare draw variables
			uint16_t tile; // Tile graphics ID
			uint16_t tileY; // Tile Y in the tileset
			uint16_t tileX; // Tile X in the tileset
			ImVec2 uvMin; // UV min
			ImVec2 uvMax; // UV max

			// Check draw every layer
			if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_LAYER2)
			{
				// Get tile
				tile = mLayer2Tilemap[(size_t)y * gridWidth + x];

				// Get X/Y offsets in the tileset
				tileY = tile / rowWidth;
				tileX = tile % rowWidth;

				// Calculate UV coords
				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);

				Globals::gDrawList->AddImage(texId, posMin, posMax, uvMin, uvMax);
			}

			if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_LAYER1)
			{
				tile = mLayer1Tilemap[(size_t)y * gridWidth + x];

				tileY = tile / rowWidth;
				tileX = tile % rowWidth;

				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);

				Globals::gDrawList->AddImage(texId, posMin, posMax, uvMin, uvMax);
			}

			if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_LAYER0)
			{
				tile = mLayer0Tilemap[(size_t)y * gridWidth + x];

				tileY = tile / rowWidth;
				tileX = tile % rowWidth;

				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);

				Globals::gDrawList->AddImage(texId, posMin, posMax, uvMin, uvMax);
			}
		}
	}
}

/// <summary>
/// Draws the grid lines
/// </summary>
void PlayField::DrawLines()
{
	for (int32_t y = 0; y < gridHeight; y++)
	{
		Globals::gDrawList->AddLine(ImVec2(Globals::gGridX, Globals::gGridY + (y + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE),
			ImVec2(Globals::gGridX + gridWidth * GRID_SQUARE_SIZE, Globals::gGridY + (y + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE), GRID_LINE_COLOR);
		
		for (int32_t x = 0; x < gridWidth; x++)
		{
			Globals::gDrawList->AddLine(ImVec2(Globals::gGridX + (x + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE, Globals::gGridY),
				ImVec2(Globals::gGridX + (x + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE, Globals::gGridY + gridHeight * GRID_SQUARE_SIZE), GRID_LINE_COLOR);
		}
	}
}

/// <summary>
/// Draws the A* path
/// </summary>
void PlayField::DrawPath()
{
	for (int32_t i = 0; i < AStar::recordPositions.size(); i++)
	{
		ImVec2 pos(Globals::gGridX + AStar::recordPositions[i].x,
			Globals::gGridY + AStar::recordPositions[i].y);

		Globals::gDrawList->AddCircleFilled(pos, GRID_SQUARE_SIZE / 2, IM_COL32(0x0, 0x0, 0xFF, 0xA0));
	}
}

void PlayField::Draw()
{
	// First draw layers
	DrawLayers();
	
	// Then clipdata, it's semi-transparent
	if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_CLIPDATA)
		DrawClipdata();

	// Then the grid on top of everything
	if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_GRID_LINES)
		DrawLines();

	if (false)
		DrawPath();

	// Draw the tower bar UI
	towerBarUI.Draw();
}

void PlayField::Save(std::string dst)
{
	// Add maps path
	std::string _dst = MAPS_PATH;
	_dst += dst;
	RLE::CompressLevel(this, _dst.c_str());
}

void PlayField::Load(std::string src)
{
	// Add maps path
	std::string _src = MAPS_PATH;
	_src += src;
	RLE::DecompressLevel(this, _src.c_str());
}

void PlayField::SetClipdataTile(uint8_t x, uint8_t y, ClipdataType type)
{
	// Check in grid
	if (x >= gridWidth || y >= gridHeight)
		return;

	mClipdata[y * gridWidth + x] = type;
}

ClipdataType PlayField::GetClipdataTile(uint8_t x, uint8_t y)
{
	// Check in grid
	if (x >= gridWidth || y >= gridHeight)
		return CLIPDATA_TYPE_NOTHING;

	return mClipdata[y * gridWidth + x];
}


void PlayField::LoadTileset(const char* name)
{
	// Load texture, don't use the ressources class because it hasn't been loaded yet
	tileset = ImGuiUtils::LoadTexture(std::string("assets/tilesets/").append(name).c_str());
}

void PlayField::SetLayertile(uint8_t x, uint8_t y, uint8_t layer, uint16_t value)
{
	// Check in grid
	if (x >= gridWidth || y >= gridHeight)
		return;

	// Write to layer
	switch (layer)
	{
		case 0:
			mLayer0Tilemap[y * gridWidth + x] = value;
			break;

		case 1:
			mLayer1Tilemap[y * gridWidth + x] = value;
			break;

		case 2:
			mLayer2Tilemap[y * gridWidth + x] = value;
	}
}

uint16_t PlayField::GetLayerTile(uint8_t x, uint8_t y, uint8_t layer)
{
	// Check in grid
	if (x >= gridWidth || y >= gridHeight)
		return 0;

	// Get from layer
	switch (layer)
	{
		case 0:
			return mLayer0Tilemap[y * gridWidth + x];

		case 1:
			return mLayer1Tilemap[y * gridWidth + x];

		case 2:
			return mLayer2Tilemap[y * gridWidth + x];

		default:
			return 0;
	}
}

ClipdataType* PlayField::GetClipdataPointer()
{
	return mClipdata.data();
}

uint16_t* PlayField::GetTilemapPointer(uint8_t layer)
{
	// Get from layer
	switch (layer)
	{
		case 0:
			return mLayer0Tilemap.data();

		case 1:
			return mLayer1Tilemap.data();

		case 2:
			return mLayer2Tilemap.data();
	}

	return nullptr;
}

std::vector<PathNode>& PlayField::GetPathNodes()
{
	return mPathNodes;
}

PathNode PlayField::GetNextPathNode(uint8_t x, uint8_t y, PathNodeDir direction)
{
	// Setup temporary node
	PathNode node(x, y, PathNodeDir::DIR_LEFT);

	// Janky code, check for a path node in every tile in the direction, however the order of the path node vector matters more than the position
	// e.g. with a setup like this :
	// >   	 v	   >
	// If the node on the right was placed first, then the GetNextPathNode of the left node will use it instead of the middle
	// Too lazy to fix, it's not a problem if the path is perfectly drawn in the editor anyway
	switch (direction)
	{
		case PathNodeDir::DIR_RIGHT:
			for (int32_t tstX = x + 1; tstX < gridWidth; tstX++)
			{
				node.x = tstX;

				std::vector<PathNode>::iterator _f = std::find(mPathNodes.begin(), mPathNodes.end(), node);
				if (_f != mPathNodes.end())
					return *_f;
			}
			break;

		case PathNodeDir::DIR_LEFT:
			for (int32_t tstX = x - 1; tstX >= 0; tstX--)
			{
				node.x = tstX;

				std::vector<PathNode>::iterator _f = std::find(mPathNodes.begin(), mPathNodes.end(), node);
				if (_f != mPathNodes.end())
					return *_f;
			}
			break;

		case PathNodeDir::DIR_UP:
			for (int32_t tstY = y - 1; tstY >= 0; tstY--)
			{
				node.y = tstY;

				std::vector<PathNode>::iterator _f = std::find(mPathNodes.begin(), mPathNodes.end(), node);
				if (_f != mPathNodes.end())
					return *_f;
			}
			break;

		case PathNodeDir::DIR_DOWN:
			for (int32_t tstY = y + 1; tstY < gridHeight; tstY++)
			{
				node.y = tstY;

				std::vector<PathNode>::iterator _f = std::find(mPathNodes.begin(), mPathNodes.end(), node);
				if (_f != mPathNodes.end())
					return *_f;
			}
			break;
	}

	// Return invalid, no node found
	return PathNode(UCHAR_MAX, UCHAR_MAX, PathNodeDir::DIR_LEFT);
}

void PlayField::GetGridPositionFromPixels(int32_t pixelX, int32_t pixelY, uint8_t& tileX, uint8_t& tileY)
{
	// Convert via euclidian division
	uint8_t x = pixelX / GRID_SQUARE_SIZE;
	uint8_t y = pixelY / GRID_SQUARE_SIZE;

	// Check in range
	if (x >= gridWidth)
		tileX = UCHAR_MAX;
	else
		tileX = x;

	if (y >= gridHeight)
		tileY = UCHAR_MAX;
	else
		tileY = y;
}

void PlayField::GetPixelPositionFromGrid(uint8_t tileX, uint8_t tileY, int32_t& pixelX, int32_t& pixelY)
{
	// Convert by multiplying
	pixelX = tileX * GRID_SQUARE_SIZE;
	pixelY = tileY * GRID_SQUARE_SIZE;
}
