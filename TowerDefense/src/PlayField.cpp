#include "PlayField.h"
#include "Globals.h"
#include "AStar.h"
#include "RLE.h"

#include "Round.h"
#include "RoundsData.h"

#include <cstdlib>
#include <iostream>

PlayField::PlayField()
	: maxR(true), r(0)
{
	Resize(42, 18);

	LoadTileset("forest.png");
	Round::StartRound(sLevel1_Wave1);
	
	if (!RLE::DecompressLevel(this, MAPS_PATH "Level1.bin"))
	{
		std::cout << "Failed to open file" << std::endl;
		for (int32_t x = 0; x < mGridWidth; x++)
			for (int32_t y = 0; y < mGridHeight; y++)
				mClipdata[y * mGridWidth + x] = CLIPDATA_TYPE_EMPTY; // static_cast<ClipdataType>(std::rand() % 5);
	}

	SetDrawFlags(PLAYFIELD_DRAW_FLAGS_OPERATION_SET, PLAYFIELD_DRAW_FLAGS_GRID_LINES | PLAYFIELD_DRAW_FLAGS_LAYER0 |
		PLAYFIELD_DRAW_FLAGS_LAYER1 | PLAYFIELD_DRAW_FLAGS_LAYER2);
}


PlayField::~PlayField()
{
}

void PlayField::DrawClipdata()
{
	for (int32_t y = 0; y < mGridHeight; y++)
	{
		for (int32_t x = 0; x < mGridWidth; x++)
		{
			ImU32 color;
			switch (mClipdata[y * mGridWidth + x])
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
	ImTextureID texId = mTileset.id;
	int32_t texWidth = mTileset.width;
	int32_t texHeight = mTileset.height;

	uint32_t rowWidth = texWidth / GRID_SQUARE_SIZE;
	uint32_t colHeight = texHeight / GRID_SQUARE_SIZE;

	float_t uvxSize = 1.f / rowWidth;
	float_t uvySize = 1.f / colHeight;

	for (int32_t y = 0; y < mGridHeight; y++)
	{
		for (int32_t x = 0; x < mGridWidth; x++)
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
				tile = mLayer2Tilemap[y * mGridWidth + x];

				tileY = tile / rowWidth;
				tileX = tile % rowWidth;

				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);

				Globals::gDrawList->AddImage(texId, pMin, pMax, uvMin, uvMax);
			}

			if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_LAYER1)
			{
				tile = mLayer1Tilemap[y * mGridWidth + x];

				tileY = tile / rowWidth;
				tileX = tile % rowWidth;

				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);

				Globals::gDrawList->AddImage(texId, pMin, pMax, uvMin, uvMax);
			}

			if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_LAYER0)
			{
				tile = mLayer0Tilemap[y * mGridWidth + x];

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
	mGridWidth = width;
	mGridHeight = height;

	mClipdata.resize(mGridWidth * mGridHeight);
	mLayer0Tilemap.resize(mGridWidth * mGridHeight);
	mLayer1Tilemap.resize(mGridWidth * mGridHeight);
	mLayer2Tilemap.resize(mGridWidth * mGridHeight);
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
	for (int32_t y = 0; y < mGridHeight; y++)
	{
		Globals::gDrawList->AddLine(ImVec2((float_t)Globals::gGridX, Globals::gGridY + (y + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE),
			ImVec2((float_t)Globals::gGridX + mGridWidth * GRID_SQUARE_SIZE, Globals::gGridY + (y + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE), GRID_LINE_COLOR);
		
		for (int32_t x = 0; x < mGridWidth; x++)
		{
			Globals::gDrawList->AddLine(ImVec2(Globals::gGridX + (x + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE, (float_t)Globals::gGridY),
				ImVec2(Globals::gGridX + (x + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE, (float_t)Globals::gGridY + mGridHeight * GRID_SQUARE_SIZE), GRID_LINE_COLOR);
		}
	}
}

void PlayField::Draw()
{
	DrawLayers();
	
	if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_CLIPDATA)
		DrawClipdata();

	if (mDrawFlags & PLAYFIELD_DRAW_FLAGS_GRID_LINES)
		DrawLines();

	towerBarUI.Draw();


	if (ImGui::Begin("A*"))
	{
		if (ImGui::Button("Test"))
		{
			std::cout << AStar::FindBestPath(mGridWidth - 1, mGridHeight / 2, 0, mGridHeight / 2) << std::endl;
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
	if (x >= mGridWidth || y >= mGridHeight)
		return;

	mClipdata[y * mGridWidth + x] = type;
}

ClipdataType PlayField::GetClipdataTile(uint8_t x, uint8_t y)
{
	if (x >= mGridWidth || y >= mGridHeight)
		return CLIPDATA_TYPE_NOTHING;

	return mClipdata[y * mGridWidth + x];
}


void PlayField::LoadTileset(const char* name)
{
	mTileset = ImGuiUtils::LoadTexture(std::string("assets/tilesets/").append(name).c_str());
}

void PlayField::SetLayertile(uint8_t x, uint8_t y, uint8_t layer, uint16_t value)
{
	if (x >= mGridWidth || y >= mGridHeight)
		return;

	switch (layer)
	{
		case 0:
			mLayer0Tilemap[y * mGridWidth + x] = value;
			break;

		case 1:
			mLayer1Tilemap[y * mGridWidth + x] = value;
			break;

		case 2:
			mLayer2Tilemap[y * mGridWidth + x] = value;
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

void PlayField::GetGridPositionFromPixels(int32_t pixelX, int32_t pixelY, uint8_t& tileX, uint8_t& tileY)
{
	uint8_t x = (pixelX - Globals::gGridX) / GRID_SQUARE_SIZE;
	uint8_t y = (pixelY - Globals::gGridY) / GRID_SQUARE_SIZE;

	if (x >= mGridWidth)
		tileX = UCHAR_MAX;
	else
		tileX = x;

	if (y >= mGridHeight)
		tileY = UCHAR_MAX;
	else
		tileY = y;
}

void PlayField::GetPixelPositionFromGrid(uint8_t tileX, uint8_t tileY, int32_t& pixelX, int32_t& pixelY)
{
	pixelX = tileX * GRID_SQUARE_SIZE + Globals::gGridX;
	pixelY = tileY * GRID_SQUARE_SIZE + Globals::gGridY;
}
