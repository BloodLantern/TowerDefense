#include "tower_bar_ui.hpp"
#include "globals.hpp"
#include "playfield.hpp"
#include "hud.hpp"

#include "minigun_tower.hpp"
#include "cannon_tower.hpp"
#include "beehive_tower.hpp"
#include "spray_tower.hpp"

#define TOWER_BAR_UI_OUTLINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)
#define TOWER_BAR_UI_BACKGROUND_COLOR IM_COL32(0x80, 0x80, 0x80, 0xFF)
#define TOWER_BAR_UI_TOWER_HOVER_COLOR IM_COL32(0xC0, 0x90, 0x40, 0xA0)
#define TOWER_BAR_UI_TOWER_COST_COLOR IM_COL32(0xD3, 0xC5, 0x3B, 0xFF)

#define TOWER_BAR_TOWER_OFFSET_X 8

#define TOWER_BAR_X 0
#define TOWER_BAR_Y (Globals::gWindowHeight - TOWER_BAR_HEIGHT)
#define TOWER_BAR_WIDTH (Globals::gWindowWidth - 7 * GRID_SQUARE_SIZE - GRID_SQUARE_SIZE / 2)

#define TOWER_BAR_TOWER_RANGE_COLOR_OCCUPIED IM_COL32(0xC0, 0x00, 0x00, 0x70)

TowerBarUI::TowerBarUI()
{
	towerTextures[0] = Globals::gResources->GetTexture("towers\\minigun_icon");
	towerTextures[1] = Globals::gResources->GetTexture("towers\\cannon_icon");
	towerTextures[2] = Globals::gResources->GetTexture("towers\\beehive");
	towerTextures[3] = Globals::gResources->GetTexture("towers\\spray");

	mTowerTemplates[0] = new MinigunTower(towerTextures[0]);
	mTowerTemplates[1] = new CannonTower(towerTextures[1]);
	mTowerTemplates[2] = new BeehiveTower(towerTextures[2]);
	mTowerTemplates[3] = new SprayTower(towerTextures[3]);
}

TowerBarUI::~TowerBarUI()
{
}

void TowerBarUI::Draw()
{
	const int32_t x = Globals::gWindowX + TOWER_BAR_X, y = Globals::gWindowY + TOWER_BAR_Y;
	const int32_t width = TOWER_BAR_WIDTH, height = TOWER_BAR_HEIGHT;

	DrawTowerBar(ImVec2(x, y), ImVec2(x + width, y + height));

	DrawTowers(x, y);

	HandleMouse();
}

void TowerBarUI::DrawTowerBar(const ImVec2 topLeft, const ImVec2 bottomRight)
{
	Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_BAR_UI_BACKGROUND_COLOR);
	Globals::gDrawList->AddRect(topLeft, bottomRight, TOWER_BAR_UI_OUTLINE_COLOR);
}

void TowerBarUI::DrawTowers(const float_t x, const float_t y)
{
	for (int i = 0; i < TOWER_COUNT; i++)
		Globals::gDrawList->AddImage(towerTextures[i]->id, ImVec2(x + i * TOWER_BAR_TOWER_SIZE, y + TOWER_BAR_Y_OFFSET / 2),
			ImVec2(x + i * TOWER_BAR_TOWER_SIZE + TOWER_BAR_TOWER_SIZE, y + TOWER_BAR_Y_OFFSET / 2 + TOWER_BAR_TOWER_SIZE));
}

void TowerBarUI::HandleMouse()
{
	if (Globals::gIO->WantCaptureMouse || !Hud::canInteract)
		return;

	const int32_t x = TOWER_BAR_X, y = TOWER_BAR_Y;
	const int32_t height = TOWER_BAR_HEIGHT;

	const ImVec2 mousePos = Globals::gIO->MousePos;
	// If on the tower bar
	if (mousePos.x > Globals::gWindowX && mousePos.y > Globals::gWindowY + y
		&& mousePos.x < Globals::gWindowX + TOWER_BAR_TOWER_SIZE * TOWER_COUNT && mousePos.y < Globals::gWindowY + y + height)
	{
		DrawOverlay(mousePos, y);
		ShowCost(mousePos, y);
	}

	HandleDragAndDrop(mousePos, x, y);
}

void TowerBarUI::DrawOverlay(const ImVec2& mousePos, const float_t y)
{
	// Draw an overlay on the tower
	ImVec2 topLeft(mousePos.x - Globals::gWindowX, Globals::gWindowY + y + TOWER_BAR_Y_OFFSET / 2);
	topLeft.x = (float_t)(((int32_t)topLeft.x) / TOWER_BAR_TOWER_SIZE) * TOWER_BAR_TOWER_SIZE + Globals::gWindowX;
	const ImVec2 bottomRight(topLeft.x + TOWER_BAR_TOWER_SIZE, topLeft.y + TOWER_BAR_TOWER_SIZE);

	Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_BAR_UI_TOWER_HOVER_COLOR, 0.5f);
}

void TowerBarUI::ShowCost(const ImVec2 &mousePos, const float_t y)
{
	uint32_t cost = mTowerTemplates[((int32_t)mousePos.x - Globals::gWindowX) / TOWER_BAR_TOWER_SIZE]->GetCost();

	ImVec2 pos(mousePos.x, Globals::gWindowY + y);
	pos.x = (((int32_t)pos.x - Globals::gWindowX) / TOWER_BAR_TOWER_SIZE) * (float_t) TOWER_BAR_TOWER_SIZE + Globals::gWindowX;
	
	const std::string text = "$" + std::to_string(cost);
	ImGui::PushFont(Globals::gFontMedium);
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
	
	Globals::gDrawList->AddRectFilled(pos, ImVec2(pos.x + textSize.x, pos.y + textSize.y), TOWER_BAR_UI_BACKGROUND_COLOR);
	Globals::gDrawList->AddRect(pos, ImVec2(pos.x + textSize.x, pos.y + textSize.y), TOWER_BAR_UI_OUTLINE_COLOR);
	// If the tower is affordable
	if (cost <= Globals::gGame->GetPlayer()->GetMoney())
		Globals::gDrawList->AddText(pos, TOWER_BAR_UI_TOWER_COST_COLOR, text.c_str());
	else
		Globals::gDrawList->AddText(pos, IM_COL32(0xFF, 0x40, 0x40, 0xFF), text.c_str());
	ImGui::PopFont();
}

void TowerBarUI::HandleDragAndDrop(const ImVec2& mousePos, const float_t x, const float_t y)
{
	ImVec2 mouseClickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];

	mouseClickedPos.x -= Globals::gWindowX;
	mouseClickedPos.y -= Globals::gWindowY;

	// If the click started on the tower bar and the mouse is dragging outside
	if (mouseClickedPos.y > y && mouseClickedPos.x < x + TOWER_BAR_TOWER_SIZE * TOWER_COUNT
		&& mouseClickedPos.x > x && mousePos.y - Globals::gWindowY < y
		// Optimization
		&& (ImGui::IsMouseDragging(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Left)))
	{
		// Don't allow dragging if the player doesn't have enough money
		if (mTowerTemplates[((int32_t)mouseClickedPos.x) / TOWER_BAR_TOWER_SIZE]->GetCost() > Globals::gGame->GetPlayer()->GetMoney())
			return;

		// Create the tower if it doesn't already exist
		if (!mSelectedTower)
			mSelectedTower = mTowerTemplates[((int32_t)mouseClickedPos.x) / TOWER_BAR_TOWER_SIZE]->Clone();

		// Tower size
		const uint8_t selectedTowerWidthTiles = mSelectedTower->GetWidth();
		const uint8_t selectedTowerHeightTiles = mSelectedTower->GetHeight();
		const int32_t selectedTowerWidthPixels = selectedTowerWidthTiles * GRID_SQUARE_SIZE;
		const int32_t selectedTowerHeightPixels = selectedTowerHeightTiles * GRID_SQUARE_SIZE;

		// Tower position
		mSelectedTower->SetPixelPosition(Point2(mousePos.x - Globals::gGridX, mousePos.y - Globals::gGridY));
		const bool isOnGrid = mSelectedTower->IsOnGrid();
		ComputeTowerPosition(isOnGrid,
			selectedTowerWidthTiles, selectedTowerHeightTiles, 
			selectedTowerWidthPixels, selectedTowerHeightPixels);

		const Point2& tilePosition = mSelectedTower->GetTilePosition();
		// Check for tile availability
		bool available = CheckTileAvailability(selectedTowerWidthTiles, selectedTowerHeightTiles, tilePosition);

		// Check if the mouse is dragging the tower
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			HandleDrag(isOnGrid, available);
		// If the mouse dropped the tower somewhere
		else
			HandleDrop(available, tilePosition);
	}
	else if (mSelectedTower)
	{
		delete mSelectedTower;
		mSelectedTower = nullptr;
	}
}

void TowerBarUI::ComputeTowerPosition(const bool& isOnGrid,
	const uint8_t& selectedTowerWidthTiles, const uint8_t& selectedTowerHeightTiles,
	const uint32_t& selectedTowerWidthPixels, const uint32_t& selectedTowerHeightPixels)
{
	if (isOnGrid)
	{
		// If it is on the grid, snap it according to its size
		const Point2& pixelPosition = mSelectedTower->GetPixelPosition();
		mSelectedTower->SetPixelPosition(Point2(pixelPosition.x - (selectedTowerWidthTiles / 2.f - 0.5f) * GRID_SQUARE_SIZE,
												pixelPosition.y - (selectedTowerHeightTiles / 2.f - 0.5f) * GRID_SQUARE_SIZE));
		mSelectedTower->SnapToGrid();
	}
	else
	{
		// Otherwise, make it follow the mouse
		const Point2& pixelPosition = mSelectedTower->GetPixelPosition();
		mSelectedTower->SetPixelPosition(Point2(pixelPosition.x - selectedTowerWidthPixels / 2, pixelPosition.y - selectedTowerHeightPixels / 2));
	}
}

bool TowerBarUI::CheckTileAvailability(const uint8_t& selectedTowerWidthTiles, const uint8_t& selectedTowerHeightTiles, const Point2& tilePosition)
{
	bool result = true;
	for (uint8_t x = 0; x < selectedTowerWidthTiles; x++)
	{
		for (uint8_t y = 0; y < selectedTowerHeightTiles; y++)
		{
			if (result)
			{
				ClipdataType tileType = Globals::gGame->GetPlayField()->GetClipdataTile(tilePosition.x + x, tilePosition.y + y);
				result = tileType == CLIPDATA_TYPE_EMPTY || tileType == CLIPDATA_TYPE_PLAYER_ONLY;
			}
			else
				break;
		}
		if (!result)
			break;
	}
	return result;
}

void TowerBarUI::HandleDrag(const bool& isOnGrid, const bool& available)
{
	if (isOnGrid)
		mSelectedTower->SnapToGrid();

	// Tower rendering
	mSelectedTower->DrawRange(available ? TOWER_RANGE_COLOR_AVAILABLE : TOWER_BAR_TOWER_RANGE_COLOR_OCCUPIED);
	mSelectedTower->DrawTowerBarTexture();
}

void TowerBarUI::HandleDrop(const bool& available, const Point2& tilePosition)
{
	Player* player = Globals::gGame->GetPlayer();
	if (available && player->GetMoney() >= mSelectedTower->GetCost())
	{
		mSelectedTower->SetOwner(player);
		player->DecreaseMoney(mSelectedTower->GetCost());
		mSelectedTower->SnapToGrid();
		player->GetTowers()->push_back(mSelectedTower);

		PlayField* playfield = Globals::gGame->GetPlayField();
		for (uint8_t x = 0; x < mSelectedTower->GetWidth(); x++)
			for (uint8_t y = 0; y < mSelectedTower->GetHeight(); y++)
				playfield->SetClipdataTile(tilePosition.x + x, tilePosition.y + y, CLIPDATA_TYPE_OCCUPIED);

		mSelectedTower = nullptr;
	}
	else
	{
		delete mSelectedTower;
		mSelectedTower = nullptr;
	}
}
