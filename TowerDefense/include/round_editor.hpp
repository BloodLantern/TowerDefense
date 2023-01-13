#pragma once

#include "round.hpp"

#include <vector>

class RoundEditor
{
private:
	static std::vector<RoundInfo> mRoundInfo;
	static char mFileName[30];


	static void DisplayTable();
	static void DisplayParamColumn(RoundInfo* round);
	static void HandleClear();
	static void HandleSaveLoad();

	// Temp
	static void Load(std::vector<RoundInfo>& dst, const char* const src);
	static void Save(std::vector<RoundInfo>& src, const char* const dst);

public:
	static void Update();
};

