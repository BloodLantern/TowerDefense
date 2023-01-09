#pragma once

#include "Tower.h"

#include <stdint.h>
#include <vector>

class Player {
public:

private:
    uint32_t m_money;
    uint8_t m_life;

    std::vector<Tower*> m_towers;
};
