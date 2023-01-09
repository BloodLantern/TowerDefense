#pragma once

#include "Tower.h"

#include <stdint.h>
#include <vector>

class Player {
public:
    Player();
    ~Player() {};

    std::vector<Tower*> *getTowers() { return &m_towers; }

private:
    uint32_t m_money = 200;
    uint8_t m_life = 10;

    std::vector<Tower*> m_towers;
};
