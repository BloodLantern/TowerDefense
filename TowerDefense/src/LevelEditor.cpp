#include "LevelEditor.h"

#include <imgui.h>

#include "Globals.h"
#include "AStar.h"
#include "Gui.h"

int32_t LevelEditor::mCurrentBlockType;
int32_t LevelEditor::mCurrentLayer;
uint16_t LevelEditor::mCurrentTileValue;

bool LevelEditor::mDragEnabled;
bool LevelEditor::mCanPlaceTile;
bool LevelEditor::mUpdateAStar;
char LevelEditor::mFileName[20];

int32_t LevelEditor::mGridWidthInput;
int32_t LevelEditor::mGridHeightInput;
PlayFieldDrawFlags LevelEditor::mDrawFlags;


bool LevelEditor::mSelectionActive;
uint8_t LevelEditor::mSelectionStartX;
uint8_t LevelEditor::mSelectionStartY;
uint8_t LevelEditor::mSelectionEndX;
uint8_t LevelEditor::mSelectionEndY;
uint8_t LevelEditor::mSelectionWidth;
uint8_t LevelEditor::mSelectionHeight;
uint8_t LevelEditor::mSelectionCopyWidth;
uint8_t LevelEditor::mSelectionCopyHeight;
std::vector<ClipdataType> LevelEditor::mSelectionCopyData;

static const char* const sClipdataNames[] = {
	"Empty",
	"Nothing",
	"Enemy only",
	"Occupied",
	"Player only",
};

void LevelEditor::Update()
{
	if (ImGui::Begin("Level editor", &Gui::mOpenedWindows[GUI_WINDOW_ID_LEVEL_EDITOR]))
	{
		LevelEditor::HandleMisc();

		LevelEditor::HandleClear();

		LevelEditor::VerticalSpace();
		LevelEditor::HandleFile();

		LevelEditor::VerticalSpace();
		LevelEditor::HandleResize();

		LevelEditor::VerticalSpace();
		LevelEditor::HandleCursor();
		LevelEditor::HandleSelection();

		LevelEditor::HandleTileset();

		LevelEditor::HandleHotkeys();
	}

	ImGui::End();
}

void LevelEditor::VerticalSpace()
{
	ImGui::Dummy(ImVec2(0, 10.f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10.f));
}

void LevelEditor::HandleMisc()
{
	ImGui::RadioButton("Clipdata", &LevelEditor::mCurrentLayer, -1);
	ImGui::SameLine();
	ImGui::Text("Clipdata");
	ImGui::SameLine();
	ImGui::Combo("##", &LevelEditor::mCurrentBlockType, sClipdataNames, IM_ARRAYSIZE(sClipdataNames));
	
	ImGui::CheckboxFlags("Display clipdata", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_CLIPDATA);
	ImGui::SameLine();
	ImGui::CheckboxFlags("Display grid", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_GRID_LINES);

	LevelEditor::VerticalSpace();

	ImGui::RadioButton("Layer 0", &LevelEditor::mCurrentLayer, 0);
	ImGui::SameLine();
	ImGui::CheckboxFlags("Display layer 0", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_LAYER0);

	ImGui::RadioButton("Layer 1", &LevelEditor::mCurrentLayer, 1);
	ImGui::SameLine();
	ImGui::CheckboxFlags("Display layer 1", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_LAYER1);

	ImGui::RadioButton("Layer 2", &LevelEditor::mCurrentLayer, 2);
	ImGui::SameLine();
	ImGui::CheckboxFlags("Display layer 2", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_LAYER2);

	ImGui::Text("Layer : %d ; Tile : %d", LevelEditor::mCurrentLayer, LevelEditor::mCurrentTileValue);

	Globals::gGame->GetPlayField()->SetDrawFlags(PLAYFIELD_DRAW_FLAGS_OPERATION_SET, LevelEditor::mDrawFlags);
	
	LevelEditor::VerticalSpace();

	ImGui::Checkbox("Enable dragging", &LevelEditor::mDragEnabled);
	ImGui::Checkbox("Update A* live", &LevelEditor::mUpdateAStar);
}


void LevelEditor::HandleClear()
{
	if (ImGui::Button("Clear"))
		ImGui::OpenPopup("Delete?");

	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to clear everything?");

		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			for (int32_t x = 0; x < Globals::gGame->GetPlayField()->mGridWidth; x++)
			{
				for (int32_t y = 0; y < Globals::gGame->GetPlayField()->mGridHeight; y++)
				{
					Globals::gGame->GetPlayField()->SetClipdataTile(x, y, CLIPDATA_TYPE_EMPTY);
					Globals::gGame->GetPlayField()->SetLayertile(x, y, 0, 115);
					Globals::gGame->GetPlayField()->SetLayertile(x, y, 1, 115);
					Globals::gGame->GetPlayField()->SetLayertile(x, y, 2, 115);
				}
			}

			AStar::FindBestPath(Globals::gGame->GetPlayField()->mGridWidth - 1, Globals::gGame->GetPlayField()->mGridHeight / 2, 0, Globals::gGame->GetPlayField()->mGridHeight / 2);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("No", ImVec2(120, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}


void LevelEditor::HandleResize()
{
	ImGui::InputInt("Grid width", &LevelEditor::mGridWidthInput, 1, 5, ImGuiInputTextFlags_CharsDecimal);
	ImGui::InputInt("Grid height", &LevelEditor::mGridHeightInput, 1, 5, ImGuiInputTextFlags_CharsDecimal);

	if (ImGui::Button("Resize"))
	{
		if (LevelEditor::mGridWidthInput > 0 && LevelEditor::mGridHeightInput > 0)
			Globals::gGame->GetPlayField()->Resize(LevelEditor::mGridWidthInput, LevelEditor::mGridHeightInput);
	}
}

void LevelEditor::HandleFile()
{
	ImGui::Text("File name");
	ImGui::SameLine();
	ImGui::PushItemWidth(-1);
	ImGui::InputText("FileNameInput", LevelEditor::mFileName, sizeof(LevelEditor::mFileName));
	ImGui::PopItemWidth();

	if (ImGui::Button("Save"))
		Globals::gGame->GetPlayField()->Save(LevelEditor::mFileName);

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		Globals::gGame->GetPlayField()->Load(LevelEditor::mFileName);

		AStar::FindBestPath(Globals::gGame->GetPlayField()->mGridWidth - 1, Globals::gGame->GetPlayField()->mGridHeight / 2, 0, Globals::gGame->GetPlayField()->mGridHeight / 2);
	}
}

void LevelEditor::HandleCursor()
{
	if (Globals::gIO->WantCaptureMouse)
		return;

	ImVec2 mouse = Globals::gIO->MousePos;
	uint8_t tileX;
	uint8_t tileY;

	Globals::gGame->GetPlayField()->GetGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);
	if (tileX != UCHAR_MAX && tileY != UCHAR_MAX)
	{
		ImVec2 pMin((float_t)Globals::gGridX + tileX * GRID_SQUARE_SIZE, (float_t)Globals::gGridY + tileY * GRID_SQUARE_SIZE);
		ImVec2 pMax((float_t)Globals::gGridX + (tileX + 1) * GRID_SQUARE_SIZE, (float_t)Globals::gGridY + (tileY + 1) * GRID_SQUARE_SIZE);

		Globals::gDrawList->AddRectFilled(pMin, pMax, IM_COL32(0xFF, 0, 0, 0x50));
		ImGui::Text("Cursor position : (%d ; %d)", tileX, tileY);

		if (!LevelEditor::mCanPlaceTile)
			return;

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
			(LevelEditor::mDragEnabled && ImGui::IsMouseDown(ImGuiMouseButton_Left)))
		{
			if (LevelEditor::mCurrentLayer == -1)
				Globals::gGame->GetPlayField()->SetClipdataTile(tileX, tileY, static_cast<ClipdataType>(LevelEditor::mCurrentBlockType));
			else
				Globals::gGame->GetPlayField()->SetLayertile(tileX, tileY, LevelEditor::mCurrentLayer, LevelEditor::mCurrentTileValue);

			if (LevelEditor::mUpdateAStar)
				AStar::FindBestPath(Globals::gGame->GetPlayField()->mGridWidth - 1, Globals::gGame->GetPlayField()->mGridHeight / 2, 0, Globals::gGame->GetPlayField()->mGridHeight / 2);
		}
	}
}


void LevelEditor::HandleSelection()
{
	if (Globals::gIO->WantCaptureMouse)
		return;

	ImVec2 mouse = Globals::gIO->MousePos;
	// Update selection positions
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		Globals::gGame->GetPlayField()->GetGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, mSelectionStartX, mSelectionStartY);
		if (mSelectionStartX == UCHAR_MAX || mSelectionStartY == UCHAR_MAX)
			return;
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		uint8_t prevEndX = mSelectionEndX;
		uint8_t prevEndY = mSelectionEndY;

		Globals::gGame->GetPlayField()->GetGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, mSelectionEndX, mSelectionEndY);
		if (mSelectionEndX == UCHAR_MAX)
			mSelectionEndX = prevEndX;
		if (mSelectionEndY == UCHAR_MAX)
			mSelectionEndY = prevEndY;

		if (mSelectionEndX < mSelectionStartX)
			mSelectionEndX--;

		if (mSelectionEndY < mSelectionStartY)
			mSelectionEndY--;

		if (mSelectionStartX == mSelectionEndX && mSelectionStartY == mSelectionEndY)
			mSelectionActive = false;
		else
			mSelectionActive = true;
	}

	if (!mSelectionActive)
		return;


	LevelEditor::mSelectionWidth = std::abs(mSelectionEndX - mSelectionStartX) + 1;
	LevelEditor::mSelectionHeight = std::abs(mSelectionEndY - mSelectionStartY) + 1;

	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_C))
	{
		mSelectionCopyData.resize((size_t)(mSelectionWidth * mSelectionHeight));

		uint8_t startX = mSelectionStartX < mSelectionEndX ? mSelectionStartX : mSelectionEndX;
		uint8_t startY = mSelectionStartY < mSelectionEndY ? mSelectionStartY : mSelectionEndY;

		mSelectionCopyWidth = mSelectionWidth;
		mSelectionCopyHeight = mSelectionHeight;

		for (int32_t y = 0; y < mSelectionHeight; y++)
		{
			for (int32_t x = 0; x < mSelectionWidth; x++)
			{
				ClipdataType tile = Globals::gGame->GetPlayField()->GetClipdataTile(startX + x, startY + y);
				size_t offset = y * mSelectionCopyWidth + x;
				mSelectionCopyData[offset] = tile;
			}
		}
	}




	// Draw outline
	ImVec2 start((float_t)Globals::gGridX + mSelectionStartX * GRID_SQUARE_SIZE,
		(float_t)Globals::gGridY + mSelectionStartY * GRID_SQUARE_SIZE);
	ImVec2 end(Globals::gGridX + mSelectionEndX * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE - 1.f,
		Globals::gGridY + mSelectionEndY * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE - 1.f);

	Globals::gDrawList->AddRect(start, end, IM_COL32(0x80, 0x80, 0xB0, 0xC0), 0, 0, 4);

	LevelEditor::VerticalSpace();

	ImGui::Text("Selection:");
	ImGui::Text("\tStart: (%d ; %d)", mSelectionStartX, mSelectionStartY);
	ImGui::Text("\tEnd:   (%d ; %d)", mSelectionEndX, mSelectionEndY);
	ImGui::Text("\tSize:  (%d ; %d)", mSelectionWidth, mSelectionHeight);
}

void LevelEditor::HandleTileset()
{
	if (ImGui::Begin("Tileset", nullptr, ImGuiWindowFlags_NoDecoration))
	{
		ImGui::SetScrollY(GRID_SQUARE_SIZE * (int)(ImGui::GetScrollY() / GRID_SQUARE_SIZE));

		ImGui::Image(Globals::gGame->GetPlayField()->mTileset.id, ImVec2(Globals::gGame->GetPlayField()->mTileset.width,
			Globals::gGame->GetPlayField()->mTileset.height));

		ImVec2 mousePos = Globals::gIO->MousePos;
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		if (mousePos.x > winPos.x && mousePos.x < winPos.x + winSize.x && mousePos.y > winPos.y && mousePos.y < winPos.y + winSize.y)
		{
			ImDrawList* dl = ImGui::GetForegroundDrawList();
			uint32_t tileX = (mousePos.x - 8 - winPos.x) / GRID_SQUARE_SIZE;
			uint32_t tileY = (mousePos.y - 8 - winPos.y) / GRID_SQUARE_SIZE;

			uint32_t x = (tileX * GRID_SQUARE_SIZE) + winPos.x + 8;
			uint32_t y = (tileY * GRID_SQUARE_SIZE) + winPos.y + 8;

			dl->AddRect(ImVec2(x, y), ImVec2(x + GRID_SQUARE_SIZE, y + GRID_SQUARE_SIZE), IM_COL32(0xFF, 0, 0, 0xFF));

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left, false))
			{
				// Tile position, accounting for Y scroll
				LevelEditor::mCurrentTileValue = (tileY + (int)ImGui::GetScrollY() / GRID_SQUARE_SIZE) *
					(Globals::gGame->GetPlayField()->mTileset.width / GRID_SQUARE_SIZE) + tileX;
			}
		}
	}

	ImGui::End();
}

void LevelEditor::HandleHotkeys()
{
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		LevelEditor::mCanPlaceTile = false;
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			uint8_t tileX;
			uint8_t tileY;
			ImVec2 mouse = Globals::gIO->MousePos;

			Globals::gGame->GetPlayField()->GetGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);

			if (tileX != UCHAR_MAX && tileY != UCHAR_MAX)
				LevelEditor::mCurrentBlockType = Globals::gGame->GetPlayField()->GetClipdataTile(tileX, tileY);
			return;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_V) && mSelectionCopyData.size() != 0)
		{
			uint8_t tileX;
			uint8_t tileY;
			ImVec2 mouse = Globals::gIO->MousePos;

			Globals::gGame->GetPlayField()->GetGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);

			if (tileX == UCHAR_MAX || tileY == UCHAR_MAX)
				return;

			if (tileX + mSelectionCopyWidth > Globals::gGame->GetPlayField()->mGridWidth)
				return;

			if (tileY + mSelectionCopyHeight > Globals::gGame->GetPlayField()->mGridHeight)
				return;

			for (int32_t y = 0; y < mSelectionHeight; y++)
			{
				for (int32_t x = 0; x < mSelectionCopyWidth; x++)
				{
					ClipdataType tile = mSelectionCopyData[y * mSelectionCopyWidth + x];
					Globals::gGame->GetPlayField()->SetClipdataTile(tileX + x, tileY + y, tile);
				}
			}

			AStar::FindBestPath(Globals::gGame->GetPlayField()->mGridWidth - 1, Globals::gGame->GetPlayField()->mGridHeight / 2, 0, Globals::gGame->GetPlayField()->mGridHeight / 2);
		}

		return;
	}

	LevelEditor::mCanPlaceTile = true;
}
