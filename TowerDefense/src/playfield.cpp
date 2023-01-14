#include "playfield.hpp"
#include "globals.hpp"
#include "a_star.hpp"
#include "rle.hpp"

#include "round.hpp"

#include <cstdlib>
#include <iostream>

PlayField::PlayField()
	: maxR(true), r(0)
{
	Resize(42, 18);

	LoadTileset("forest.png");
	
	if (!RLE::DecompressLevel(this, MAPS_PATH "Level1.bin"))
	{
		std::cout << "Failed to open file" << std::endl;
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

	SetDrawFlags(PLAYFIELD_DRAW_FLAGS_OPERATION_SET, PLAYFIELD_DRAW_FLAGS_LAYER0 |
		PLAYFIELD_DRAW_FLAGS_LAYER1 | PLAYFIELD_DRAW_FLAGS_LAYER2);
}


PlayField::~PlayField()
{
}

void PlayField::DrawClipdata()
{
	for (int32_t y = 0; y < gridHeight; y++)
	{
		for (int32_t x = 0; x < gridWidth; x++)
		{
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
					// GRAY
					color = IM_COL32(0xC0, 0xC0, 0xC0, 0x40);
					break;
				case CLIPDATA_TYPE_PLAYER_ONLY:
					// GREEN
					color = IM_COL32(0x0, 0xFF, 0x0, 0x40);
					break;
				default:
					// Weird grey
					color = IM_COL32(0x50, 0x50, 0x50, 0x50);
			}

			ImVec2 pMin((float_t)Globals::gGridX + x * GRID_SQUARE_SIZE, (float_t)Globals::gGridY + y * GRID_SQUARE_SIZE);
			ImVec2 pMax(Globals::gGridX + (x + GRID_SQUARE_LINE_SIZE) * GRID_SQUARE_SIZE, Globals::gGridY + (y + GRID_SQUARE_LINE_SIZE) * GRID_SQUARE_SIZE);

			Globals::gDrawList->AddRectFilled(pMin, pMax, color);
		}
	}
}

void PlayField::DrawLayers()
{
	ImTextureID texId = tileset->id;
	int32_t texWidth = tileset->width;
	int32_t texHeight = tileset->height;

	uint32_t rowWidth = texWidth / GRID_SQUARE_SIZE;
	uint32_t colHeight = texHeight / GRID_SQUARE_SIZE;

	float_t uvxSize = 1.f / rowWidth;
	float_t uvySize = 1.f / colHeight;

	for (int32_t y = 0; y < gridHeight; y++)
	{
		for (int32_t x = 0; x < gridWidth; x++)
		{
			ImVec2 pMin((float_t)Globals::gGridX + x * GRID_SQUARE_SIZE, (float_t)Globals::gGridY + y * GRID_SQUARE_SIZE);
			ImVec2 pMax(Globals::gGridX + (x + 1.f) * GRID_SQUARE_SIZE, Globals::gGridY + (y + 1.f) * GRID_SQUARE_SIZE);
			uint16_t tile;
			uint16_t tileY;
			uint16_t tileX;
			ImVec2 uvMin;
			ImVec2 uvMax;

			if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_LAYER2)
			{
				tile = mLayer2Tilemap[y * gridWidth + x];

				tileY = tile / rowWidth;
				tileX = tile % rowWidth;

				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);

				Globals::gDrawList->AddImage(texId, pMin, pMax, uvMin, uvMax);
			}

			if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_LAYER1)
			{
				tile = mLayer1Tilemap[y * gridWidth + x];

				tileY = tile / rowWidth;
				tileX = tile % rowWidth;

				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);

				Globals::gDrawList->AddImage(texId, pMin, pMax, uvMin, uvMax);
			}

			if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_LAYER0)
			{
				tile = mLayer0Tilemap[y * gridWidth + x];

				tileY = tile / rowWidth;
				tileX = tile % rowWidth;

				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);

				Globals::gDrawList->AddImage(texId, pMin, pMax, uvMin, uvMax);
			}
		}
	}
}

void PlayField::Resize(uint16_t width, uint16_t height)
{
	gridWidth = width;
	gridHeight = height;

	mClipdata.resize(gridWidth * gridHeight);
	mLayer0Tilemap.resize(gridWidth * gridHeight);
	mLayer1Tilemap.resize(gridWidth * gridHeight);
	mLayer2Tilemap.resize(gridWidth * gridHeight);
}

PlayFieldDrawFlags PlayField::GetDrawFlags()
{
	return mDrawFlags;
}

void PlayField::SetDrawFlags(PlayFieldDrawFlagsOperation operation, PlayFieldDrawFlags flags)
{
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

void PlayField::DrawLines()
{
	for (int32_t y = 0; y < gridHeight; y++)
	{
		Globals::gDrawList->AddLine(ImVec2((float_t)Globals::gGridX, Globals::gGridY + (y + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE),
			ImVec2((float_t)Globals::gGridX + gridWidth * GRID_SQUARE_SIZE, Globals::gGridY + (y + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE), GRID_LINE_COLOR);
		
		for (int32_t x = 0; x < gridWidth; x++)
		{
			Globals::gDrawList->AddLine(ImVec2(Globals::gGridX + (x + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE, (float_t)Globals::gGridY),
				ImVec2(Globals::gGridX + (x + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE, (float_t)Globals::gGridY + gridHeight * GRID_SQUARE_SIZE), GRID_LINE_COLOR);
		}
	}
}

void PlayField::DrawPath()
{
	if (ImGui::Begin("A*"))
	{
		if (ImGui::Button("Test"))
		{
			std::cout << AStar::FindBestPath(gridWidth - 1, gridHeight / 2, 0, gridHeight / 2) << std::endl;
		}

		if (AStar::recordPositions.size() != 0)
		{
			if (maxR)
				r = AStar::recordPositions.size();
			ImGui::SliderInt("Record", (int32_t*)&r, 0, (int32_t)AStar::recordPositions.size());
			maxR = (r == AStar::recordPositions.size());

			for (int32_t i = 0; i < std::min((int32_t)AStar::recordPositions.size(), (int32_t)r); i++)
			{
				ImVec2 pos(Globals::gGridX + AStar::recordPositions[i].x,
					Globals::gGridY + AStar::recordPositions[i].y);

				Globals::gDrawList->AddCircleFilled(pos, GRID_SQUARE_SIZE / 2, IM_COL32(0x0, 0x0, 0xFF, 0xA0));
			}
		}
	}
	ImGui::End();
}

void PlayField::Draw()
{
	DrawLayers();
	
	if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_CLIPDATA)
		DrawClipdata();

	if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_GRID_LINES)
		DrawLines();

	if (false)
		DrawPath();

	towerBarUI.Draw();
}

void PlayField::Save(std::string dst)
{
	std::string _dst = MAPS_PATH;
	_dst += dst;
	RLE::CompressLevel(this, _dst.c_str());
}

void PlayField::Load(std::string src)
{
	std::string _src = MAPS_PATH;
	_src += src;
	RLE::DecompressLevel(this, _src.c_str());
}

void PlayField::SetClipdataTile(uint8_t x, uint8_t y, ClipdataType type)
{
	if (x >= gridWidth || y >= gridHeight)
		return;

	mClipdata[y * gridWidth + x] = type;
}

ClipdataType PlayField::GetClipdataTile(uint8_t x, uint8_t y)
{
	if (x >= gridWidth || y >= gridHeight)
		return CLIPDATA_TYPE_NOTHING;

	return mClipdata[y * gridWidth + x];
}


void PlayField::LoadTileset(const char* name)
{
	tileset = ImGuiUtils::LoadTexture(std::string("assets/tilesets/").append(name).c_str());
}

void PlayField::SetLayertile(uint8_t x, uint8_t y, uint8_t layer, uint16_t value)
{
	if (x >= gridWidth || y >= gridHeight)
		return;

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
	if (x >= gridWidth || y >= gridHeight)
		return 0;

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
	uint16_t* pLayers[] = {
		mLayer0Tilemap.data(),
		mLayer1Tilemap.data(),
		mLayer2Tilemap.data(),
	};

	return pLayers[layer];
}

std::vector<PathNode>& PlayField::GetPathNodes()
{
	return mPathNodes;
}

void PlayField::GetGridPositionFromPixels(int32_t pixelX, int32_t pixelY, uint8_t& tileX, uint8_t& tileY)
{
	uint8_t x = pixelX / GRID_SQUARE_SIZE;
	uint8_t y = pixelY / GRID_SQUARE_SIZE;

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
	pixelX = tileX * GRID_SQUARE_SIZE;
	pixelY = tileY * GRID_SQUARE_SIZE;
}
