#pragma once

#include <stdint.h>
#include "playfield.hpp"
#include "imgui_utils.hpp"
#include "path.hpp"

class LevelEditor
{
private:
	struct SelectionInfo
	{
		bool active = false;
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

	struct PathEditInfo
	{
		bool active = false;
		bool render = true;

		PathNodeDir placingNodeDir;
		int16_t selectedNode;
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
	static struct PathEditInfo mPathEditInfo;

	static void Init();

	static void VerticalSpace();

	static void HandleMisc();
	
	static void HandlePath();
	static void DrawPath();
	static void HandleNodePlacement(uint8_t x, uint8_t y);

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

