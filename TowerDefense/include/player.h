#pragma once

#include "Tower.h"

#include <stdint.h>
#include <vector>

class Player {
public:
    Player();
    ~Player() {};

    std::vector<Tower*>* GetTowers() { return &mTowers; }

private:
    uint32_t mMoney = 200;
    uint8_t mLife = 10;

    std::vector<Tower*> mTowers;
};
