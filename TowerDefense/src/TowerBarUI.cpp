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
		topLeft.x = (float)(((int) topLeft.x) / TOWER_BAR_TOWER_SIZE) * TOWER_BAR_TOWER_SIZE + x;
		ImVec2 bottomRight(topLeft.x + TOWER_BAR_TOWER_SIZE, topLeft.y + TOWER_BAR_TOWER_SIZE);

		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_BAR_UI_TOWER_HOVER_COLOR);
	}


	// Drag and drop
	ImVec2 mouseClickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];
	// If the click started on the tower bar and the mouse is dragging
	if (mouseClickedPos.y > y && mouseClickedPos.x < x + TOWER_BAR_TOWER_SIZE * TOWER_COUNT && mousePos.y < y)
	{
		int selectedTowerIndex = std::min(std::max((((int)mouseClickedPos.x) / TOWER_BAR_TOWER_SIZE) - 1, 0), TOWER_COUNT - 1);
		const Tower* selectedTower = towerTemplates[selectedTowerIndex];
		const int selectedTowerWidth = selectedTower->getWidth() * GRID_SQUARE_SIZE;
		const int selectedTowerHeight = selectedTower->getHeight() * GRID_SQUARE_SIZE;

		ImVec2 topLeft = ImVec2(mousePos.x - selectedTowerWidth / 2, mousePos.y - selectedTowerHeight / 2);
		ImVec2 bottomRight = ImVec2(topLeft.x + selectedTowerWidth, topLeft.y + selectedTowerHeight);

		int tileX = (int) (mousePos.x - Globals::gGridX) / GRID_SQUARE_SIZE;
		int tileY = (int) (mousePos.y - Globals::gGridY) / GRID_SQUARE_SIZE;
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{

			// If on the playfield
			if (mousePos.x < Globals::gGridX + Globals::gGame->getPlayField()->m_gridWidth * GRID_SQUARE_SIZE
				&& mousePos.y < Globals::gGridY + Globals::gGame->getPlayField()->m_gridHeight * GRID_SQUARE_SIZE)
			{
				int xSnapped = tileX * GRID_SQUARE_SIZE;
				int ySnapped = tileY * GRID_SQUARE_SIZE;
				topLeft.x = xSnapped + Globals::gGridX;
				topLeft.y = ySnapped + Globals::gGridY;
				bottomRight.x = topLeft.x + selectedTowerWidth;
				bottomRight.y = topLeft.y + selectedTowerHeight;
			}

			// Range
			Globals::gDrawList->AddCircleFilled(ImVec2(topLeft.x + selectedTowerWidth / 2, topLeft.y + selectedTowerHeight / 2),
				towerTemplates[selectedTowerIndex]->getRange() * GRID_SQUARE_SIZE, IM_COL32(0xB0, 0xB0, 0xB0, 0x70));
			// Tower
			Globals::gDrawList->AddImage(towerTextures[selectedTowerIndex].id, topLeft, bottomRight);

			// Debugging
			//Globals::gDrawList->AddText(ImVec2(topLeft.x + selectedTowerWidth / 2, topLeft.y + selectedTowerHeight / 2), IM_COL32(0xFF, 0xFF, 0xFF, 0xC0), std::to_string(selectedTowerIndex).c_str());
		}
		else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			// TODO Remove money
			// TODO Check if there is enough space
			Tower* tower = new Tower(*towerTemplates[selectedTowerIndex]);
			tower->setTilePosition(Point2(tileX, tileY));
			Globals::gGame->getPlayer()->getTowers()->push_back(tower);
		}
	}
}
