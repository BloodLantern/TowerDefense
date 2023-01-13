#pragma once

#include "tower.hpp"

#include <stdint.h>
#include <vector>

class Player {
public:
    Player();
    ~Player();

    std::vector<Tower*>* GetTowers() { return &mTowers; }

    void OnRender();

    void DecreaseLife(uint16_t amount);
    void IncreaseLife(uint16_t amount);

    uint32_t GetMoney() const { return mMoney; }
    void DecreaseMoney(uint32_t amount);
    void IncreaseMoney(uint32_t amount);

private:
    uint32_t mMoney = 200;
    uint16_t mLife = 10;

    std::vector<Tower*> mTowers;
};
