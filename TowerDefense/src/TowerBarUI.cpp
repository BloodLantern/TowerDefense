#include "TowerBarUI.h"
#include "Globals.h"

#define TOWER_BAR_TOWER_SIZE 64
#define TOWER_BAR_SIZE TOWER_BAR_TOWER_SIZE + 10
#define TOWER_BAR_X Globals::gWindowX
#define TOWER_BAR_Y Globals::gWindowY + Globals::gWindowHeight - TOWER_BAR_SIZE
#define TOWER_BAR_WIDTH Globals::gWindowWidth
#define TOWER_BAR_HEIGHT TOWER_BAR_SIZE

void TowerBarUI::draw()
{
	int32_t x = TOWER_BAR_X;
	int32_t y = TOWER_BAR_Y;
	int32_t width = TOWER_BAR_WIDTH;
	int32_t height = TOWER_BAR_HEIGHT;

	Globals::gDrawList->AddRect(ImVec2(x, y), ImVec2(x + width, y + height), TOWER_BAR_UI_OUTLINE_COLOR);

	Globals::gDrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), TOWER_BAR_UI_BACKGROUND_COLOR);

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
	if (pos.x > x && pos.y > y && pos.x < x + width && pos.y < y + height)
	{
		ImVec2 topLeft(pos.x - x, y);

		topLeft.x = (((int) topLeft.x) / TOWER_BAR_TOWER_SIZE + 1) * TOWER_BAR_TOWER_SIZE;

		ImVec2 bottomRight = topLeft;
		bottomRight.x += TOWER_BAR_TOWER_SIZE;
		bottomRight.y += TOWER_BAR_TOWER_SIZE;

		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_BAR_UI_TOWER_HOVER_COLOR, 10.f);
	}


	// Drag and drop

	ImVec2 clickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];

	if (clickedPos.y > Globals::gWindowY + Globals::gWindowHeight - TOWER_BAR_SIZE && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{

	}
}
