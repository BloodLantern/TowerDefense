#include "tower_bar_ui.hpp"
#include "globals.hpp"
#include "playfield.hpp"
#include "hud.hpp"

#include "test_tower.hpp"
#include "cannon_tower.hpp"
#include "beehive_tower.hpp"

#define TOWER_BAR_UI_OUTLINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)
#define TOWER_BAR_UI_BACKGROUND_COLOR IM_COL32(0x80, 0x80, 0x80, 0xFF)
#define TOWER_BAR_UI_TOWER_HOVER_COLOR IM_COL32(0xC0, 0x90, 0x40, 0xA0)

#define TOWER_BAR_TOWER_OFFSET_X 8

#define TOWER_BAR_Y (Globals::gWindowHeight - TOWER_BAR_HEIGHT)
#define TOWER_BAR_WIDTH (Globals::gWindowWidth - 7 * GRID_SQUARE_SIZE - GRID_SQUARE_SIZE / 2)

#define TOWER_BAR_TOWER_RANGE_COLOR_OCCUPIED IM_COL32(0xC0, 0x00, 0x00, 0x70)

TowerBarUI::TowerBarUI()
{
	towerTextures[0] = Globals::gResources->GetTexture("towers\\unicorn");
	towerTextures[1] = Globals::gResources->GetTexture("towers\\cannon_icon");
	towerTextures[2] = Globals::gResources->GetTexture("towers\\beehive");
		
	mTowerTemplates[0] = new TestTower(towerTextures[0]);
	mTowerTemplates[1] = new CannonTower(towerTextures[1]);
	mTowerTemplates[2] = new BeehiveTower(towerTextures[2]);
}

TowerBarUI::~TowerBarUI()
{
}

void TowerBarUI::Draw()
{
	const int32_t x = Globals::gWindowX, y = Globals::gWindowY + TOWER_BAR_Y;
	const int32_t width = TOWER_BAR_WIDTH, height = TOWER_BAR_HEIGHT;

	Globals::gDrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), TOWER_BAR_UI_BACKGROUND_COLOR);
	Globals::gDrawList->AddRect(ImVec2(x, y), ImVec2(x + width, y + height), TOWER_BAR_UI_OUTLINE_COLOR);

	for (int i = 0; i < TOWER_COUNT; i++)
		Globals::gDrawList->AddImage(towerTextures[i]->id, ImVec2(x + i * TOWER_BAR_TOWER_SIZE, y + TOWER_BAR_Y_OFFSET / 2),
			ImVec2(x + i * TOWER_BAR_TOWER_SIZE + TOWER_BAR_TOWER_SIZE, y + TOWER_BAR_Y_OFFSET / 2 + TOWER_BAR_TOWER_SIZE));

	HandleMouse();
}

void TowerBarUI::HandleMouse()
{
	//if (Globals::gIO->WantCaptureMouse || !Hud::canInteract)
	//	return;

	const int32_t x = 0, y = TOWER_BAR_Y;
	const int32_t height = TOWER_BAR_HEIGHT;

	// Overlay

	const ImVec2 mousePos = Globals::gIO->MousePos;
	// If on the tower bar
	if (mousePos.x > Globals::gWindowX && mousePos.y > Globals::gWindowY + y
		&& mousePos.x < Globals::gWindowX + TOWER_BAR_TOWER_SIZE * TOWER_COUNT && mousePos.y < Globals::gWindowY + y + height)
	{
		// Draw an overlay on the tower
		ImVec2 topLeft(mousePos.x - Globals::gWindowX, Globals::gWindowY + y + TOWER_BAR_Y_OFFSET / 2);
		topLeft.x = (float_t)(((int32_t)topLeft.x) / TOWER_BAR_TOWER_SIZE) * TOWER_BAR_TOWER_SIZE + Globals::gWindowX;
		ImVec2 bottomRight(topLeft.x + TOWER_BAR_TOWER_SIZE, topLeft.y + TOWER_BAR_TOWER_SIZE);

		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_BAR_UI_TOWER_HOVER_COLOR);
	}


	// Drag and drop

	ImVec2 mouseClickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];
	mouseClickedPos.x -= Globals::gWindowX;
	mouseClickedPos.y -= Globals::gWindowY;
	// If the click started on the tower bar and the mouse is dragging outside
	if (mouseClickedPos.y > y && mouseClickedPos.x < x + TOWER_BAR_TOWER_SIZE * TOWER_COUNT
		&& mouseClickedPos.x > x && mousePos.y - Globals::gWindowY < y
		// Optimization
		&& (ImGui::IsMouseDragging(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Left)))
	{
		// Create the tower if it doesn't already exist
		if (!mSelectedTower)
			mSelectedTower = mTowerTemplates[((int32_t)mouseClickedPos.x) / TOWER_BAR_TOWER_SIZE]->Clone();

		const int32_t selectedTowerWidth = mSelectedTower->GetWidth() * GRID_SQUARE_SIZE;
		const int32_t selectedTowerHeight = mSelectedTower->GetHeight() * GRID_SQUARE_SIZE;

		mSelectedTower->SetPixelPosition(Point2(mousePos.x - Globals::gGridX, mousePos.y - Globals::gGridY));
		const bool isOnGrid = mSelectedTower->IsOnGrid();
		const Point2& tilePosition = mSelectedTower->GetTilePosition();

		// If it is on the grid, snap it
		if (isOnGrid)
		{
			mSelectedTower->SnapToGrid();
		}
		// Otherwise, make it follow the mouse
		else
		{
			const Point2& pixelPosition = mSelectedTower->GetPixelPosition();
			mSelectedTower->SetPixelPosition(Point2(pixelPosition.x - selectedTowerWidth / 2, pixelPosition.y - selectedTowerHeight / 2));
		}

		// Check for tile availability
		bool available = true;
		for (uint8_t x = 0; x < mSelectedTower->GetWidth(); x++)
		{
			for (uint8_t y = 0; y < mSelectedTower->GetHeight(); y++)
			{
				if (available)
				{
					ClipdataType tileType = Globals::gGame->GetPlayField()->GetClipdataTile(tilePosition.x + x, tilePosition.y + y);
					available = tileType == CLIPDATA_TYPE_EMPTY || tileType == CLIPDATA_TYPE_PLAYER_ONLY;
				}
				else
					break;
			}
			if (!available)
				break;
		}

		// Check if the mouse is dragging the tower
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			if (isOnGrid)
				mSelectedTower->SnapToGrid();

			// Tower rendering
			mSelectedTower->DrawRange(available ? TOWER_RANGE_COLOR_AVAILABLE : TOWER_BAR_TOWER_RANGE_COLOR_OCCUPIED);
			//Globals::gDrawList->AddImage(mSelectedTower->GetTexture()->id, topLeft, ImVec2(topLeft.x + selectedTowerWidth, topLeft.y + selectedTowerHeight));
			mSelectedTower->OnRender();
		}
		// If the mouse dropped the tower somewhere
		else
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
	}
	else if (mSelectedTower)
	{
		delete mSelectedTower;
		mSelectedTower = nullptr;
	}
}
