#include "tower.hpp"

#include "globals.hpp"

#include <GLFW/glfw3.h>

#define TOWER_RANGE_OUTLINE_COLOR IM_COL32(0x20, 0x20, 0x20, 0xA0)
#define TOWER_SELECTION_OVERLAY_COLOR IM_COL32(0x80, 0x80, 0x80, 0x80)
#define TOWER_SELECTION_OUTLINE_COLOR IM_COL32(0xFF, 0xFF, 0xFF, 0xFF)

#define TOWER_PANEL_OFFSET_X 17
#define TOWER_PANEL_WIDTH 6 * GRID_SQUARE_SIZE
#define TOWER_PANEL_OUTLINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)
#define TOWER_PANEL_BACKGROUND_COLOR IM_COL32(0x80, 0x80, 0x80, 0xFF)

#define TOWER_PANEL_UPGRADE_TAB_WIDTH 12
#define IMGUI_SET_CURSOR_POS_X(x) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x)
#define IMGUI_LINE_SPACING_HEIGHT 10
#define TOWER_PANEL_IMAGE_SIZE_BIG ImVec2(TOWER_PANEL_TEXT_SIZE_BIG, TOWER_PANEL_TEXT_SIZE_BIG)
#define TOWER_PANEL_IMAGE_SIZE_MEDIUM ImVec2(TOWER_PANEL_TEXT_SIZE_MEDIUM, TOWER_PANEL_TEXT_SIZE_MEDIUM)

Tower::Tower(const Tower& other)
	: Entity(other),
	mName(other.mName),
	mRange(other.mRange),
	mAttackSpeed(other.mAttackSpeed),
	mProjectileTemplate(other.mProjectileTemplate),
	mWidth(other.mWidth),
	mHeight(other.mHeight)
{
	mOwner = nullptr;
	mTarget = nullptr;
	mSelected = false;
}

Tower::Tower(Projectile* projectileTemplate)
	: Entity(), mProjectileTemplate(projectileTemplate)
{
	mOwner = nullptr;
	mTarget = nullptr;
	mSelected = false;
}

Tower::Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate)
	: Entity(pixelPosition), mRange(range), mAttackSpeed(attackSpeed), mProjectileTemplate(projectileTemplate)
{
	mCustomUpgradeLevel = 0;

	mOwner = nullptr;
	mTarget = nullptr;
	mSelected = false;
}

void Tower::Shoot(const Projectile& projTemplate)
{
	
}

void Tower::DrawRange(ImU32 color) const
{
	Globals::gDrawList->AddCircle(
		ImVec2(GetPixelPosition().x + GetWidth() * GRID_SQUARE_SIZE / 2, GetPixelPosition().y + GetHeight() * GRID_SQUARE_SIZE / 2),
		GetRange() * GRID_SQUARE_SIZE, TOWER_RANGE_OUTLINE_COLOR, 0, 2.f);
	Globals::gDrawList->AddCircleFilled(
		ImVec2(GetPixelPosition().x + GetWidth() * GRID_SQUARE_SIZE / 2, GetPixelPosition().y + GetHeight() * GRID_SQUARE_SIZE / 2),
		GetRange() * GRID_SQUARE_SIZE, color);
}

void Tower::OnUpdate()
{
	// Update pixel position from tile position
	SnapToGrid();

	// Check for selection
	if (Globals::gIO->MouseClicked[ImGuiMouseButton_Left] && !Globals::gIO->WantCaptureMouse)
	{
		ImVec2 mouseClickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];
		// If the tower is being clicked
		int32_t pixelX = mouseClickedPos.x, pixelY = mouseClickedPos.y;
		uint8_t tileX, tileY;
		Globals::gGame->GetPlayField()->GetGridPositionFromPixels(pixelX, pixelY, tileX, tileY);

		mSelected = tileX == GetTilePosition().x && tileY == GetTilePosition().y && !mSelected;
	}
}

void Tower::IncrementGenericUpgrade(uint8_t upgrade)
{
	mOwner->DecreaseMoney(GetGenericUpgradeCost(upgrade));
	mGenericUpgradeLevels[upgrade]++;
}

void Tower::DisplayTowerUpgrade(uint8_t upgrade, std::string name)
{
	IMGUI_SET_CURSOR_POS_X(TOWER_PANEL_UPGRADE_TAB_WIDTH);

	bool notEnoughMoney = mOwner->GetMoney() < GetGenericUpgradeCost(upgrade);
	if (notEnoughMoney)
		ImGui::BeginDisabled();

	if (ImGui::ImageButton(("upgrade" + name + "Button").c_str(), Globals::gResources->GetTexture("ui\\upgrade_icon")->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM))
		IncrementGenericUpgrade(upgrade);

	if (notEnoughMoney)
		ImGui::EndDisabled();

	ImGui::SameLine();
	ImGui::Text(name.c_str());
	ImGui::SameLine();
	switch (upgrade)
	{
		case 0:
			ImGui::Text("%d", GetDamage());
			break;
		case 1:
			ImGui::Text("%.2f", GetAttackSpeed());
			break;
		case 2:
			ImGui::Text("%.2f", GetRange());
			break;
	}
}

void Tower::OnRender()
{
	if (mSelected)
		DrawRange();

	const ImVec2 topLeft = GetPixelPosition();
	const ImVec2 bottomRight = ImVec2(topLeft.x + GetWidth() * GRID_SQUARE_SIZE, topLeft.y + GetHeight() * GRID_SQUARE_SIZE);
	Globals::gDrawList->AddImage(GetTexture()->id, topLeft, bottomRight);

	if (mSelected)
	{
		// Selection overlay
		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_SELECTION_OVERLAY_COLOR);
		Globals::gDrawList->AddRect(topLeft, bottomRight, TOWER_SELECTION_OUTLINE_COLOR, 0, 3.f);

		// Tower panel
		ImVec2 panelPosition(Globals::gWindowX + Globals::gWindowWidth - TOWER_PANEL_WIDTH - TOWER_PANEL_OFFSET_X, Globals::gGridY);

		// Draw stats
		ImGui::SetNextWindowPos(panelPosition);
		ImGui::SetNextWindowSize(ImVec2(TOWER_PANEL_WIDTH + TOWER_PANEL_OFFSET_X, Globals::gWindowHeight - TOWER_BAR_HEIGHT - GRID_OFFSET_Y));
		ImGui::Begin("Tower panel", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		ImDrawList* dl = ImGui::GetWindowDrawList();
		ImGui::PushFont(Globals::gFontBig);

		// TODO: Center tower name
        //ImGui::SetCursorPosX(ImGui::CalcTextSize(mName.c_str()).x);
		ImGui::Text(mName.c_str());

		// Tower image
		ImGui::Dummy(ImVec2(1, TOWER_PANEL_TEXT_SIZE_MEDIUM));
		ImGui::Image(GetTexture()->id, ImVec2(TOWER_PANEL_WIDTH, TOWER_PANEL_WIDTH));

		// Tower upgrades
		ImGui::Dummy(ImVec2(1, TOWER_PANEL_TEXT_SIZE_MEDIUM));
		ImGui::PushFont(Globals::gFontMedium);
		ImVec2 cursorPos = ImGui::GetCursorPos();
		cursorPos.x += panelPosition.x;
		cursorPos.y += panelPosition.y - 2; // -2 for offset
		dl->AddLine(cursorPos, ImVec2(cursorPos.x, cursorPos.y + (TOWER_PANEL_TEXT_SIZE_MEDIUM + IMGUI_LINE_SPACING_HEIGHT) * 3), IM_COL32(0xFF, 0xFF, 0x0, 0xB0), 4);

		DisplayTowerUpgrade(0, "Damage");
		DisplayTowerUpgrade(1, "Attack speed");
		DisplayTowerUpgrade(2, "Range");

		ImGui::Dummy(ImVec2(1, TOWER_PANEL_TEXT_SIZE_MEDIUM));
		cursorPos = ImGui::GetCursorPos();
		cursorPos.x += panelPosition.x;
		cursorPos.y += panelPosition.y - 2; // -2 for offset
		dl->AddLine(cursorPos, ImVec2(cursorPos.x, cursorPos.y + (TOWER_PANEL_TEXT_SIZE_MEDIUM + IMGUI_LINE_SPACING_HEIGHT) * 1), IM_COL32(0xFF, 0xB0, 0x0, 0xD0), 4);

		IMGUI_SET_CURSOR_POS_X(TOWER_PANEL_UPGRADE_TAB_WIDTH);
		if (ImGui::ImageButton("upgradeCustomButton", Globals::gResources->GetTexture("ui\\upgrade_icon")->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM))
		{
			std::cout << "Custom upgrade" << std::endl;
		}
		ImGui::SameLine();
		ImGui::Text("Custom");

		ImGui::PopFont();
		
		// Tower stats
		ImGui::Dummy(ImVec2(1, TOWER_PANEL_TEXT_SIZE_BIG));
		ImGui::Text("%d", mKillCount);
		ImGui::SameLine();
		ImGui::Image(Globals::gResources->GetTexture("ui\\kill_icon")->id, TOWER_PANEL_IMAGE_SIZE_BIG);

		ImGui::Text("%d", mDamageDealt);
		ImGui::SameLine();
		ImGui::Image(Globals::gResources->GetTexture("ui\\damage_icon")->id, TOWER_PANEL_IMAGE_SIZE_BIG);

		ImGui::Text("%d", mMoneyGenerated);
		ImGui::SameLine();
		ImGui::Image(Globals::gResources->GetTexture("ui\\money_icon")->id, TOWER_PANEL_IMAGE_SIZE_BIG);

		ImGui::PopFont();
		ImGui::End();
	}
}
