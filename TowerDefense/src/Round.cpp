#include "round.hpp"
#include "globals.hpp"
#include "imgui_utils.hpp"
#include <assert.h>


const RoundInfo* Round::mRoundInfo;
uint32_t Round::mCurrentCommand;

float_t Round::mTimer;
bool Round::mEnded;

Texture mTempTex;

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
			// TODO Spawn appropriate enemy

			int32_t screenX;
			int32_t screenY;
			Globals::gGame->GetPlayField()->GetPixelPositionFromGrid(0, 7, screenX, screenY);

			{
				Enemy* e = new Enemy(Point2(screenX, screenY + GRID_SQUARE_SIZE / 2), 0, 1, 0);
				mTempTex = ImGuiUtils::LoadTexture("assets/ant_alpha.png");
				e->SetTexture(&mTempTex);
				e->SetScale(.1f);

				Globals::gGame->mEnemies.push_back(e);
			}

			AdvanceRound();
			break;

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

		case ROUND_COMMAND_END:
			// Set ended
			mEnded = true;
			break;
	}
}
