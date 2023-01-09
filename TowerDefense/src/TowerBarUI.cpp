#include "TowerBarUI.h"
#include "Globals.h"
#include "PlayField.h"

#include <string>

#define TOWER_BAR_TOWER_SIZE 64
#define TOWER_BAR_TOWER_OFFSET_X 8

#define TOWER_BAR_Y_OFFSET 2
#define TOWER_BAR_HEIGHT TOWER_BAR_TOWER_SIZE + TOWER_BAR_Y_OFFSET
#define TOWER_BAR_X Globals::gWindowX
#define TOWER_BAR_Y Globals::gWindowY + Globals::gWindowHeight - TOWER_BAR_TOWER_SIZE - TOWER_BAR_Y_OFFSET
#define TOWER_BAR_WIDTH Globals::gWindowWidth

TowerBarUI::TowerBarUI()
{
	for (int i = 0; i < TOWER_COUNT; i++)
		towerTextures[i] = ImGuiUtils::LoadTexture("assets/towers/TestTower.png");
}

TowerBarUI::~TowerBarUI()
{
	for (int i = 0; i < TOWER_COUNT; i++) {
		ImGuiUtils::UnloadTexture(towerTextures[i]);
		delete towerTemplates[i];
	}
}

void TowerBarUI::draw()
{
	int32_t x = TOWER_BAR_X;
	int32_t y = TOWER_BAR_Y;
	int32_t width = TOWER_BAR_WIDTH;
	int32_t height = TOWER_BAR_HEIGHT;

	Globals::gDrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), TOWER_BAR_UI_BACKGROUND_COLOR);
	Globals::gDrawList->AddRect(ImVec2(x, y), ImVec2(x + width, y + height), TOWER_BAR_UI_OUTLINE_COLOR);

	for (int i = 0; i < TOWER_COUNT; i++)
		Globals::gDrawList->AddImage(towerTextures[i].id, ImVec2(x, y + TOWER_BAR_Y_OFFSET / 2),
			ImVec2(x + TOWER_BAR_TOWER_SIZE, y + TOWER_BAR_Y_OFFSET / 2 + TOWER_BAR_TOWER_SIZE));

	handleMouse();
}

void TowerBarUI::handleMouse()
{
	if (Globals::gIO->WantCaptureMouse)
		return;

	int32_t x = TOWER_BAR_X;
	int32_t y = TOWER_BAR_Y;
	int32_t width = TOWER_BAR_WIDTH;
	int32_t height = TOWER_BAR_HEIGHT;

	// Overlay
	ImVec2 mousePos = Globals::gIO->MousePos;
	// If on the tower bar
	if (mousePos.x > x && mousePos.y > y && mousePos.x < x + TOWER_BAR_TOWER_SIZE * TOWER_COUNT && mousePos.y < y + height)
	{
		ImVec2 topLeft(mousePos.x - x, y + TOWER_BAR_Y_OFFSET / 2);
		topLeft.x = (((int) topLeft.x) / TOWER_BAR_TOWER_SIZE) * TOWER_BAR_TOWER_SIZE + x;
		ImVec2 bottomRight(topLeft.x + TOWER_BAR_TOWER_SIZE, topLeft.y + TOWER_BAR_TOWER_SIZE);

		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_BAR_UI_TOWER_HOVER_COLOR);
	}


	// Drag and drop
	ImVec2 mouseClickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];
	// If the click started on the tower bar and the mouse is dragging
	if (mouseClickedPos.y > y && mouseClickedPos.x < x + TOWER_BAR_TOWER_SIZE * TOWER_COUNT && mousePos.y < y)
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			int selectedTowerIndex = std::min(std::max((((int)mouseClickedPos.x) / TOWER_BAR_TOWER_SIZE) - 1, 0), TOWER_COUNT - 1);

			const Tower* selectedTower = towerTemplates[selectedTowerIndex];
			const int selectedTowerWidth = selectedTower->getWidth() * GRID_SQUARE_SIZE;
			const int selectedTowerHeight = selectedTower->getHeight() * GRID_SQUARE_SIZE;

			ImVec2 topLeft = ImVec2(mousePos.x - selectedTowerWidth / 2, mousePos.y - selectedTowerHeight / 2);

			ImVec2 bottomRight = ImVec2(topLeft.x + selectedTowerWidth, topLeft.y + selectedTowerHeight);

			// If on the playfield
			if (mousePos.x < Globals::gGridX + Globals::gPlayField->m_gridWidth * GRID_SQUARE_SIZE
				&& mousePos.y < Globals::gGridY + Globals::gPlayField->m_gridHeight * GRID_SQUARE_SIZE)
			{
				int xSnapped = ((int) (mousePos.x - Globals::gGridX) / GRID_SQUARE_SIZE) * GRID_SQUARE_SIZE;
				int ySnapped = ((int) (mousePos.y - Globals::gGridY) / GRID_SQUARE_SIZE) * GRID_SQUARE_SIZE;
				topLeft.x = xSnapped + Globals::gGridX;
				topLeft.y = ySnapped + Globals::gGridY;
				bottomRight.x = topLeft.x + selectedTowerWidth;
				bottomRight.y = topLeft.y + selectedTowerHeight;
			}

			Globals::gDrawList->AddImage(towerTextures[selectedTowerIndex].id, topLeft, bottomRight);
			// Debugging
			//Globals::gDrawList->AddText(ImVec2(topLeft.x + selectedTowerWidth / 2, topLeft.y + selectedTowerHeight / 2), IM_COL32(0xFF, 0xFF, 0xFF, 0xC0), std::to_string(selectedTowerIndex).c_str());
		} else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			
		}
}
