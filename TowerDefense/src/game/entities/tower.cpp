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

static const char* TOWER_PANEL_ATTACK_DAMAGE_TOOLTIP_TEXT = "Attack damage";
static const char* TOWER_PANEL_ATTACK_SPEED_TOOLTIP_TEXT = "Attack speed - number of attacks per second";
static const char* TOWER_PANEL_RANGE_TOOLTIP_TEXT = "Range - in tiles";
static const char* TOWER_PANEL_KILL_COUNT_TOOLTIP_TEXT = "Kill count";
static const char* TOWER_PANEL_DAMAGE_DEALT_TOOLTIP_TEXT = "Damage dealt";
static const char* TOWER_PANEL_MONEY_GENERATED_TOOLTIP_TEXT = "Money generated";

Texture* Tower::mUpgradeIconTexture = nullptr;
Texture* Tower::mKillIconTexture = nullptr;
Texture* Tower::mDamageIconTexture = nullptr;
Texture* Tower::mMoneyIconTexture = nullptr;
Texture* Tower::mGenericUpgradeAttackDamageIconTexture = nullptr;
Texture* Tower::mGenericUpgradeAtackSpeedIconTexture = nullptr;
Texture* Tower::mGenericUpgradeRangeIconTexture = nullptr;

Tower::Tower(Projectile* projectileTemplate, float_t attackSpeed, float_t range, std::string name, uint32_t cost, Texture* texture)
	: Entity(),
	mProjectileTemplate(projectileTemplate)
{
	InitStats(
		projectileTemplate->GetDamage(),
		attackSpeed,
		range,
		name,
		cost,
		texture
	);
}

void Tower::Shoot()
{
	Projectile* projTemplate = mProjectileTemplate->Clone();
	projTemplate->SetDamage(mDamage);
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
	// Check for selection
	HandleSelection();

	// Check whether the tower is able to shoot
	HandleShoot();

	// Update attack cooldown
	mTimeSinceLastAttack += Globals::gGame->GetPlayingSpeedDeltaTime();
}

void Tower::InitUITextures()
{
	Tower::mUpgradeIconTexture = Globals::gResources->GetTexture("ui\\upgrade_icon");
	Tower::mKillIconTexture = Globals::gResources->GetTexture("ui\\kill_icon");
	Tower::mDamageIconTexture = Globals::gResources->GetTexture("ui\\damage_icon");
	Tower::mMoneyIconTexture = Globals::gResources->GetTexture("ui\\money_icon");
	Tower::mGenericUpgradeAttackDamageIconTexture = Globals::gResources->GetTexture("ui\\generic_upgrade_attack_damage_icon");
	Tower::mGenericUpgradeAtackSpeedIconTexture = Globals::gResources->GetTexture("ui\\generic_upgrade_attack_speed_icon");
	Tower::mGenericUpgradeRangeIconTexture = Globals::gResources->GetTexture("ui\\generic_upgrade_range_icon");
}

void Tower::UpdateGeneric(GenericUpgradeType upgrade)
{
	switch (upgrade)
	{
		case GenericUpgradeType::DAMAGE:
		    UpdateDamage();
            break;
			
        case GenericUpgradeType::ATTACK_SPEED:
            UpdateAttackSpeed();
			break;
			
        case GenericUpgradeType::RANGE:
            UpdateRange();
            break;
	}
}

void Tower::UpdateDamage()
{
	mDamage = mStartDamage + mGenericUpgradeLevels[GenericUpgradeType::DAMAGE] * TOWER_UPGRADE_GENERIC_DAMAGE_MULTIPLIER;
}

void Tower::UpdateAttackSpeed()
{
	mAttackSpeed = mStartAttackSpeed + mGenericUpgradeLevels[GenericUpgradeType::ATTACK_SPEED] * TOWER_UPGRADE_GENERIC_ATTACK_SPEED_MULTIPLIER;
}

void Tower::UpdateRange()
{
	mRange = mStartRange + mGenericUpgradeLevels[GenericUpgradeType::RANGE] * TOWER_UPGRADE_GENERIC_RANGE_MULTIPLIER;
}

void Tower::IncrementGenericUpgrade(GenericUpgradeType upgrade)
{
	uint32_t cost = GetGenericUpgradeCost(upgrade);
	mOwner->DecreaseMoney(cost);
	mMoneyInvested += cost;
	mGenericUpgradeLevels[upgrade]++;
	UpdateGeneric(upgrade);
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

		Point2 tilePos = GetTilePosition();

		mSelected = !mSelected && tilePos.x <= tileX && tilePos.y <= tileY && tilePos.x + mWidth > tileX && tilePos.y + mHeight > tileY;
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
			if (mRotateTowardsEnemies)
				RotateTowardsTarget();
			Shoot();
		}
	}
}

void Tower::RotateTowardsTarget()
{
	Vector2 direction(GetPixelPosition(), mTarget->GetPixelPosition());

	mRotation = atan2(direction.y, direction.x);
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
	ImGui::Text("%s", mName.c_str());
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
	if (DrawSellingButton())
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

	ImGui::PopFont();
	ImGui::End();
}

bool Tower::DrawSellingButton()
{
	char buffer[25];
	sprintf_s(buffer, "Sell for $%d", GetSellingPrice());
	ImGui::SetCursorPosX(TOWER_PANEL_WIDTH - ImGui::CalcTextSize(buffer).x);
	
	ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(0xE3 / 255.f, 0x14 / 255.f, 0x02 / 255.f, 0xDD / 255.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(0xBC / 255.f, 0x00 / 255.f, 0x00 / 255.f, 0xE0 / 255.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(0x93 / 255.f, 0x00 / 255.f, 0x00 / 255.f, 0xE0 / 255.f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);
	
	bool clicked = ImGui::Button(buffer);

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);

	return clicked;
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
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);
	ImGui::BeginDisabled();
	if (ImGui::ImageButton("upgradeCustomButton", mUpgradeIconTexture->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM))
	{
		std::cout << "Custom upgrade" << std::endl;
	}
	ImGui::EndDisabled();
	ImGui::PopStyleVar();
	AddTooltip("Not yet implemented");
	ImGui::SameLine();
	ImGui::Text("Custom");
	AddTooltip("Not yet implemented");
}

bool Tower::DrawUpgradeButton(GenericUpgradeType upgrade)
{
	ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(0x1E / 255.f, 0xFF / 255.f, 0x3E / 255.f, 0xC0 / 255.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(0x1A / 255.f, 0xCF / 255.f, 0x33 / 255.f, 0xC0 / 255.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(0x16 / 255.f, 0xB0 / 255.f, 0x2B / 255.f, 0xC0 / 255.f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);

	bool clicked = ImGui::ImageButton(("upgrade" + std::to_string(upgrade) + "Button").c_str(), mUpgradeIconTexture->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
	
	ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);

	return clicked;
}

void Tower::InitStats(uint32_t damage, float_t attackSpeed, float_t range, std::string name, uint32_t cost, Texture * texture)
{
	mStartDamage = damage;
	mDamage = damage;
	mStartAttackSpeed = attackSpeed;
	mAttackSpeed = attackSpeed;
	mStartRange = range;
	mRange = range;
	mName = name;
    mCost = cost;
	mMoneyInvested = cost;
	mTexture = texture;
}

void Tower::DrawStats()
{
	ImGui::Dummy(ImVec2(1, TOWER_PANEL_TEXT_SIZE_MEDIUM));
	ImGui::Image(mKillIconTexture->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
	AddTooltip(TOWER_PANEL_KILL_COUNT_TOOLTIP_TEXT);
	ImGui::SameLine();
	ImGui::Text("%d", mKillCount);
	AddTooltip(TOWER_PANEL_KILL_COUNT_TOOLTIP_TEXT);

	ImGui::Image(mDamageIconTexture->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
	AddTooltip(TOWER_PANEL_DAMAGE_DEALT_TOOLTIP_TEXT);
	ImGui::SameLine();
	ImGui::Text("%d", mDamageDealt);
	AddTooltip(TOWER_PANEL_DAMAGE_DEALT_TOOLTIP_TEXT);

	ImGui::Image(mMoneyIconTexture->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
	AddTooltip(TOWER_PANEL_MONEY_GENERATED_TOOLTIP_TEXT);
	ImGui::SameLine();
	ImGui::Text("$%d", mMoneyGenerated);
	AddTooltip(TOWER_PANEL_MONEY_GENERATED_TOOLTIP_TEXT);
}

void Tower::DisplayTowerUpgrade(GenericUpgradeType upgrade)
{
	IMGUI_SET_CURSOR_POS_X(TOWER_PANEL_UPGRADE_TAB_WIDTH);
	IMGUI_SET_CURSOR_POS_Y(IMGUI_IMAGE_BUTTON_OFFSET_Y / 2);

	switch (upgrade)
	{
		case GenericUpgradeType::DAMAGE:
			ImGui::Image(mGenericUpgradeAttackDamageIconTexture->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
			AddTooltip(TOWER_PANEL_ATTACK_DAMAGE_TOOLTIP_TEXT);

			ImGui::SameLine();
			
			ImGui::Text("%-d", mDamage);
			AddTooltip(TOWER_PANEL_ATTACK_DAMAGE_TOOLTIP_TEXT);
			break;

		case GenericUpgradeType::ATTACK_SPEED:
		{
			ImGui::Image(mGenericUpgradeAtackSpeedIconTexture->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
			AddTooltip(TOWER_PANEL_ATTACK_SPEED_TOOLTIP_TEXT);

			ImGui::SameLine();
			
			if (mAttackSpeed >= 10.f)
				ImGui::Text("%-.1f", mAttackSpeed);
			else
				ImGui::Text("%-.2f", mAttackSpeed);
			AddTooltip(TOWER_PANEL_ATTACK_SPEED_TOOLTIP_TEXT);
			break;
		}

		case GenericUpgradeType::RANGE:
			ImGui::Image(mGenericUpgradeRangeIconTexture->id, TOWER_PANEL_IMAGE_SIZE_MEDIUM);
			AddTooltip(TOWER_PANEL_RANGE_TOOLTIP_TEXT);

			ImGui::SameLine();

			ImGui::Text("%-.2f", mRange);
			AddTooltip(TOWER_PANEL_RANGE_TOOLTIP_TEXT);
			break;
	}


	// If the upgrade level is less than 10, show the button
	if (mGenericUpgradeLevels[upgrade] < TOWER_UPGRADE_GENERIC_LEVEL_MAX)
	{
		ImGui::SameLine();
		ImGui::SetCursorPosX(100);
		IMGUI_SET_CURSOR_POS_Y(-IMGUI_IMAGE_BUTTON_OFFSET_Y / 2);

		bool canUpgrade = mOwner->GetMoney() >= GetGenericUpgradeCost(upgrade);
		if (!canUpgrade)
			ImGui::BeginDisabled();

		if (DrawUpgradeButton(upgrade))
			IncrementGenericUpgrade(upgrade);

		if (!canUpgrade)
		{
			ImGui::EndDisabled();

			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0xFF / 255.f, 0x00 / 255.f, 0x00 / 255.f, 0xFF / 255.f), "$%d", GetGenericUpgradeCost(upgrade));
		}
		else
        {
			ImGui::SameLine();
			ImGui::Text("$%d", GetGenericUpgradeCost(upgrade));
		}
	}
	else
	{
		IMGUI_SET_CURSOR_POS_Y(IMGUI_IMAGE_BUTTON_OFFSET_Y / 2);
	}

}

void Tower::AddTooltip(const char* text)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
		ImGui::SetTooltip("%s", text);
}

void Tower::OnRender()
{
	// Draw range if selected
	if (mSelected)
		DrawRange();

    // Draw tower texture
	const ImVec2 topLeft(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);
	const ImVec2 bottomRight = ImVec2(topLeft.x + GetWidth() * GRID_SQUARE_SIZE, topLeft.y + GetHeight() * GRID_SQUARE_SIZE);
	Draw(topLeft, bottomRight);

	// Draw upgrade/stats panel if selected
	if (mSelected)
	{
		// Selection overlay
		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_SELECTION_OVERLAY_COLOR);
		Globals::gDrawList->AddRect(topLeft, bottomRight, TOWER_SELECTION_OUTLINE_COLOR, 0, 3.f);

		HandlePanel(topLeft, bottomRight);
	}
}

void Tower::DrawTowerBarTexture()
{
	const ImVec2 topLeft(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);
	const ImVec2 bottomRight = ImVec2(topLeft.x + GetWidth() * GRID_SQUARE_SIZE, topLeft.y + GetHeight() * GRID_SQUARE_SIZE);
	Globals::gDrawList->AddImage(mTexture->id, topLeft, bottomRight);
}

void Tower::Draw(const ImVec2& topLeft, const ImVec2& bottomRight) const
{
	Globals::gDrawList->AddImage(mTexture->id, topLeft, bottomRight);
}