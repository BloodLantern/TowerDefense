#pragma once

#include <stdint.h>
#include "PlayField.h"
#include "imgui_utils.hpp"

class LevelEditor
{
private:
	static int32_t m_currentBlockType;
	static bool m_dragEnabled;
	static bool m_canPlaceTile;
	static bool m_updateAStar;
	static char m_fileName[20];

	static bool m_selectionActive;
	static uint8_t m_selectionStartX;
	static uint8_t m_selectionStartY;
	static uint8_t m_selectionEndX;
	static uint8_t m_selectionEndY;
	static uint8_t m_selectionWidth;
	static uint8_t m_selectionHeight;;
	static uint8_t m_selectionCopyWidth;
	static uint8_t m_selectionCopyHeight;
	static std::vector<SquareType> m_selectionCopyData;

	
	static PlayField* m_playField;

	static Texture m_tileset;


	static void verticalSpace();

	static void handleMisc();
	static void handleClear();
	static void handleFile();
	static void handleCursor();

	static void handleTileset();

	static void handleSelection();
	static void handleHotkeys();


public:
	static void loadTileset(const char* name);
	static void bindPlayField(PlayField* field);
	
	static void update();
};

