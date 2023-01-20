#include "player.hpp"
#include "globals.hpp"
#include <limits.h>
#include <windows.h>
#include <Lmcons.h>

Player::Player()
{
	// Use this constructor only in solo
	mPlayerID = 0;
	mUsername = "";
}

Player::Player(std::string username, uint32_t uid)
	: mUsername(username)
{
	// Use this constructor only in multi
	mPlayerID = uid;

	std::cout << "Created player " << mUsername << " with id " << mPlayerID << std::endl;
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
