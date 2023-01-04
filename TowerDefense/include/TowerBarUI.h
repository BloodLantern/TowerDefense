#pragma once

#include "TestTower.h"
#include <imgui.h>

#define TOWER_BAR_UI_OUTLINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)
#define TOWER_BAR_UI_BACKGROUND_COLOR IM_COL32(0x80, 0x80, 0x80, 0xFF)

class TowerBarUI
{
public:
	void draw();

private:
	const Tower* towerTemplates[1]{
		new TestTower(Point2(150, 150))
	};
	bool extended;
};
