#include "player.hpp"
#include "globals.hpp"
#include <limits.h>
#include <windows.h>
#include <Lmcons.h>

uint32_t Player::playerGlobalID;

Player::Player()
{
	TCHAR username[UNLEN + 1];
	DWORD usernameLen = UNLEN + 1;
	GetUserName(username, &usernameLen);
	std::wstring messageW(username);

	std::string usernameStr(messageW.begin(), messageW.end());

	mUsername = usernameStr;

	mPlayerID = Player::playerGlobalID++;
}

Player::Player(std::string username)
	: mUsername(username)
{
	mPlayerID = Player::playerGlobalID++;
}

Player::~Player()
{
	playerGlobalID--;

	for (std::vector<Tower*>::iterator it = mTowers.begin(); it != mTowers.end(); it++)
		delete *it;
}

void Player::DecreaseMoney(uint32_t amount)
{
	// Prevent underflow
	if (amount > mMoney)
        mMoney = 0;
    else
		mMoney -= amount;
}

void Player::IncreaseMoney(uint32_t amount)
{
	mMoney += amount;
}

void Player::Reset()
{
	mMoney = PLAYER_DEFAULT_MONEY;
}
