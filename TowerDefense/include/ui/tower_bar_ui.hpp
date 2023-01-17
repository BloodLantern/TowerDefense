#pragma once

#include <imgui.h>
#include <imgui_utils.hpp>

#include "tower.hpp"

#define TOWER_COUNT 3

#define TOWER_BAR_TOWER_SIZE 64
#define TOWER_BAR_Y_OFFSET 2
#define TOWER_BAR_HEIGHT (TOWER_BAR_TOWER_SIZE + TOWER_BAR_Y_OFFSET)

class TowerBarUI
{
public:
	Texture* towerTextures[TOWER_COUNT];

	TowerBarUI();
	~TowerBarUI();

	void Draw();

private:
	Tower* mTowerTemplates[TOWER_COUNT];
	Tower* mSelectedTower = nullptr;
	bool mExtended = false;

	void DrawTowerBar(const ImVec2 topLeft, const ImVec2 bottomRight);
	void DrawTowers(const float_t x, const float_t y);
	void HandleMouse();
	void DrawOverlay(const ImVec2& mousePos, const float_t y);
	void ShowCost(const ImVec2& mousePos, const float_t y);
	void HandleDragAndDrop(const ImVec2& mousePos, const float_t x, const float_t y);
	void ComputeTowerPosition(const bool& isOnGrid, const uint8_t& selectedTowerWidthTiles, const uint8_t& selectedTowerHeightTiles, const uint32_t& selectedTowerWidthPixels, const uint32_t& selectedTowerHeightPixels);
	bool CheckTileAvailability(const uint8_t& selectedTowerWidthTiles, const uint8_t& selectedTowerHeightTiles, const Point2& tilePosition);
	void HandleDrag(const bool& isOnGrid, const bool& available);
	void HandleDrop(const bool& available, const Point2& tilePosition);
};
