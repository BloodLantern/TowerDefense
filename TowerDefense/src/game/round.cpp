#include "round.hpp"
#include "globals.hpp"
#include "imgui_utils.hpp"

#include "ant_enemy.hpp"
#include "mite_enemy.hpp"
#include "gold_scarab_enemy.hpp"
#include "ladybug_enemy.hpp"
#include "red_ant_enemy.hpp"
#include "ant_colony_enemy.hpp"
#include "trilobyte_enemy.hpp"
#include "firefly_enemy.hpp"

#include "enemies_data.hpp"

#include <assert.h>


const RoundInfo* Round::mRoundInfo;
uint32_t Round::mCurrentCommand;

float_t Round::mTimer;
bool Round::mEnded;

std::stack<Round::LoopInfo> Round::mLoopStack;


void Round::StartRound(const RoundInfo* const info)
{
	assert(info != nullptr && "Round info should never be null");

	mRoundInfo = info;

	mCurrentCommand = 0;
	mTimer = FLT_MAX;

	mEnded = false;
}

void Round::AdvanceRound()
{
	mCurrentCommand++;
	mTimer = FLT_MAX;
}

bool Round::HasEnded()
{
	return mEnded;
}

void Round::GrantEndRoundMoney()
{
	Globals::gGame->GetPlayer()->IncreaseMoney(mRoundInfo[mCurrentCommand].data.dataInt);
}

void Round::OnUpdate()
{
	// End only once
	if (mEnded)
		return;

	RoundCommand command = mRoundInfo[mCurrentCommand].command;

	// Execute command
	switch (command)
	{
		case ROUND_COMMAND_SPAWN_ENEMY:
		{
			int32_t screenX;
			int32_t screenY;
			Point2 position;

			PlayField* pf = Globals::gGame->GetPlayField();
			uint8_t tileX = pf->GetPathNodes()[0].x;
			uint8_t tileY = pf->GetPathNodes()[0].y;

			pf->GetPixelPositionFromGrid(tileX, tileY, screenX, screenY);

			Enemy* e;
			Point2 pos(screenX, screenY + GRID_SQUARE_SIZE / 2);
			switch (mRoundInfo[mCurrentCommand].data.dataInt)
			{
				case EnemyId::ANT:
					e = new AntEnemy(pos);
					break;

				case EnemyId::MITE:
					e = new MiteEnemy(pos);
					break;

				case EnemyId::GOLD_SCARAB:
					e = new GoldScarabEnemy(pos);
					break;

				case EnemyId::LADYBUG:
					e = new LadybugEnemy(pos);
					break;
					
				case EnemyId::RED_ANT:
					e = new RedAntEnemy(pos);
					break;
					
				case EnemyId::ANT_COLONY:
					e = new AntColonyEnemy(pos);
					break;

				case EnemyId::TRILOBITE:
					e = new TrilobiteEnemy(pos);
					break;

				case EnemyId::FIREFLY:
					e = new FireflyEnemy(pos);
					break;
			}

			Globals::gGame->enemies.push_back(e);
			AdvanceRound();
			break;
		}

		case ROUND_COMMAND_COOLDOWN:
			if (mTimer == FLT_MAX)
			{
				// Set cooldown
				mTimer = mRoundInfo[mCurrentCommand].data.dataFloat;
				break;
			}

			// Update
			mTimer -= Globals::gGame->GetPlayingSpeedDeltaTime();

			if (mTimer < 0)
			{
				AdvanceRound();
				break;
			}
			break;

		case ROUND_COMMAND_GFX_EFFECT:
			AdvanceRound();
			break;

		case ROUND_COMMAND_PLAY_SOUND:
			// PlaySound(mRoundInfo[mCurrentCommand].data.dataInt);
			AdvanceRound();
			break;

		case ROUND_COMMAND_LOOP_START:
			mLoopStack.push(LoopInfo(mCurrentCommand, mRoundInfo[mCurrentCommand].data.dataInt));

			AdvanceRound();
			break;

		case ROUND_COMMAND_LOOP_END:
		{
			LoopInfo& info = mLoopStack.top();

			if (info.length == 0)
			{
				mLoopStack.pop();
				AdvanceRound();
				break;
			}

			info.length--;

			if (info.length == 0)
			{
				mLoopStack.pop();
				AdvanceRound();
				break;
			}

			mCurrentCommand = info.startCommandID;

			AdvanceRound();
			break;
		}

		case ROUND_COMMAND_END:
			// Set ended
			mEnded = true;
			break;
	}
}
