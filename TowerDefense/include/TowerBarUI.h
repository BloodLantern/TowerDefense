#pragma once

#include "TestTower.h"
#include <imgui.h>

#define TOWER_BAR_UI_OUTLINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)
#define TOWER_BAR_UI_BACKGROUND_COLOR IM_COL32(0x80, 0x80, 0x80, 0xFF)
#define TOWER_BAR_UI_TOWER_HOVER_COLOR IM_COL32(0xC0, 0x90, 0x40, 0xA0)

#define TOWER_COUNT 1

class TowerBarUI
{
public:
	void draw();
	void handleMouse();

private:
	const Tower* towerTemplates[TOWER_COUNT]{
		new TestTower(Point2(150, 150))
	};
	bool extended;


};
