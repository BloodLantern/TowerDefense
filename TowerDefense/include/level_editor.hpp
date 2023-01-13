#pragma once

#include <stdint.h>
#include "playfield.hpp"
#include "imgui_utils.hpp"

class LevelEditor
{
private:
	static int32_t mCurrentBlockType;
	static int32_t mCurrentLayer;
	static uint16_t mCurrentTileValue;
		
	static bool mDragEnabled;
	static bool mCanPlaceTile;
	static bool mUpdateAStar;
	static char mFileName[20];

	static int32_t mGridWidthInput;
	static int32_t mGridHeightInput;
	static PlayFieldDrawFlags mDrawFlags;

	static bool mSelectionActive;
	static uint8_t mSelectionStartX;
	static uint8_t mSelectionStartY;
	static uint8_t mSelectionEndX;
	static uint8_t mSelectionEndY;
	static uint8_t mSelectionWidth;
	static uint8_t mSelectionHeight;;
	static uint8_t mSelectionCopyWidth;
	static uint8_t mSelectionCopyHeight;
	static std::vector<ClipdataType> mSelectionCopyData;

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

