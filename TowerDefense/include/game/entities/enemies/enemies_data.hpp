#pragma once

#include <stdint.h>
#include "imgui_utils.hpp"

enum class EnemySpeed
{
	VERY_SLOW,
	SLOW,
	NORMAL,
	FAST,
	VERY_FAST
};

struct EnemyInfo
{
	uint32_t health;
	uint16_t damage;
	uint32_t moneyDrop;

	EnemySpeed speed;

	const char* const description;
	const char* const ability;
};

enum EnemyId
{
	ANT,
	MITE,
	GOLD_SCARAB,
	LADYBUG,
	RED_ANT,
	ANT_COLONY,
	TRILOBITE,
	FIREFLY,

	ENEMY_COUNT
};

inline EnemyId& operator++(EnemyId& a, int)
{
	uint32_t val = static_cast<uint32_t>(a) + 1;
	val %= ENEMY_COUNT;

	return a = static_cast<EnemyId>(val);
}

inline EnemyId& operator--(EnemyId& a, int)
{
	int32_t val = static_cast<uint32_t>(a) - 1;
	if (val < 0)
		val = ENEMY_COUNT - 1;

	return a = static_cast<EnemyId>(val);
}

extern const EnemyInfo sEnemyInfo[EnemyId::ENEMY_COUNT];
