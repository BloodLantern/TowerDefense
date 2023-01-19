#pragma once

#include <stdint.h>
#include "enemies_data.hpp"
#include "scene.hpp"

class Bestiary
{
public:
	static void InitTextures();
	static Scene Update();

private:
	static EnemyId mCurrentEnemy;

	static const Texture* mTextures[EnemyId::ENEMY_COUNT];
};

