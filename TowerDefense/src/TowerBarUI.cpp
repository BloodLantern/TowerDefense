#include "TowerBarUI.h"
#include "Globals.h"

#define TOWER_BAR_SIZE 100

void TowerBarUI::draw()
{
	Globals::gDrawList->AddRect(ImVec2(Globals::gWindowX + 0, Globals::gWindowY + Globals::gWindowHeight),
		ImVec2(Globals::gWindowX + Globals::gWindowWidth, Globals::gWindowY + Globals::gWindowHeight - TOWER_BAR_SIZE), TOWER_BAR_UI_OUTLINE_COLOR);

	Globals::gDrawList->AddRectFilled(ImVec2(Globals::gWindowX + 0, Globals::gWindowY + Globals::gWindowHeight),
		ImVec2(Globals::gWindowX + Globals::gWindowWidth, Globals::gWindowY + Globals::gWindowHeight - TOWER_BAR_SIZE), TOWER_BAR_UI_BACKGROUND_COLOR);
}
