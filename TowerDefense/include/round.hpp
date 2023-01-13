#pragma once

#include <stdint.h>

enum RoundCommand
{
	ROUND_COMMAND_SPAWN_ENEMY,
	ROUND_COMMAND_COOLDOWN,
	ROUND_COMMAND_GFX_EFFECT,
	ROUND_COMMAND_PLAY_SOUND,
	ROUND_COMMAND_END
};

struct RoundInfo
{
	RoundCommand command;
	uint32_t data;

	RoundInfo(RoundCommand cmd, uint32_t d)
		: command(cmd), data(d)
	{ }
};

class Round
{
public:
	static void StartRound(const RoundInfo* const info);
	static void OnUpdate();

	static bool HasEnded();

private:
	static void AdvanceRound();

	static const RoundInfo* mRoundInfo;
	static uint32_t mCurrentCommand;

	static uint32_t mTimer;
	static bool mEnded;
};

