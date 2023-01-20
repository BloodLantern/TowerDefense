#include "player.hpp"
#include "globals.hpp"
#include <limits.h>
#include <windows.h>
#include <Lmcons.h>

Player::Player()
{
	// Use this constructor only in solo
	mPlayerID = 0;

	TCHAR username[UNLEN + 1];
	DWORD usernameLen = UNLEN + 1;
	GetUserName(username, &usernameLen); 
	
	std::wstring messageW(username);

	std::string usernameStr(messageW.begin(), messageW.end());

	std::string trimUsername;

	for (size_t i = 0; i < usernameStr.size(); i++)
	{
		if (usernameStr[i] == '\0')
			break;

		trimUsername = trimUsername.append(std::string(1, usernameStr[i]));
	}

	mUsername = trimUsername;
	std::cout << "Created player (SOLO) " << mUsername << " with id " << mPlayerID << std::endl;
}

Player::Player(std::string username, uint32_t uid)
	: mUsername(username)
{
	// Use this constructor only in multi
	mPlayerID = uid;

	std::cout << "Created player (NETWORK) " << mUsername << " with id " << mPlayerID << std::endl;
}

Player::~Player()
{
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
