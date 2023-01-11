#include "Round.h"
#include "Globals.h"
#include "imgui_utils.hpp"
#include <assert.h>


const RoundInfo* Round::mRoundInfo;
uint32_t Round::mCurrentCommand;

uint32_t Round::mTimer;
bool Round::mEnded;

Texture mTempTex;

void Round::StartRound(const RoundInfo* const info)
{
	assert(info != nullptr && "Round info should never be null");

	mRoundInfo = info;

	mCurrentCommand = 0;
	mTimer = UINT32_MAX;

	mEnded = false;
}

void Round::AdvanceRound()
{
	mCurrentCommand++;
	mTimer = UINT32_MAX;
}

bool Round::HasEnded()
{
	return mEnded;
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
				mTempTex = ImGuiUtils::LoadTexture("assets/ant.png");
				e->SetTexture(&mTempTex);
				e->SetScale(.1f);

				Globals::gGame->mEnemies.push_back(e);
			}

			AdvanceRound();
			break;

		case ROUND_COMMAND_COOLDOWN:
			if (mTimer == UINT32_MAX)
			{
				// Set cooldown
				mTimer = mRoundInfo[mCurrentCommand].data;
				break;
			}

			// Update
			mTimer--;

			if (mTimer == 0)
			{
				AdvanceRound();
				break;
			}
			break;

		case ROUND_COMMAND_GFX_EFFECT:
			AdvanceRound();
			break;

		case ROUND_COMMAND_PLAY_SOUND:
			// PlaySound(mRoundInfo[mCurrentCommand].data);
			AdvanceRound();
			break;

		case ROUND_COMMAND_END:
			// Reward end of round money
			Globals::gGame->GetPlayer()->IncreaseMoney(mRoundInfo[mCurrentCommand].data);
			mEnded = true;
			break;
	}
}
