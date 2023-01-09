#include "PlayField.h"
#include "Globals.h"
#include "AStar.h"
#include "RLE.h"

#include <cstdlib>
#include <iostream>

PlayField::PlayField()
	: maxR(true), r(0)
{
	resize(42, 18);

	loadTileset("forest.png");
	
	if (!RLE::decompressLevel(this, MAPS_PATH "Default.bin"))
	{
		std::cout << "Failed to open file" << std::endl;
		for (int32_t x = 0; x < m_gridWidth; x++)
			for (int32_t y = 0; y < m_gridHeight; y++)
				m_clipdata[y * m_gridWidth + x] = CLIPDATA_TYPE_EMPTY; // static_cast<ClipdataType>(std::rand() % 5);
	}
}


PlayField::~PlayField()
{
}

void PlayField::drawClipdata()
{
	for (int32_t y = 0; y < m_gridHeight; y++)
	{
		for (int32_t x = 0; x < m_gridWidth; x++)
		{
			ImU32 color;
			switch (m_clipdata[y * m_gridWidth + x])
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

void PlayField::drawLayers()
{
	ImTextureID texId = m_tileset.id;
	int32_t texWidth = m_tileset.width;
	int32_t texHeight = m_tileset.height;

	uint32_t rowWidth = texWidth / GRID_SQUARE_SIZE;
	uint32_t colHeight = texHeight / GRID_SQUARE_SIZE;

	float_t uvxSize = 1.f / rowWidth;
	float_t uvySize = 1.f / colHeight;

	for (int32_t y = 0; y < m_gridHeight; y++)
	{
		for (int32_t x = 0; x < m_gridWidth; x++)
		{
			ImVec2 pMin;
			ImVec2 pMax;
			uint16_t tile;
			uint16_t tileY;
			uint16_t tileX;
			ImVec2 uvMin;
			ImVec2 uvMax;

			if (m_drawFlags & PLAYFIELD_DRAW_FLAGS_LAYER0)
			{
				pMin = ImVec2((float_t)Globals::gGridX + x * GRID_SQUARE_SIZE, (float_t)Globals::gGridY + y * GRID_SQUARE_SIZE);
				pMax = ImVec2(Globals::gGridX + (x + 1.f) * GRID_SQUARE_SIZE, Globals::gGridY + (y + 1.f) * GRID_SQUARE_SIZE);

				tile = m_layer0Tilemap[y * m_gridWidth + x];

				tileY = tile / rowWidth;
				tileX = tile % rowWidth;

				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);

				Globals::gDrawList->AddImage(texId, pMin, pMax, uvMin, uvMax);
			}

			if (m_drawFlags & PLAYFIELD_DRAW_FLAGS_LAYER1)
			{
				tile = m_layer1Tilemap[y * m_gridWidth + x];
				tileY = tile / texWidth;
				tileX = tile % texWidth;

				uvMin = ImVec2(uvxSize * tileX, uvySize * tileY);
				uvMax = ImVec2(uvMin.x + uvxSize, uvMin.y + uvySize);
				
				Globals::gDrawList->AddImage(texId, pMin, pMax, uvMin, uvMax);
			}
		}
	}
}

void PlayField::resize(uint16_t width, uint16_t height)
{
	m_gridWidth = width;
	m_gridHeight = height;

	m_clipdata.resize(m_gridWidth * m_gridHeight);
	m_layer0Tilemap.resize(m_gridWidth * m_gridHeight);
	m_layer1Tilemap.resize(m_gridWidth * m_gridHeight);
}

void PlayField::setDrawFlags(PlayFieldDrawFlagsOperation operation, PlayFieldDrawFlags flags)
{
	switch (operation)
	{
		case PLAYFIELD_DRAW_FLAGS_OPERATION_ADD:
			m_drawFlags |= flags;
			break;

		case PLAYFIELD_DRAW_FLAGS_OPERATION_REMOVE:
			m_drawFlags &= ~flags;
			break;

		case PLAYFIELD_DRAW_FLAGS_OPERATION_TOGGLE:
			m_drawFlags ^= flags;
			break;

		case PLAYFIELD_DRAW_FLAGS_OPERATION_SET:
			m_drawFlags = flags;
	}
}

void PlayField::drawLines()
{
	for (int32_t y = 0; y < m_gridHeight; y++)
	{
		Globals::gDrawList->AddLine(ImVec2((float_t)Globals::gGridX, Globals::gGridY + (y + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE),
			ImVec2((float_t)Globals::gGridX + m_gridWidth * GRID_SQUARE_SIZE, Globals::gGridY + (y + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE), GRID_LINE_COLOR);
		
		for (int32_t x = 0; x < m_gridWidth; x++)
		{
			Globals::gDrawList->AddLine(ImVec2(Globals::gGridX + (x + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE, (float_t)Globals::gGridY),
				ImVec2(Globals::gGridX + (x + 1) * GRID_SQUARE_SIZE - GRID_SQUARE_LINE_SIZE, (float_t)Globals::gGridY + m_gridHeight * GRID_SQUARE_SIZE), GRID_LINE_COLOR);
		}
	}
}

void PlayField::draw()
{
	drawLayers();
	
	if (m_drawFlags & PLAYFIELD_DRAW_FLAGS_CLIPDATA)
		drawClipdata();

	if (m_drawFlags & PLAYFIELD_DRAW_FLAGS_GRID_LINES)
		drawLines();

	towerBarUI.draw();


	if (ImGui::Begin("A*"))
	{
		if (ImGui::Button("Test"))
		{
			std::cout << AStar::findBestPath(m_gridWidth - 1, m_gridHeight - 1, 0, 0) << std::endl;
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

void PlayField::save(std::string dst)
{
	std::string _dst = MAPS_PATH;
	_dst += dst;
	RLE::compressLevel(this, _dst.c_str());
}

void PlayField::load(std::string src)
{
	std::string _src = MAPS_PATH;
	_src += src;
	RLE::decompressLevel(this, _src.c_str());
}

void PlayField::setClipdataTile(uint8_t x, uint8_t y, ClipdataType type)
{
	if (x >= m_gridWidth || y >= m_gridHeight)
		return;

	m_clipdata[y * m_gridWidth + x] = type;
}

ClipdataType PlayField::getClipdataTile(uint8_t x, uint8_t y)
{
	if (x >= m_gridWidth || y >= m_gridHeight)
		return CLIPDATA_TYPE_NOTHING;

	return m_clipdata[y * m_gridWidth + x];
}


void PlayField::loadTileset(const char* name)
{
	m_tileset = ImGuiUtils::LoadTexture(std::string("assets/tilesets/").append(name).c_str());
}

void PlayField::setLayertile(uint8_t x, uint8_t y, uint8_t layer, uint16_t value)
{
	if (x >= m_gridWidth || y >= m_gridHeight)
		return;

	switch (layer)
	{
		case 0:
			m_layer0Tilemap[y * m_gridWidth + x] = value;
			break;

		case 1:
			m_layer1Tilemap[y * m_gridWidth + x] = value;
	}
}

ClipdataType* PlayField::getClipdataPointer()
{
	return m_clipdata.data();
}

uint16_t* PlayField::getTilemapPointer(uint8_t layer)
{
	uint16_t* pLayers[] = {
		m_layer0Tilemap.data(),
		m_layer1Tilemap.data(),
	};

	return pLayers[layer];
}

void PlayField::getGridPositionFromPixels(int32_t mouseX, int32_t mouseY, uint8_t& tileX, uint8_t& tileY)
{
	uint8_t x = (mouseX - Globals::gGridX) / GRID_SQUARE_SIZE;
	uint8_t y = (mouseY - Globals::gGridY) / GRID_SQUARE_SIZE;

	if (x >= m_gridWidth)
		tileX = UCHAR_MAX;
	else
		tileX = x;

	if (y >= m_gridHeight)
		tileY = UCHAR_MAX;
	else
		tileY = y;
}

void PlayField::getPixelPositionFromGrid(uint8_t tileX, uint8_t tileY, int32_t& screenX, int32_t& screenY)
{
	screenX = tileX * GRID_SQUARE_SIZE;
	screenY = tileY * GRID_SQUARE_SIZE;
}
