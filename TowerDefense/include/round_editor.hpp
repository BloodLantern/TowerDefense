#pragma once

#include "round.hpp"

#include <vector>

class RoundEditor
{
private:
	static std::vector<RoundInfo> mRoundInfo;


	static void DisplayTable();
	static void DisplayParamColumn(RoundInfo* round);

public:
	static void Update();
};

