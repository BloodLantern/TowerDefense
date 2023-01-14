#pragma once

#include <stdint.h>
#include "playfield.hpp"
#include "imgui_utils.hpp"

class LevelEditor
{
private:
	struct SelectionInfo
	{
		bool active;
		uint8_t startX;
		uint8_t startY;
		uint8_t endX;
		uint8_t endY;
		uint8_t width;
		uint8_t height;
		uint8_t copyWidth;
		uint8_t copyHeight;

		std::vector<ClipdataType> clipdataData;
		std::vector<uint16_t> layerData;
		uint8_t layer;
	};

	static bool mInitDone;

	static int32_t mCurrentBlockType;
	static int32_t mCurrentLayer;
	static bool mEditingClipdata;
	static uint16_t mCurrentTileValue;
		
	static bool mDragEnabled;
	static bool mCanPlaceTile;
	static bool mUpdateAStar;
	static char mFileName[20];

	static int32_t mGridWidthInput;
	static int32_t mGridHeightInput;
	static PlayFieldDrawFlags mDrawFlags;

	static struct SelectionInfo mSelectionInfo;

	static void Init();

	static void VerticalSpace();

	static void HandleMisc();
	static void HandleClear();
	static void HandleResize();

	static void HandleFile();
	static void HandleCursor();

	static void HandleTileset();

	static void HandleSelection();
	static void HandleHotkeys();


public:	
	static void Update();
};

