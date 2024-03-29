#include "red_ant_enemy.hpp"
#include "enemies_data.hpp"
#include "globals.hpp"

RedAntEnemy::RedAntEnemy(Point2 pos)
	: Enemy(pos, sEnemyInfo[EnemyId::RED_ANT].health, sEnemyInfo[EnemyId::RED_ANT].damage, sEnemyInfo[EnemyId::RED_ANT].moneyDrop)
{
	mTexture = Globals::gResources->GetTexture("enemies\\ant_alpha");
	SetScale(.1f);
	mSpeed = 3.5f;
}

void RedAntEnemy::OnRender()
{
	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);

	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, mScale, mRotation, IM_COL32(0xFF, 0x0, 0x0, 0xD0));

	Enemy::DrawHealthBar(pos);
}
