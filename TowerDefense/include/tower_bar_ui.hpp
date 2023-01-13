#pragma once

#include "test_tower.hpp"

#include <imgui.h>
#include <imgui_utils.hpp>

#define TOWER_COUNT 1

#define TOWER_BAR_TOWER_SIZE 64
#define TOWER_BAR_Y_OFFSET 2
#define TOWER_BAR_HEIGHT (TOWER_BAR_TOWER_SIZE + TOWER_BAR_Y_OFFSET)

class TowerBarUI
{
public:
	Texture* mTowerTextures[TOWER_COUNT];

	TowerBarUI();
	~TowerBarUI();

	void Draw();

private:
	Tower* mTowerTemplates[TOWER_COUNT];
	Tower* mSelectedTower = nullptr;
	bool mExtended = false;

	void HandleMouse();
};
