#include "LevelEditor.h"

#include <imgui.h>

#include "Globals.h"
#include "imgui_utils.hpp"
#include "AStar.h"

int32_t LevelEditor::m_currentBlockType;
bool LevelEditor::m_dragEnabled;
bool LevelEditor::m_canPlaceTile;
bool LevelEditor::m_updateAStar;
char LevelEditor::m_fileName[20];

bool LevelEditor::m_selectionActive;
uint8_t LevelEditor::m_selectionStartX;
uint8_t LevelEditor::m_selectionStartY;
uint8_t LevelEditor::m_selectionEndX;
uint8_t LevelEditor::m_selectionEndY;
uint8_t LevelEditor::m_selectionWidth;
uint8_t LevelEditor::m_selectionHeight;
uint8_t LevelEditor::m_selectionCopyWidth;
uint8_t LevelEditor::m_selectionCopyHeight;
std::vector<SquareType> LevelEditor::m_selectionCopyData;

PlayField* LevelEditor::m_playField;

static const char* const sBlockTypes[] = {
	"Empty",
	"Nothing",
	"Enemy only",
	"Occupied",
	"Player only",
};

void LevelEditor::bindPlayField(PlayField* field)
{
	m_playField = field;
}

void LevelEditor::loadTileset()
{
	ImGuiUtils::LoadTexture("assets/tilesets/forest.png");
}

void LevelEditor::update()
{
	if (ImGui::Begin("Level editor"))
	{
		LevelEditor::handleMisc();

		LevelEditor::handleClear();

		LevelEditor::verticalSpace();
		LevelEditor::handleFile();

		LevelEditor::verticalSpace();
		LevelEditor::handleCursor();

		LevelEditor::handleSelection();
		LevelEditor::handleHotkeys();
	}

	ImGui::End();
}

void LevelEditor::verticalSpace()
{
	ImGui::Dummy(ImVec2(0, 10.f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10.f));
}

void LevelEditor::handleMisc()
{
	ImGui::Text("Block type");
	ImGui::SameLine();
	ImGui::Combo("##", &LevelEditor::m_currentBlockType, sBlockTypes, IM_ARRAYSIZE(sBlockTypes));

	ImGui::Checkbox("Enable dragging", &LevelEditor::m_dragEnabled);
	ImGui::Checkbox("Update A* live", &LevelEditor::m_updateAStar);
}

void LevelEditor::handleClear()
{
	if (ImGui::Button("Clear"))
		ImGui::OpenPopup("Delete?");

	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to clear everything?");

		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			for (int32_t x = 0; x < GRID_WIDTH; x++)
				for (int32_t y = 0; y < GRID_HEIGHT; y++)
					m_playField->setGridTile(x, y, SQUARE_TYPE_EMPTY);

			AStar::findBestPath(0, 0, GRID_WIDTH - 1, GRID_HEIGHT - 1);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("No", ImVec2(120, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}


void LevelEditor::handleFile()
{
	ImGui::Text("File name");
	ImGui::SameLine();
	ImGui::PushItemWidth(-1);
	ImGui::InputText("FileNameInput", LevelEditor::m_fileName, sizeof(LevelEditor::m_fileName));
	ImGui::PopItemWidth();

	if (ImGui::Button("Save"))
		m_playField->save(LevelEditor::m_fileName);

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		m_playField->load(LevelEditor::m_fileName);

		AStar::findBestPath(0, 0, GRID_WIDTH - 1, GRID_HEIGHT - 1);
	}
}

void LevelEditor::handleCursor()
{
	ImVec2 mouse = Globals::gIO->MousePos;
	uint8_t tileX;
	uint8_t tileY;

	m_playField->getGridPositionFromCoords((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);
	if (tileX != UCHAR_MAX && tileY != UCHAR_MAX)
	{
		ImVec2 pMin((float_t)Globals::gWindowX + tileX * GRID_SQUARE_SIZE, (float_t)Globals::gWindowY + tileY * GRID_SQUARE_SIZE);
		ImVec2 pMax((float_t)Globals::gWindowX + (tileX + 1) * GRID_SQUARE_SIZE, (float_t)Globals::gWindowY + (tileY + 1) * GRID_SQUARE_SIZE);

		Globals::gDrawList->AddRectFilled(pMin, pMax, IM_COL32(0xFF, 0, 0, 0x50));
		ImGui::Text("Cursor position : (%d ; %d)", tileX, tileY);

		if (!LevelEditor::m_canPlaceTile)
			return;

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
			(LevelEditor::m_dragEnabled && ImGui::IsMouseDown(ImGuiMouseButton_Left)))
		{
			m_playField->setGridTile(tileX, tileY, static_cast<SquareType>(LevelEditor::m_currentBlockType));
			if (LevelEditor::m_updateAStar)
				AStar::findBestPath(0, 0, GRID_WIDTH - 1, GRID_HEIGHT - 1);
		}
	}
}

void LevelEditor::handleSelection()
{
	ImVec2 mouse = Globals::gIO->MousePos;
	// Update selection positions
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		m_playField->getGridPositionFromCoords((int32_t)mouse.x, (int32_t)mouse.y, m_selectionStartX, m_selectionStartY);
		if (m_selectionStartX == UCHAR_MAX || m_selectionStartY == UCHAR_MAX)
			return;
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		uint8_t prevEndX = m_selectionEndX;
		uint8_t prevEndY = m_selectionEndY;

		m_playField->getGridPositionFromCoords((int32_t)mouse.x, (int32_t)mouse.y, m_selectionEndX, m_selectionEndY);
		if (m_selectionEndX == UCHAR_MAX)
			m_selectionEndX = prevEndX;
		if (m_selectionEndY == UCHAR_MAX)
			m_selectionEndY = prevEndY;

		if (m_selectionEndX < m_selectionStartX)
			m_selectionEndX--;

		if (m_selectionEndY < m_selectionStartY)
			m_selectionEndY--;

		if (m_selectionStartX == m_selectionEndX && m_selectionStartY == m_selectionEndY)
			m_selectionActive = false;
		else
			m_selectionActive = true;
	}

	if (!m_selectionActive)
		return;


	LevelEditor::m_selectionWidth = std::abs(m_selectionEndX - m_selectionStartX) + 1;
	LevelEditor::m_selectionHeight = std::abs(m_selectionEndY - m_selectionStartY) + 1;

	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_C))
	{
		m_selectionCopyData.resize((size_t)(m_selectionWidth * m_selectionHeight));

		uint8_t startX = m_selectionStartX < m_selectionEndX ? m_selectionStartX : m_selectionEndX;
		uint8_t startY = m_selectionStartY < m_selectionEndY ? m_selectionStartY : m_selectionEndY;

		m_selectionCopyWidth = m_selectionWidth;
		m_selectionCopyHeight = m_selectionHeight;

		for (int32_t y = 0; y < m_selectionHeight; y++)
		{
			for (int32_t x = 0; x < m_selectionWidth; x++)
			{
				SquareType tile = m_playField->getGridTile(startX + x, startY + y);
				size_t offset = y * m_selectionCopyWidth + x;
				m_selectionCopyData[offset] = tile;
			}
		}
	}




	// Draw outline
	ImVec2 start((float_t)Globals::gWindowX + m_selectionStartX * GRID_SQUARE_SIZE,
		(float_t)Globals::gWindowY + m_selectionStartY * GRID_SQUARE_SIZE);
	ImVec2 end(Globals::gWindowX + m_selectionEndX * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE - 1.f,
		Globals::gWindowY + m_selectionEndY * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE - 1.f);

	Globals::gDrawList->AddRect(start, end, IM_COL32(0x80, 0x80, 0xB0, 0xC0), 0, 0, 4);

	LevelEditor::verticalSpace();

	ImGui::Text("Selection:");
	ImGui::Text("\tStart: (%d ; %d)", m_selectionStartX, m_selectionStartY);
	ImGui::Text("\tEnd:   (%d ; %d)", m_selectionEndX, m_selectionEndY);
	ImGui::Text("\tSize:  (%d ; %d)", m_selectionWidth, m_selectionHeight);
}

void LevelEditor::handleHotkeys()
{
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		LevelEditor::m_canPlaceTile = false;
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			uint8_t tileX;
			uint8_t tileY;
			ImVec2 mouse = Globals::gIO->MousePos;

			LevelEditor::m_playField->getGridPositionFromCoords((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);

			if (tileX != UCHAR_MAX && tileY != UCHAR_MAX)
				LevelEditor::m_currentBlockType = LevelEditor::m_playField->getGridTile(tileX, tileY);
			return;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_V) && m_selectionCopyData.size() != 0)
		{
			uint8_t tileX;
			uint8_t tileY;
			ImVec2 mouse = Globals::gIO->MousePos;
			
			m_playField->getGridPositionFromCoords((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);

			if (tileX == UCHAR_MAX || tileY == UCHAR_MAX)
				return;

			if (tileX + m_selectionCopyWidth > GRID_WIDTH)
				return;

			if (tileY + m_selectionCopyHeight > GRID_HEIGHT)
				return;

			for (int32_t y = 0; y < m_selectionHeight; y++)
			{
				for (int32_t x = 0; x < m_selectionCopyWidth; x++)
				{
					SquareType tile = m_selectionCopyData[y * m_selectionCopyWidth + x];
					m_playField->setGridTile(tileX + x, tileY + y, tile);
				}
			}

			AStar::findBestPath(0, 0, GRID_WIDTH - 1, GRID_HEIGHT - 1);
		}

		return;
	}

	LevelEditor::m_canPlaceTile = true;
}
