#include "tower.hpp"

#include "globals.hpp"

#include <GLFW/glfw3.h>

#define TOWER_RANGE_OUTLINE_COLOR IM_COL32(0x20, 0x20, 0x20, 0xA0)
#define TOWER_SELECTION_OVERLAY_COLOR IM_COL32(0x80, 0x80, 0x80, 0x80)
#define TOWER_SELECTION_OUTLINE_COLOR IM_COL32(0xFF, 0xFF, 0xFF, 0xFF)

#define TOWER_PANEL_IMAGE_SIZE_MULTIPLIER 5
#define TOWER_PANEL_TEXT_SIZE_MULTIPLIER 1.f
#define TOWER_PANEL_WIDTH TOWER_PANEL_IMAGE_SIZE_MULTIPLIER * GRID_SQUARE_SIZE
#define TOWER_PANEL_OUTLINE_COLOR IM_COL32(0x0, 0x0, 0x0, 0x80)
#define TOWER_PANEL_BACKGROUND_COLOR IM_COL32(0x80, 0x80, 0x80, 0xFF)

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
	mGenericUpgradeLevel = 0;

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

		mSelected = tileX == GetTilePosition().x && tileY == GetTilePosition().y;
	}
}

//void ImGuiCallback_NearestInterpolation(const ImDrawList*, const ImDrawCmd*)
//{
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//}

void Tower::OnRender()
{
	if (mSelected)
		DrawRange();

	//ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, ImVec2(GetScale(), GetScale()), GetRotation());
	const ImVec2 topLeft = GetPixelPosition();
	const ImVec2 bottomRight = ImVec2(topLeft.x + GetWidth() * GRID_SQUARE_SIZE, topLeft.y + GetHeight() * GRID_SQUARE_SIZE);
	Globals::gDrawList->AddImage(GetTexture()->id, topLeft, bottomRight);

	if (mSelected)
	{
		// Selection overlay
		Globals::gDrawList->AddRectFilled(topLeft, bottomRight, TOWER_SELECTION_OVERLAY_COLOR);
		Globals::gDrawList->AddRect(topLeft, bottomRight, TOWER_SELECTION_OUTLINE_COLOR, 0, 3.f);

		// Tower panel
		ImVec2 panelPosition(Globals::gWindowX + Globals::gWindowWidth - TOWER_PANEL_WIDTH, Globals::gGridY);

		// Draw stats
		ImGui::SetNextWindowPos(panelPosition);
		ImGui::SetNextWindowSize(ImVec2(TOWER_PANEL_WIDTH, Globals::gWindowHeight - TOWER_BAR_HEIGHT - GRID_OFFSET_Y));
		ImGui::Begin("Tower panel", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		//ImDrawList* dl = ImGui::GetForegroundDrawList();
		// For pixel arts upscale
		//dl->AddCallback(ImGuiCallback_NearestInterpolation, nullptr);

        ImGui::SetCursorPosX(ImGui::CalcTextSize(mName.c_str()).x / TOWER_PANEL_TEXT_SIZE_MULTIPLIER);
		ImGui::SetWindowFontScale(TOWER_PANEL_TEXT_SIZE_MULTIPLIER);
		ImGui::Text(mName.c_str());
		//dl->AddText(ImVec2(ImGui::CalcTextSize(mName.c_str()).x, 50), IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), mName.c_str());

		//Texture* texture = GetTexture();
		//ImVec2 towerTexturePos(panelPosition.x, panelPosition.y + 20);
		//dl->AddImage(texture->id, towerTexturePos, ImVec2(towerTexturePos.x + texture->width * TOWER_PANEL_IMAGE_SIZE_MULTIPLIER,
		//	towerTexturePos.y + texture->height * TOWER_PANEL_IMAGE_SIZE_MULTIPLIER));
		ImGui::Dummy(ImVec2(1, 100));
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "%d kills", mKillCount);
		ImGui::TextColored(ImVec4(1, 0.5, 0, 1), "%d", mDamageDealt);
		//ImGui::Image();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d$", mMoneyGenerated);

		//dl->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
		ImGui::End();
	}
}
