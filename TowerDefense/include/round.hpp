#pragma once

#include <stdint.h>
#include <stack>
#include <math.h>

enum RoundCommand : uint8_t
{
	ROUND_COMMAND_SPAWN_ENEMY,
	ROUND_COMMAND_COOLDOWN,
	ROUND_COMMAND_GFX_EFFECT,
	ROUND_COMMAND_PLAY_SOUND,
	ROUND_COMMAND_LOOP_START,
	ROUND_COMMAND_LOOP_END,
	ROUND_COMMAND_END
};

struct RoundInfo
{
	RoundCommand command;
	union
	{
		uint32_t dataInt;
		float_t dataFloat;
	} data;

	RoundInfo(RoundCommand cmd, uint32_t d)
		: command(cmd)
	{
		data.dataInt = d;
	}

	RoundInfo(RoundCommand cmd, float_t d)
		: command(cmd)
	{
		data.dataFloat = d;
	}
};

class Round
{
public:
	static void StartRound(const RoundInfo* const info);
	static void OnUpdate();

	static bool HasEnded();
	static void GrantEndRoundMoney();

private:
	struct LoopInfo
	{
		uint32_t startCommandID;
		uint32_t length;

		LoopInfo(uint32_t startID, uint32_t len)
			: startCommandID(startID), length(len)
		{ }
	};

	static void AdvanceRound();

	static const RoundInfo* mRoundInfo;
	static uint32_t mCurrentCommand;

	static float_t mTimer;
	static bool mEnded;

	static std::stack<LoopInfo> mLoopStack;
};

