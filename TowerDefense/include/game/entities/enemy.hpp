#pragma once

#include "entity.hpp"
#include <stdint.h>
#include <Vector2.h>


class Enemy : public Entity
{
public:
	Enemy(Point2 pixelPosition, uint32_t health, uint16_t damage, uint32_t moneyDrop);

	virtual void OnUpdate() override;
	virtual void OnRender() override;

	virtual void StickToPath();

	/// <summary>
	/// Deals damage to ane enemy
	/// </summary>
	/// <param name="damage">Damage to deal</param>
	/// <param name="damageDealt">(Reference) The actual damage dealt</param>
	/// <returns>Wheter the enemy died</returns>
	bool DealDamage(uint32_t damage, uint32_t& damageDealt);

	/// <summary>
	/// Heals an enemy
	/// </summary>
	/// <param name="increment">Amout to heal</param>
	/// <returns>Wheter the enemy was fully healed</returns>
	bool Heal(uint32_t increment);

	void SlowDown();

	uint32_t GetMoneyDrop() const { return mMoneyDrop; }
	uint32_t GetSpawnHealth() const { return mSpawnHealth; }
	void SetSpawnHealth(uint32_t newSpawnHealth) { mSpawnHealth = newSpawnHealth; }
	uint32_t GetHealth() const { return mHealth; }
	void SetHealth(uint32_t newHealth) { mHealth = newHealth; }
	void SetCurrentPathIndex(uint16_t newCurrentPathIndex) { mCurrentPathIndex = newCurrentPathIndex; }
	void SetMoneyDrop(uint32_t newMoneyDrop) { mMoneyDrop = newMoneyDrop; }

protected:
	float mSpeed;
	uint32_t mHealth;
	uint32_t mSpawnHealth;
	bool mRotateForward = true;

	uint16_t mCurrentPathIndex;

    virtual void DrawHealthBar(ImVec2& pos);

private:
	Vector2 mVelocity;

	uint16_t mDamage;

	uint32_t mMoneyDrop;
	
	bool mSlowed;
	float_t mSlowedTimer;
};
