#include "PlayField.h"
#include "Globals.h"
#include "AStar.h"
#include "RLE.h"

#include <cstdlib>
#include <iostream>

PlayField::PlayField()
	: maxR(true), r(0)
{
	AStar::bindField(this);
	/*FILE* f;
	fopen_s(&f, "TestField.bin", "rb");
	if (!f)
	{
		std::cout << "Failed to open file" << std::endl;
		for (int32_t x = 0; x < GRID_WIDTH; x++)
			for (int32_t y = 0; y < GRID_HEIGHT; y++)
				m_grid[y][x] = SQUARE_TYPE_EMPTY; // static_cast<SquareType>(std::rand() % 5);
	}
	else
	{
		fread_s(m_grid, sizeof(m_grid), sizeof(SquareType), GRID_HEIGHT * GRID_WIDTH, f);

		fclose(f);
	}*/
	
	if (!RLE::decompress(m_grid, MAPS_PATH "Default.bin"))
	{
		std::cout << "Failed to open file" << std::endl;
		for (int32_t x = 0; x < GRID_WIDTH; x++)
			for (int32_t y = 0; y < GRID_HEIGHT; y++)
				m_grid[y][x] = SQUARE_TYPE_EMPTY; // static_cast<SquareType>(std::rand() % 5);
	}
}


PlayField::~PlayField()
{
}

void PlayField::draw()
{
	for (int32_t y = 0; y < GRID_HEIGHT; y++)
	{
		for (int32_t x = 0; x < GRID_WIDTH; x++)
		{
			ImU32 color;
			switch (m_grid[y][x])
			{
				case SQUARE_TYPE_EMPTY:
					// WHITE
					color = IM_COL32(0xFF, 0xFF, 0xFF, 0xFF);
					break;
				case SQUARE_TYPE_NOTHING:
					// BLACK
					color = IM_COL32(0x0, 0x0, 0x0, 0xFF);
					break;
				case SQUARE_TYPE_ENEMY_ONLY:
					// RED
					color = IM_COL32(0xFF, 0x0, 0x0, 0xFF);
					break;
				case SQUARE_TYPE_OCCUPIED:
					// GRAY
					color = IM_COL32(0xC0, 0xC0, 0xC0, 0xFF);
					break;
				case SQUARE_TYPE_PLAYER_ONLY:
					// GREEN
					color = IM_COL32(0x0, 0xFF, 0x0, 0xFF);
					break;
				default:
					// Weird grey
					color = IM_COL32(0x50, 0x50, 0x50, 0x50);
			}

			ImVec2 pMin((float_t)Globals::gWindowX + x * GRID_SQUARE_SIZE, (float_t)Globals::gWindowY + y * GRID_SQUARE_SIZE);
			ImVec2 pMax(Globals::gWindowX + (x + 1.f) * GRID_SQUARE_SIZE, Globals::gWindowY + (y + 1.f) * GRID_SQUARE_SIZE);

			Globals::gDrawList->AddRectFilled(pMin, pMax, color);
		}

		Globals::gDrawList->AddLine(ImVec2((float_t)Globals::gWindowX, Globals::gWindowY + (y + 1) * GRID_SQUARE_SIZE - 1.f),
			ImVec2((float_t)Globals::gWindowX + GRID_WIDTH * GRID_SQUARE_SIZE, Globals::gWindowY + (y + 1) * GRID_SQUARE_SIZE - 1.f), GRID_LINE_COLOR);
	}

	for (int32_t x = 0; x < GRID_WIDTH; x++)
		Globals::gDrawList->AddLine(ImVec2(Globals::gWindowX + (x + 1) * GRID_SQUARE_SIZE - 1.f, (float_t)Globals::gWindowY),
			ImVec2(Globals::gWindowX + (x + 1) * GRID_SQUARE_SIZE - 1.f, (float_t)Globals::gWindowY + GRID_HEIGHT * GRID_SQUARE_SIZE), GRID_LINE_COLOR);

	towerBarUI.draw();


	if (ImGui::Begin("A*"))
	{
		if (ImGui::Button("Test"))
		{
			std::cout << AStar::findBestPath(GRID_WIDTH - 1, GRID_HEIGHT - 1, 0, 0) << std::endl;
		}

		if (AStar::recordPositions.size() != 0)
		{
			if (maxR)
				r = AStar::recordPositions.size();
			ImGui::SliderInt("Record", (int32_t*)&r, 0, (int32_t)AStar::recordPositions.size());
			maxR = (r == AStar::recordPositions.size());

			for (int32_t i = 0; i < std::min((int32_t)AStar::recordPositions.size(), (int32_t)r); i++)
			{
				ImVec2 pos(Globals::gWindowX + AStar::recordPositions[i].x,
					Globals::gWindowY + AStar::recordPositions[i].y);

				Globals::gDrawList->AddCircleFilled(pos, GRID_SQUARE_SIZE / 2, IM_COL32(0x0, 0x0, 0xFF, 0xA0));
			}
		}
	}
	ImGui::End();
}

void PlayField::save(std::string dst)
{
	std::string _dst = MAPS_PATH;
	_dst += dst;
	RLE::compress(m_grid, sizeof(m_grid), _dst.c_str());
}

void PlayField::load(std::string src)
{
	std::string _src = MAPS_PATH;
	_src += src;
	RLE::decompress(m_grid, _src.c_str());
}

void PlayField::setGridTile(uint8_t x, uint8_t y, SquareType type)
{
	if (x >= GRID_WIDTH || y >= GRID_HEIGHT)
		return;

	m_grid[y][x] = type;
}

SquareType PlayField::getGridTile(uint8_t x, uint8_t y)
{
	if (x >= GRID_WIDTH || y >= GRID_HEIGHT)
		return SQUARE_TYPE_NOTHING;

	return m_grid[y][x];
}

void PlayField::getGridPositionFromCoords(int32_t screenX, int32_t screenY, uint8_t& tileX, uint8_t& tileY)
{
	uint8_t x = (screenX - Globals::gWindowX) / GRID_SQUARE_SIZE;
	uint8_t y = (screenY - Globals::gWindowY) / GRID_SQUARE_SIZE;

	if (x >= GRID_WIDTH)
		tileX = UCHAR_MAX;
	else
		tileX = x;

	if (y >= GRID_HEIGHT)
		tileY = UCHAR_MAX;
	else
		tileY = y;
}
