#include "bestiary.hpp"
#include "globals.hpp"

EnemyId Bestiary::mCurrentEnemy;

const Texture* Bestiary::mTextures[EnemyId::ENEMY_COUNT];

void Bestiary::InitTextures()
{
	Resources* r = Globals::gResources;

	mTextures[EnemyId::ANT] = r->GetTexture("enemies\\ant_alpha");
	mTextures[EnemyId::MITE] = r->GetTexture("enemies\\mite");
	mTextures[EnemyId::GOLD_SCARAB] = r->GetTexture("enemies\\gold_scarab");
	mTextures[EnemyId::LADYBUG] = r->GetTexture("enemies\\ladybug");
	mTextures[EnemyId::RED_ANT] = r->GetTexture("enemies\\ant_alpha");
	mTextures[EnemyId::ANT_COLONY] = r->GetTexture("enemies\\ant_colony");
	mTextures[EnemyId::TRILOBITE] = r->GetTexture("enemies\\trilobite");
	mTextures[EnemyId::FIREFLY] = r->GetTexture("enemies\\firefly");
}

Scene Bestiary::Update()
{
	Scene result = Scene::BESTIARY;

	const EnemyInfo& enemyInfo = sEnemyInfo[mCurrentEnemy];
	const Texture* tex = mTextures[mCurrentEnemy];

	ImGui::SetNextWindowPos(ImVec2(Globals::gGridX, Globals::gGridY));
	ImGui::SetNextWindowSize(ImVec2(Globals::gWindowWidth, Globals::gWindowHeight - GRID_OFFSET_Y));

	if (ImGui::Begin("##bestiary", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize))
	{
		ImGui::PushFont(Globals::gFontBig);
		ImGui::Dummy(ImVec2(1, 50));
		ImGui::SetCursorPosX(Globals::gWindowWidth / 2.f - ImGui::CalcTextSize("Bestiary").x / 2);
		ImGui::Text("Bestiary");
		ImGui::PopFont();

		ImGui::PushFont(Globals::gFontMedium);
		ImVec2 cursor(Globals::gWindowWidth / 2.f - tex->width / 4.f, ImGui::GetCursorPosY() + 50);

		ImGui::SetCursorPos(cursor);
		if (mCurrentEnemy == EnemyId::RED_ANT)
			ImGui::Image(tex->id, ImVec2(tex->width / 2.f, tex->height / 2.f),
				ImVec2(0, 0), ImVec2(1, 1), ImColor(0xFF, 0, 0, 0xFF));
		else
			ImGui::Image(tex->id, ImVec2(tex->width / 2.f, tex->height / 2.f));

		cursor.x -= 200;
		cursor.y += tex->height / 2.f + 50;
		ImGui::SetCursorPos(cursor);
		ImGui::Text("Health : %d", enemyInfo.health);
		
		cursor.y += 20;
		ImGui::SetCursorPos(cursor);
		ImGui::Text("Damage : %d", enemyInfo.damage);
		
		cursor.y += 20;
		ImGui::SetCursorPos(cursor);
		ImGui::Text("Money drop: %d", enemyInfo.moneyDrop);
		
		cursor.y += 20;
		ImGui::SetCursorPos(cursor);
		ImGui::Text("Description : %s", enemyInfo.description);
		
		cursor.y += 20;
		ImGui::SetCursorPos(cursor);
		if (enemyInfo.ability)
			ImGui::Text("Ability : %s", enemyInfo.ability);
		else
			ImGui::Text("No ability");

		const ImVec2 buttonSize(130, 50);

		cursor.x = Globals::gWindowWidth / 2.f - buttonSize.x / 2.f;
		cursor.y += 60;
		ImGui::SetCursorPos(cursor);
		if (ImGui::Button("Back", buttonSize))
			result = Scene::MAIN_MENU;

		cursor.y = Globals::gWindowHeight / 2.f - buttonSize.y / 2.f;
		cursor.x = 50;
		ImGui::SetCursorPos(cursor);
		if (ImGui::Button("< Previous", buttonSize))
			mCurrentEnemy--;

		cursor.x = Globals::gWindowWidth - buttonSize.x - 50;
		ImGui::SetCursorPos(cursor);
		if (ImGui::Button("Next >", buttonSize))
			mCurrentEnemy++;

		ImGui::PopFont();
	}

	ImGui::End();
	return result;
}
