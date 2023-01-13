#pragma once

#include "round.hpp"

#include <vector>

class RoundEditor
{
private:
	static std::vector<RoundInfo> mRoundInfo;


	static void DisplayTable();
	static void DisplayParamColumn(RoundInfo* round);
	static void HandleClear();

public:
	static void Update();
};

