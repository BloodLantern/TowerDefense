#include "player.hpp"
#include <limits.h>

Player::Player()
{

}

Player::~Player()
{
	for (std::vector<Tower*>::iterator it = mTowers.begin(); it != mTowers.end(); it++)
		delete *it;
}

void Player::OnRender()
{
	if (ImGui::Begin("Player info"))
	{
		ImGui::Text("HP : %d", mLife);
		ImGui::Text("Money : %d", mMoney);
	}

	ImGui::End();
}

void Player::DecreaseLife(uint16_t amount)
{
	// Prevent underflow
	if (amount > mLife)
		mLife = 0;
	else
		mLife -= amount;
}

void Player::IncreaseLife(uint16_t amount)
{
	uint32_t max = mLife + amount;

	// Prevent overflow
	if (max > USHRT_MAX)
		mLife = USHRT_MAX;
	else
		mLife = max;
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
