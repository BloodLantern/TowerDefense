#pragma once

#include <stdint.h>
#include "PlayField.h"
#include "imgui_utils.hpp"

class LevelEditor
{
private:
	static int32_t m_currentBlockType;
	static int32_t m_currentLayer;
	static uint16_t m_currentTileValue;
		
	static bool m_dragEnabled;
	static bool m_canPlaceTile;
	static bool m_updateAStar;
	static char m_fileName[20];

	static int32_t m_gridWidthInput;
	static int32_t m_gridHeightInput;

	static bool m_selectionActive;
	static uint8_t m_selectionStartX;
	static uint8_t m_selectionStartY;
	static uint8_t m_selectionEndX;
	static uint8_t m_selectionEndY;
	static uint8_t m_selectionWidth;
	static uint8_t m_selectionHeight;;
	static uint8_t m_selectionCopyWidth;
	static uint8_t m_selectionCopyHeight;
	static std::vector<ClipdataType> m_selectionCopyData;

	static void verticalSpace();

	static void handleMisc();
	static void handleClear();
	static void handleResize();

	static void handleFile();
	static void handleCursor();

	static void handleTileset();

	static void handleSelection();
	static void handleHotkeys();


public:
	static void loadTileset(const char* name);
	
	static void update();
};

