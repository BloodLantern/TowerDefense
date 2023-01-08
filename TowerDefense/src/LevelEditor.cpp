#include "LevelEditor.h"

#include <imgui.h>

#include "Globals.h"
#include "AStar.h"
#include "Gui.h"

int32_t LevelEditor::m_currentBlockType;
int32_t LevelEditor::m_currentLayer;
uint16_t LevelEditor::m_currentTileValue;

bool LevelEditor::m_dragEnabled;
bool LevelEditor::m_canPlaceTile;
bool LevelEditor::m_updateAStar;
char LevelEditor::m_fileName[20];

int32_t LevelEditor::m_gridWidthInput;
int32_t LevelEditor::m_gridHeightInput;

bool LevelEditor::m_selectionActive;
uint8_t LevelEditor::m_selectionStartX;
uint8_t LevelEditor::m_selectionStartY;
uint8_t LevelEditor::m_selectionEndX;
uint8_t LevelEditor::m_selectionEndY;
uint8_t LevelEditor::m_selectionWidth;
uint8_t LevelEditor::m_selectionHeight;
uint8_t LevelEditor::m_selectionCopyWidth;
uint8_t LevelEditor::m_selectionCopyHeight;
std::vector<ClipdataType> LevelEditor::m_selectionCopyData;

static const char* const sClipdataNames[] = {
	"Empty",
	"Nothing",
	"Enemy only",
	"Occupied",
	"Player only",
};

void LevelEditor::update()
{
	if (ImGui::Begin("Level editor", &Gui::m_openedWindows[GUI_WINDOW_ID_LEVEL_EDITOR]))
	{
		LevelEditor::handleMisc();

		LevelEditor::handleClear();

		LevelEditor::verticalSpace();
		LevelEditor::handleFile();

		LevelEditor::verticalSpace();
		LevelEditor::handleResize();

		LevelEditor::verticalSpace();
		LevelEditor::handleCursor();
		LevelEditor::handleSelection();

		LevelEditor::handleTileset();

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
	ImGui::Text("Clipdata");
	ImGui::SameLine();
	ImGui::Combo("##", &LevelEditor::m_currentBlockType, sClipdataNames, IM_ARRAYSIZE(sClipdataNames));

	LevelEditor::verticalSpace();

	ImGui::RadioButton("Layer 0", &LevelEditor::m_currentLayer, 0);
	ImGui::RadioButton("Layer 1", &LevelEditor::m_currentLayer, 1);

	ImGui::Text("Layer : %d ; Tile : %d", LevelEditor::m_currentLayer, LevelEditor::m_currentTileValue);

	LevelEditor::verticalSpace();

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
			for (int32_t x = 0; x < Globals::gPlayField->m_gridWidth; x++)
				for (int32_t y = 0; y < Globals::gPlayField->m_gridHeight; y++)
					Globals::gPlayField->setClipdataTile(x, y, CLIPDATA_TYPE_EMPTY);

			AStar::findBestPath(0, 0, Globals::gPlayField->m_gridWidth - 1, Globals::gPlayField->m_gridHeight - 1);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("No", ImVec2(120, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}


void LevelEditor::handleResize()
{
	ImGui::InputInt("Grid width", &LevelEditor::m_gridWidthInput, 1, 5, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputInt("Grid height", &LevelEditor::m_gridHeightInput, 1, 5, ImGuiInputTextFlags_CharsDecimal);

	if (ImGui::Button("Resize"))
	{
		if (LevelEditor::m_gridWidthInput > 0 && LevelEditor::m_gridHeightInput > 0)
			Globals::gPlayField->resize(LevelEditor::m_gridWidthInput, LevelEditor::m_gridHeightInput);
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
		Globals::gPlayField->save(LevelEditor::m_fileName);

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		Globals::gPlayField->load(LevelEditor::m_fileName);

		AStar::findBestPath(0, 0, Globals::gPlayField->m_gridWidth - 1, Globals::gPlayField->m_gridHeight - 1);
	}
}

void LevelEditor::handleCursor()
{
	if (Globals::gIO->WantCaptureMouse)
		return;

	ImVec2 mouse = Globals::gIO->MousePos;
	uint8_t tileX;
	uint8_t tileY;

	Globals::gPlayField->getGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);
	if (tileX != UCHAR_MAX && tileY != UCHAR_MAX)
	{
		ImVec2 pMin((float_t)Globals::gGridX + tileX * GRID_SQUARE_SIZE, (float_t)Globals::gGridY + tileY * GRID_SQUARE_SIZE);
		ImVec2 pMax((float_t)Globals::gGridX + (tileX + 1) * GRID_SQUARE_SIZE, (float_t)Globals::gGridY + (tileY + 1) * GRID_SQUARE_SIZE);

		Globals::gDrawList->AddRectFilled(pMin, pMax, IM_COL32(0xFF, 0, 0, 0x50));
		ImGui::Text("Cursor position : (%d ; %d)", tileX, tileY);

		if (!LevelEditor::m_canPlaceTile)
			return;

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
			(LevelEditor::m_dragEnabled && ImGui::IsMouseDown(ImGuiMouseButton_Left)))
		{
			Globals::gPlayField->setClipdataTile(tileX, tileY, static_cast<ClipdataType>(LevelEditor::m_currentBlockType));

			Globals::gPlayField->setLayertile(tileX, tileY, LevelEditor::m_currentLayer, LevelEditor::m_currentTileValue);

			if (LevelEditor::m_updateAStar)
				AStar::findBestPath(0, 0, Globals::gPlayField->m_gridWidth - 1, Globals::gPlayField->m_gridHeight - 1);
		}
	}
}


void LevelEditor::handleSelection()
{
	if (Globals::gIO->WantCaptureMouse)
		return;

	ImVec2 mouse = Globals::gIO->MousePos;
	// Update selection positions
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		Globals::gPlayField->getGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, m_selectionStartX, m_selectionStartY);
		if (m_selectionStartX == UCHAR_MAX || m_selectionStartY == UCHAR_MAX)
			return;
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		uint8_t prevEndX = m_selectionEndX;
		uint8_t prevEndY = m_selectionEndY;

		Globals::gPlayField->getGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, m_selectionEndX, m_selectionEndY);
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
				ClipdataType tile = Globals::gPlayField->getClipdataTile(startX + x, startY + y);
				size_t offset = y * m_selectionCopyWidth + x;
				m_selectionCopyData[offset] = tile;
			}
		}
	}




	// Draw outline
	ImVec2 start((float_t)Globals::gGridX + m_selectionStartX * GRID_SQUARE_SIZE,
		(float_t)Globals::gGridY + m_selectionStartY * GRID_SQUARE_SIZE);
	ImVec2 end(Globals::gGridX + m_selectionEndX * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE - 1.f,
		Globals::gGridY + m_selectionEndY * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE - 1.f);

	Globals::gDrawList->AddRect(start, end, IM_COL32(0x80, 0x80, 0xB0, 0xC0), 0, 0, 4);

	LevelEditor::verticalSpace();

	ImGui::Text("Selection:");
	ImGui::Text("\tStart: (%d ; %d)", m_selectionStartX, m_selectionStartY);
	ImGui::Text("\tEnd:   (%d ; %d)", m_selectionEndX, m_selectionEndY);
	ImGui::Text("\tSize:  (%d ; %d)", m_selectionWidth, m_selectionHeight);
}

void LevelEditor::handleTileset()
{
	if (ImGui::Begin("Tileset", nullptr, ImGuiWindowFlags_NoDecoration))
	{
		ImGui::SetScrollY(GRID_SQUARE_SIZE * (int)(ImGui::GetScrollY() / GRID_SQUARE_SIZE));

		ImGui::Image(Globals::gPlayField->m_tileset.id, ImVec2(Globals::gPlayField->m_tileset.width,
			Globals::gPlayField->m_tileset.height));

		ImVec2 mousePos = Globals::gIO->MousePos;
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		if (mousePos.x > winPos.x && mousePos.x < winPos.x + winSize.x && mousePos.y > winPos.y && mousePos.y < winPos.y + winSize.y)
		{
			ImDrawList* dl = ImGui::GetForegroundDrawList();
			uint32_t tileX = (mousePos.x - 8 - winPos.x) / GRID_SQUARE_SIZE;
			uint32_t tileY = (mousePos.y - 8 - winPos.y) / GRID_SQUARE_SIZE;

			uint32_t x = (tileX * GRID_SQUARE_SIZE) + winPos.x + 8;
			uint32_t y = (tileY * GRID_SQUARE_SIZE) + winPos.y + 8;

			dl->AddRect(ImVec2(x, y), ImVec2(x + GRID_SQUARE_SIZE, y + GRID_SQUARE_SIZE), IM_COL32(0xFF, 0, 0, 0xFF));

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left, false))
			{
				LevelEditor::m_currentTileValue = tileY * (Globals::gPlayField->m_tileset.width / GRID_SQUARE_SIZE) + tileX;
			}
		}
	}

	ImGui::End();
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

			Globals::gPlayField->getGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);

			if (tileX != UCHAR_MAX && tileY != UCHAR_MAX)
				LevelEditor::m_currentBlockType = Globals::gPlayField->getClipdataTile(tileX, tileY);
			return;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_V) && m_selectionCopyData.size() != 0)
		{
			uint8_t tileX;
			uint8_t tileY;
			ImVec2 mouse = Globals::gIO->MousePos;

			Globals::gPlayField->getGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);

			if (tileX == UCHAR_MAX || tileY == UCHAR_MAX)
				return;

			if (tileX + m_selectionCopyWidth > Globals::gPlayField->m_gridWidth)
				return;

			if (tileY + m_selectionCopyHeight > Globals::gPlayField->m_gridHeight)
				return;

			for (int32_t y = 0; y < m_selectionHeight; y++)
			{
				for (int32_t x = 0; x < m_selectionCopyWidth; x++)
				{
					ClipdataType tile = m_selectionCopyData[y * m_selectionCopyWidth + x];
					Globals::gPlayField->setClipdataTile(tileX + x, tileY + y, tile);
				}
			}

			AStar::findBestPath(0, 0, Globals::gPlayField->m_gridWidth - 1, Globals::gPlayField->m_gridHeight - 1);
		}

		return;
	}

	LevelEditor::m_canPlaceTile = true;
}
