#include "enemies_data.hpp"

const EnemyInfo sEnemyInfo[EnemyId::ENEMY_COUNT] = {
	{
		.health = 50,
		.damage = 1,
		.moneyDrop = 15,
		.speed = EnemySpeed::NORMAL,

		.description = "A simple ant, average in everything",
		.ability = nullptr
	},
	{
		.health = 150,
		.damage = 2,
		.moneyDrop = 20,
		.speed = EnemySpeed::SLOW,

		.description = "A threatening mite, more dangerous than an ant",
		.ability = nullptr
	},
	{
		.health = 70,
		.damage = 2,
		.moneyDrop = 30,
		.speed = EnemySpeed::FAST,

		.description = "A gold and shiny scarab, moves much more quickly than anything else",
		.ability = nullptr
	},
	{
		.health = 150,
		.damage = 3,
		.moneyDrop = 40,
		.speed = EnemySpeed::FAST,

		.description = "A dangerous ladybug that doesn't like getting hit",
		.ability = "The less health it has, the quicker it moves"
	},
	{
		.health = 50,
		.damage = 1,
		.moneyDrop = 20,
		.speed = EnemySpeed::VERY_FAST,

		.description = "A more dangerous ant, it moves at tremendous speeds",
		.ability = nullptr
	},
	{
		.health = 50000,
		.damage = UINT16_MAX,
		.moneyDrop = 500,
		.speed = EnemySpeed::VERY_SLOW,

		.description = "The ant's nest and home, your strongest adversary",
		.ability = "Spawns a never ending stream of ants\nRegularly spawns weaker version of normal and red ants"
	},
	{
		.health = 1000,
		.damage = 10,
		.moneyDrop = 100,
		.speed = EnemySpeed::VERY_SLOW,

		.description = "A trilobite that's somehow on the ant's side.\nIts hard shell makes it a very resistant oppenent",
		.ability = nullptr
	},
	{
		.health = 250,
		.damage = 1,
		.moneyDrop = 75,
		.speed = EnemySpeed::NORMAL,

		.description = "A firefly, here to support its comrades",
		.ability = "Heals nearby enemies or itself"
	}
};
