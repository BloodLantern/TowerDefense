#include "tower.hpp"

#include "globals.hpp"

#include <GLFW/glfw3.h>

#define TOWER_RANGE_OUTLINE_COLOR IM_COL32(0x20, 0x20, 0x20, 0xA0)
#define TOWER_SELECTION_OVERLAY_COLOR IM_COL32(0x80, 0x80, 0x80, 0x80)
#define TOWER_SELECTION_OUTLINE_COLOR IM_COL32(0xFF, 0xFF, 0xFF, 0xFF)

#define TOWER_PANEL_OFFSET_X 17
#define TOWER_PANEL_WIDTH 7 * GRID_SQUARE_SIZE
#define TOWER_PANEL_OUTLINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)
#define TOWER_PANEL_BACKGROUND_COLOR IM_COL32(0x80, 0x80, 0x80, 0xFF)

#define IMGUI_SET_CURSOR_POS_X(x) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (x))
#define IMGUI_SET_CURSOR_POS_Y(y) ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (y))
#define IMGUI_IMAGE_BUTTON_OFFSET_X 8
#define IMGUI_IMAGE_BUTTON_OFFSET_Y 6

#define TOWER_PANEL_UPGRADE_TAB_WIDTH 12
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

void Tower::Shoot(Projectile* projTemplate)
{
	projTemplate->SetDamage(GetDamage());
	//projTemplate->SetTarget(mTarget);
	Point2 pixelPosition(GetPixelPosition().x + mWidth, GetPixelPosition().y + mHeight);
	projTemplate->SetPixelPosition(pixelPosition);
	projTemplate->SetVelocity(Vector2(pixelPosition, mTarget->GetPixelPosition()).Normalize() * 60);
	projTemplate->SetOwner(this);

	Globals::gGame->projectiles.push_back(projTemplate);
}

void Tower::DrawRange(ImU32 color) const
{
	Globals::gDrawList->AddCircle(
		ImVec2(Globals::gGridX + GetPixelPosition().x + GetWidth() * GRID_SQUARE_SIZE / 2, Globals::gGridY + GetPixelPosition().y + GetHeight() * GRID_SQUARE_SIZE / 2),
		GetRange() * GRID_SQUARE_SIZE, TOWER_RANGE_OUTLINE_COLOR, 0, 2.f);
	Globals::gDrawList->AddCircleFilled(
		ImVec2(Globals::gGridX + GetPixelPosition().x + GetWidth() * GRID_SQUARE_SIZE / 2, Globals::gGridY + GetPixelPosition().y + GetHeight() * GRID_SQUARE_SIZE / 2),
		GetRange() * GRID_SQUARE_SIZE, color);
}

void Tower::OnUpdate()
{
	// Update pixel position from tile position
	SnapToGrid();

	// Check for selection
	HandleSelection();

	// Check whether the tower is able to shoot
	HandleShoot();

	// Update attack cooldown
	mTimeSinceLastAttack += Globals::gGame->GetPlayingSpeedDeltaTime();
}

void Tower::IncrementGenericUpgrade(GenericUpgradeType upgrade)
{
	uint32_t cost = GetGenericUpgradeCost(upgrade);
	mOwner->DecreaseMoney(cost);
	mMoneyInvested += cost;
	mGenericUpgradeLevels[upgrade]++;
}

void Tower::HandleSelection()
{
	if (Globals::gIO->MouseClicked[ImGuiMouseButton_Left] && !Globals::gIO->WantCaptureMouse)
	{
		ImVec2 mouseClickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];
		// If the tower is being clicked
		int32_t pixelX = mouseClickedPos.x, pixelY = mouseClickedPos.y;
		uint8_t tileX, tileY;
		Globals::gGame->GetPlayField()->GetGridPositionFromPixels(pixelX - Globals::gGridX, pixelY - Globals::gGridY, tileX, tileY);

		mSelected = tileX == GetTilePosition().x && tileY == GetTilePosition().y && !mSelected;
	}
}

void Tower::HandleShoot()
{
	// If the tower can attack
	if (mTimeSinceLastAttack > 1 / GetAttackSpeed())
	{
		if (mTarget)
		{
			// If the target is too far away
			if (mTarget->toDelete || Vector2(GetPixelPosition(), mTarget->GetPixelPosition()).GetNorm() > GetRange() * GRID_SQUARE_SIZE)
				mTarget = nullptr;
		}
		
		// If the tower doesn't have a target, try to find one
		if (!mTarget)
			for (std::vector<Enemy*>::iterator it = Globals::gGame->enemies.begin(); it != Globals::gGame->enemies.end(); ++it)
				if (Vector2(GetPixelPosition(), (*it)->GetPixelPosition()).GetNorm() <= GetRange() * GRID_SQUARE_SIZE)
				{
					mTarget = *it;
					break;
				}

		if (mTarget)
		{
			mTimeSinceLastAttack = 0;
			Shoot(new Projectile(*mProjectileTemplate));
		}
	}
}

void Tower::HandlePanel(const ImVec2& topLeft, const ImVec2& bottomRight)
{
	// Tower panel
	ImVec2 panelPosition(Globals::gWindowX + Globals::gWindowWidth - TOWER_PANEL_WIDTH - TOWER_PANEL_OFFSET_X, Globals::gGridY);

	ImGui::SetNextWindowPos(panelPosition);
	ImGui::SetNextWindowSize(ImVec2(TOWER_PANEL_WIDTH + TOWER_PANEL_OFFSET_X, Globals::gWindowHeight - TOWER_BAR_HEIGHT - GRID_OFFSET_Y));
	ImGui::Begin("Tower panel", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	ImDrawList* dl = ImGui::GetWindowDrawList();

	// Draw name
	ImGui::PushFont(Globals::gFontBig);
	ImGui::SetCursorPosX((TOWER_PANEL_WIDTH - ImGui::CalcTextSize(mName.c_str()).x) * 0.5f);
	ImGui::Text(mName.c_str());
	ImGui::PopFont();

	// Draw image
	ImGui::Dummy(ImVec2(1, TOWER_PANEL_TEXT_SIZE_MEDIUM));
	ImGui::Image(GetTexture()->id, ImVec2(TOWER_PANEL_WIDTH, TOWER_PANEL_WIDTH));

	// Draw upgrades
	ImGui::Dummy(ImVec2(1, TOWER_PANEL_TEXT_SIZE_MEDIUM));
	ImGui::PushFont(Globals::gFontMedium);
	DrawUpgrades(panelPosition, dl);
	
	// Draw stats
	DrawStats();

	// Draw selling button
	char buffer[25];
	sprintf_s(buffer, "Sell for %d$", GetSellingPrice());
	ImGui::SetCursorPosX(TOWER_PANEL_WIDTH - ImGui::CalcTextSize(buffer).x);
	
	ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(0xE3 / 255.f, 0x14 / 255.f, 0x02 / 255.f, 0xDD / 255.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(0xCC / 255.f, 0x04 / 255.f, 0x00 / 255.f, 0xE0 / 255.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(0xA3 / 255.f, 0x00 / 255.f, 0x00 / 255.f, 0xE0 / 255.f));
	
	if (ImGui::Button(buffer))
	{
		mOwner->IncreaseMoney(GetSellingPrice());

		PlayField* pf = Globals::gGame->GetPlayField();
		Point2 pos = GetTilePosition();
		// TODO replace by original tile
		for (int32_t x = 0; x < mWidth; x++)
		{
			for (int32_t y = 0; y < mHeight; y++)
			{
				pf->SetClipdataTile(pos.x + x, pos.y + y, CLIPDATA_TYPE_EMPTY);
			}
		}

		toDelete = true;
	}

	ImGui::PopStyleColor(3);

	ImGui::PopFont();
	ImGui::End();
}

void Tower::DrawUpgrades(const ImVec2& panelPosition, ImDrawList* dl)
{
	ImVec2 cursorPos = ImGui::GetCursorPos();
	cursorPos.x += panelPosition.x;
	cursorPos.y += panelPosition.y - 2; // -2 for offset
	dl->AddLine(cursorPos, ImVec2(cursorPos.x, cursorPos.y + (TOWER_PANEL_TEXT_SIZE_MEDIUM + IMGUI_LINE_SPACING_HEIGHT) * 3), IM_COL32(0xFF, 0xFF, 0x0, 0xB0), 4);

	DisplayTowerUpgrade(GenericUpgradeType::DAMAGE);
	DisplayTowerUpgrade(GenericUpgradeType::ATTACK_SPEED);
	DisplayTowerUpgrade(GenericUpgradeType::RANGE);

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
}

void Tower::DrawStats()
{
	ImGui::Dummy(ImVec2(1, TOWER_PANEL_TEXT_SIZE_MEDIUM));
	ImGui::Image(Globals::gResources->GetTexture("ui\\kill_icon")->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
	AddTooltip("Kill count");
	ImGui::SameLine();
	ImGui::Text("%d", mKillCount);

	ImGui::Image(Globals::gResources->GetTexture("ui\\damage_icon")->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
	AddTooltip("Damage dealt");
	ImGui::SameLine();
	ImGui::Text("%d", mDamageDealt);

	ImGui::Image(Globals::gResources->GetTexture("ui\\money_icon")->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
	AddTooltip("Money generated");
	ImGui::SameLine();
	ImGui::Text("%d", mMoneyGenerated);
}

void Tower::DisplayTowerUpgrade(GenericUpgradeType upgrade)
{
	IMGUI_SET_CURSOR_POS_X(TOWER_PANEL_UPGRADE_TAB_WIDTH);

	// If the upgrade level is less than 10, show the button
	if (mGenericUpgradeLevels[upgrade] < TOWER_UPGRADE_GENERIC_LEVEL_MAX)
	{

		bool canUpgrade = mOwner->GetMoney() >= GetGenericUpgradeCost(upgrade);
		if (!canUpgrade)
			ImGui::BeginDisabled();

		if (ImGui::ImageButton(("upgrade" + std::to_string(upgrade) + "Button").c_str(), Globals::gResources->GetTexture("ui\\upgrade_icon")->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM))
			IncrementGenericUpgrade(upgrade);

		if (!canUpgrade)
			ImGui::EndDisabled();
	}
	else
	{
		ImGui::Dummy(ImVec2(TOWER_PANEL_TEXT_SIZE_MEDIUM + IMGUI_IMAGE_BUTTON_OFFSET_X, TOWER_PANEL_TEXT_SIZE_MEDIUM + IMGUI_IMAGE_BUTTON_OFFSET_Y));
	}

	ImGui::SameLine();
	switch (upgrade)
	{
		case GenericUpgradeType::DAMAGE:
		    IMGUI_SET_CURSOR_POS_Y(IMGUI_IMAGE_BUTTON_OFFSET_Y / 2);
			ImGui::Image(Globals::gResources->GetTexture("ui\\generic_upgrade_attack_damage_icon")->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
			AddTooltip("Attack damage");

			ImGui::SameLine();
			
		    IMGUI_SET_CURSOR_POS_Y(IMGUI_IMAGE_BUTTON_OFFSET_Y / 2);
			ImGui::Text("%d", GetDamage());
			break;

		case GenericUpgradeType::ATTACK_SPEED:
		    IMGUI_SET_CURSOR_POS_Y(IMGUI_IMAGE_BUTTON_OFFSET_Y / 2);
			ImGui::Image(Globals::gResources->GetTexture("ui\\generic_upgrade_attack_speed_icon")->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
			AddTooltip("Attack speed");

			ImGui::SameLine();

		    IMGUI_SET_CURSOR_POS_Y(IMGUI_IMAGE_BUTTON_OFFSET_Y / 2);
			ImGui::Text("%.2f", GetAttackSpeed());
			break;

		case GenericUpgradeType::RANGE:
		    IMGUI_SET_CURSOR_POS_Y(IMGUI_IMAGE_BUTTON_OFFSET_Y / 2);
			ImGui::Image(Globals::gResources->GetTexture("ui\\generic_upgrade_range_icon")->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
			AddTooltip("Range");

			ImGui::SameLine();

		    IMGUI_SET_CURSOR_POS_Y(IMGUI_IMAGE_BUTTON_OFFSET_Y / 2);
			ImGui::Text("%.2f", GetRange());
			break;
	}
}

void Tower::AddTooltip(const char* text)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
	{
		ImGui::SetTooltip(text);
	}
}

void Tower::OnRender()
{
	// Draw range if selected
	if (mSelected)
		DrawRange();

    // Draw tower texture
	const ImVec2 topLeft(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);
	const ImVec2 bottomRight = ImVec2(topLeft.x + GetWidth() * GRID_SQUARE_SIZE, topLeft.y + GetHeight() * GRID_SQUARE_SIZE);
	Globals::gDrawList->AddImage(GetTexture()->id, topLeft, bottomRight);

	// Draw upgrade/stats panel if selected
	if (mSelected)
	{
		// Selection overlay
		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_SELECTION_OVERLAY_COLOR);
		Globals::gDrawList->AddRect(topLeft, bottomRight, TOWER_SELECTION_OUTLINE_COLOR, 0, 3.f);

		HandlePanel(topLeft, bottomRight);
	}
}
