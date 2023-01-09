#include "TowerBarUI.h"
#include "Globals.h"
#include "PlayField.h"

#define TOWER_BAR_TOWER_SIZE 64
#define TOWER_BAR_TOWER_OFFSET_X 8

#define TOWER_BAR_Y_OFFSET 2
#define TOWER_BAR_HEIGHT TOWER_BAR_TOWER_SIZE + TOWER_BAR_Y_OFFSET
#define TOWER_BAR_X Globals::gWindowX
#define TOWER_BAR_Y Globals::gWindowY + Globals::gWindowHeight - TOWER_BAR_TOWER_SIZE - TOWER_BAR_Y_OFFSET
#define TOWER_BAR_WIDTH Globals::gWindowWidth

void TowerBarUI::draw()
{
	int32_t x = TOWER_BAR_X;
	int32_t y = TOWER_BAR_Y;
	int32_t width = TOWER_BAR_WIDTH;
	int32_t height = TOWER_BAR_HEIGHT;

	Globals::gDrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), TOWER_BAR_UI_BACKGROUND_COLOR);
	Globals::gDrawList->AddRect(ImVec2(x, y), ImVec2(x + width, y + height), TOWER_BAR_UI_OUTLINE_COLOR);

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
	ImVec2 pos = Globals::gIO->MousePos;
	// If on the tower bar
	if (pos.x > x && pos.y > y && pos.x < x + TOWER_BAR_TOWER_SIZE * TOWER_COUNT && pos.y < y + height)
	{
		ImVec2 topLeft(pos.x - x, y + TOWER_BAR_Y_OFFSET / 2);
		topLeft.x = (((int) topLeft.x) / TOWER_BAR_TOWER_SIZE) * TOWER_BAR_TOWER_SIZE + x;
		ImVec2 bottomRight(topLeft.x + TOWER_BAR_TOWER_SIZE, topLeft.y + TOWER_BAR_TOWER_SIZE);
		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_BAR_UI_TOWER_HOVER_COLOR, 10.f);
	}


	// Drag and drop
	ImVec2 clickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];
	// If the click started on the tower bar and dragging
	if (clickedPos.y > y && clickedPos.x < x + TOWER_BAR_TOWER_SIZE * TOWER_COUNT && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		// TODO Get tower texture and display it under the mouse
		//int selectedTower = ((int) clickedPos.x) / TOWER_BAR_TOWER_SIZE; 

		// Temp
		ImVec2 mousePos = Globals::gIO->MousePos;
		ImVec2 topLeft = ImVec2(mousePos.x - TOWER_BAR_TOWER_SIZE / 2, mousePos.y - TOWER_BAR_TOWER_SIZE / 2);
		ImVec2 bottomRight = ImVec2(topLeft.x + TOWER_BAR_TOWER_SIZE, topLeft.y + TOWER_BAR_TOWER_SIZE);

		// If on the playfield
		if (pos.y < Globals::gGridY + Globals::gPlayField->m_gridHeight * GRID_SQUARE_SIZE && pos.x < Globals::gGridX + Globals::gPlayField->m_gridWidth * GRID_SQUARE_SIZE)
		{
			int xSnapped = ((int) (mousePos.x - Globals::gGridX) / GRID_SQUARE_SIZE) * GRID_SQUARE_SIZE;
			int ySnapped = ((int) (mousePos.y - Globals::gGridY) / GRID_SQUARE_SIZE) * GRID_SQUARE_SIZE;
			topLeft.x = xSnapped + Globals::gGridX;
			topLeft.y = ySnapped + Globals::gGridY;
			bottomRight.x = topLeft.x + TOWER_BAR_TOWER_SIZE;
			bottomRight.y = topLeft.y + TOWER_BAR_TOWER_SIZE;
		}

		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_BAR_UI_TOWER_HOVER_COLOR, 10.f);
	}
}
