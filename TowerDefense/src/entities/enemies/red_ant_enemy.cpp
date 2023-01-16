#include "red_ant_enemy.hpp"
#include "globals.hpp"

RedAntEnemy::RedAntEnemy(Point2 pos)
	: Enemy(pos, 5, 1, 20)
{
	mTexture = Globals::gResources->GetTexture("enemies\\ant_alpha");
	SetScale(.1f);
	mSpeed = 3.5f;
}

RedAntEnemy::~RedAntEnemy()
{

}

void RedAntEnemy::OnRender()
{
	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);

	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, ImVec2(GetScale(), GetScale()), GetRotation(), IM_COL32(0xFF, 0x0, 0x0, 0xD0));

	Enemy::DrawHealthBar(pos);
}
