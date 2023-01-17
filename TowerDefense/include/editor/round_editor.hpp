#pragma once

#include "round.hpp"

#include <vector>

#define WAVES_PATH "data\\waves\\"

class RoundEditor
{
private:
	static std::vector<RoundInfo> mRoundInfo;
	static std::vector<bool> mSeparators;
	static char mFileName[30];

	static void DisplayTable();
	static void DisplayParamColumn(RoundInfo* round);

	static void HandleClear();
	static void HandleCurrentWaveLoad();
	static void HandleTest();
	static void HandleSaveLoad();

public:
	static void Update();

	// Temp?
	static bool Load(std::vector<RoundInfo>& dst, const char* const src);
	static void Save(std::vector<RoundInfo>& src, const char* const dst);
};

