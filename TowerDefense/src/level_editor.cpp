#include "level_editor.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <imgui.h>

#include "globals.hpp"
#include "a_star.hpp"
#include "gui.hpp"


bool LevelEditor::mInitDone;

int32_t LevelEditor::mCurrentBlockType;
int32_t LevelEditor::mCurrentLayer;
bool LevelEditor::mEditingClipdata;
uint16_t LevelEditor::mCurrentTileValue;

bool LevelEditor::mDragEnabled;
bool LevelEditor::mCanPlaceTile;
bool LevelEditor::mUpdateAStar;
char LevelEditor::mFileName[20];

int32_t LevelEditor::mGridWidthInput;
int32_t LevelEditor::mGridHeightInput;
PlayFieldDrawFlags LevelEditor::mDrawFlags;

LevelEditor::SelectionInfo LevelEditor::mSelectionInfo;
struct LevelEditor::PathEditInfo LevelEditor::mPathEditInfo;


static const char* const sClipdataNames[] = {
	"Empty",
	"Nothing",
	"Enemy only",
	"Occupied",
	"Player only",
};

void LevelEditor::Init()
{
	PlayField* pf = Globals::gGame->GetPlayField();

	LevelEditor::mDrawFlags = pf->GetDrawFlags();
	LevelEditor::mGridWidthInput = pf->gridWidth;
	LevelEditor::mGridHeightInput = pf->gridHeight;
}

void LevelEditor::Update()
{
	if (!mInitDone)
	{
		LevelEditor::Init();
		mInitDone = true;
	}

	if (ImGui::Begin("Level editor", &Gui::openedWindows[GUI_WINDOW_ID_LEVEL_EDITOR]))
	{
		LevelEditor::HandleMisc();
		LevelEditor::HandleClear();
		
		LevelEditor::VerticalSpace();
		LevelEditor::HandlePath();
		LevelEditor::DrawPath();

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
	ImGui::Text("Clipdata");
	ImGui::SameLine();
	ImGui::Combo("##", &LevelEditor::mCurrentBlockType, sClipdataNames, IM_ARRAYSIZE(sClipdataNames));

	LevelEditor::VerticalSpace();

	// Not very scalable
	float_t posX = ImGui::GetWindowSize().x - 200.f;

	ImGui::Text("Edit");
	ImGui::SameLine(posX);
	ImGui::Text("Display");

	ImGui::Dummy(ImVec2(0, 10));

	ImGui::SetCursorPosX(posX);
	ImGui::CheckboxFlags("Display grid", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_GRID_LINES);
	ImGui::Checkbox("Clipdata", &LevelEditor::mEditingClipdata);
	ImGui::SameLine(posX);
	ImGui::CheckboxFlags("Display clipdata", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_CLIPDATA);

	ImGui::RadioButton("Layer 0", &LevelEditor::mCurrentLayer, 0);
	ImGui::SameLine(posX);
	ImGui::CheckboxFlags("Display layer 0", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_LAYER0);

	ImGui::RadioButton("Layer 1", &LevelEditor::mCurrentLayer, 1);
	ImGui::SameLine(posX);
	ImGui::CheckboxFlags("Display layer 1", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_LAYER1);

	ImGui::RadioButton("Layer 2", &LevelEditor::mCurrentLayer, 2);
	ImGui::SameLine(posX);
	ImGui::CheckboxFlags("Display layer 2", (uint32_t*)&LevelEditor::mDrawFlags, PLAYFIELD_DRAW_FLAGS_LAYER2);

	ImGui::Text("Layer : %d ; Tile : %d", LevelEditor::mCurrentLayer, LevelEditor::mCurrentTileValue);

	Globals::gGame->GetPlayField()->SetDrawFlags(PLAYFIELD_DRAW_FLAGS_OPERATION_SET, LevelEditor::mDrawFlags);
	
	LevelEditor::VerticalSpace();

	ImGui::Checkbox("Enable dragging", &LevelEditor::mDragEnabled);
	ImGui::Checkbox("Update A* live", &LevelEditor::mUpdateAStar);
}

void LevelEditor::HandlePath()
{
	PathEditInfo* info = &LevelEditor::mPathEditInfo;

	if (ImGui::Button("Add node"))
	{
		info->active ^= true;

		info->placingNodeDir = PathNodeDir::DIR_LEFT;
		info->selectedNode = -1;
	}

	ImGui::SameLine();
	ImGui::Checkbox("Draw path", &info->render);
}

void LevelEditor::HandleNodePlacement(uint8_t x, uint8_t y)
{
	Texture* tex = Globals::gResources->GetTexture("ui\\arrow");
	PathEditInfo* info = &LevelEditor::mPathEditInfo;

	ImVec2 pos(Globals::gGridX + x * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2,
		Globals::gGridY + y * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2);

	if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
		info->placingNodeDir = PathNodeDir::DIR_LEFT;

	if (ImGui::IsKeyPressed(ImGuiKey_RightArrow))
		info->placingNodeDir = PathNodeDir::DIR_RIGHT;

	if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
		info->placingNodeDir = PathNodeDir::DIR_UP;

	if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
		info->placingNodeDir = PathNodeDir::DIR_DOWN;

	double_t rotation;
	switch (info->placingNodeDir)
	{
		case PathNodeDir::DIR_RIGHT:
			rotation = 3 * M_PI / 2;
			break;

		case PathNodeDir::DIR_LEFT:
			rotation = M_PI / 2;
			break;
		
		case PathNodeDir::DIR_UP:
			rotation = M_PI;
			break;
		
		default:
		case PathNodeDir::DIR_DOWN:
			rotation = 0;
	}

	ImVec2 scale(GRID_SQUARE_SIZE / (float_t)tex->width, GRID_SQUARE_SIZE / (float_t)tex->height);
	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *tex, pos, scale, rotation);

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		PathNode node(x, y, info->placingNodeDir);

		std::vector<PathNode>& nodes = Globals::gGame->GetPlayField()->GetPathNodes();

		std::vector<PathNode>::iterator _f = std::find(nodes.begin(), nodes.end(), node);

		if (_f != nodes.end())
			_f->direction = info->placingNodeDir;
		else
			nodes.push_back(node);


		info->active = false;
	}
}

void LevelEditor::DrawPath()
{
	PathEditInfo* info = &LevelEditor::mPathEditInfo;
	
	if (!info->render)
		return;

	Texture* tex = Globals::gResources->GetTexture("ui\\arrow");
	PlayField* pf = Globals::gGame->GetPlayField();

	std::vector<PathNode>& nodes = pf->GetPathNodes();
	if (nodes.size() == 0)
		return;

	std::vector<PathNode> pathNodes;
	pathNodes.push_back(nodes[0]);

	bool brokenPath = false;
	PathNode* pathEnd = nullptr;

	for (std::vector<PathNode>::iterator _n = nodes.begin(); _n != nodes.end(); _n++)
	{
		PathNode* node = &*_n;

		double_t rotation;
		switch (node->direction)
		{
			case PathNodeDir::DIR_RIGHT:
				rotation = 3 * M_PI / 2;
				break;

			case PathNodeDir::DIR_LEFT:
				rotation = M_PI / 2;
				break;

			case PathNodeDir::DIR_UP:
				rotation = M_PI;
				break;

			default:
			case PathNodeDir::DIR_DOWN:
				rotation = 0;
		}

		ImVec2 pos(Globals::gGridX + node->x * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2,
			Globals::gGridY + node->y * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2);

		ImVec2 scale(GRID_SQUARE_SIZE / (float_t)tex->width, GRID_SQUARE_SIZE / (float_t)tex->height);
		ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *tex, pos, scale, rotation);

		if (brokenPath)
			continue;

		PathNode nextNode = pf->GetNextPathNode(node->x, node->y, node->direction);
		if (nextNode.x == UCHAR_MAX)
		{
			brokenPath = true;

			if (pathEnd == nullptr)
				pathEnd = node;
			continue;
		}

		pathNodes.push_back(nextNode);
	}

	PathNode* prevNode = nullptr;
	for (std::vector<PathNode>::iterator _n = pathNodes.begin(); _n != pathNodes.end(); _n++)
	{
		PathNode* node = &*_n;

		if (prevNode)
		{
			ImVec2 start(Globals::gGridX + node->x * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2,
				Globals::gGridY + node->y * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2);

			ImVec2 end(Globals::gGridX + prevNode->x * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2,
				Globals::gGridY + prevNode->y * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2);

			Globals::gDrawList->AddLine(start, end, IM_COL32(0xFF, 0, 0, 0xFF), 5.f);
		}
		
		prevNode = node;
	}
}

void LevelEditor::HandleClear()
{
	if (ImGui::Button("Clear"))
		ImGui::OpenPopup("Delete?");

	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		PlayField* pf = Globals::gGame->GetPlayField();

		ImGui::Text("Are you sure you want to clear everything?");

		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			for (int32_t x = 0; x < pf->gridWidth; x++)
			{
				for (int32_t y = 0; y < pf->gridHeight; y++)
				{
					pf->SetClipdataTile(x, y, CLIPDATA_TYPE_EMPTY);
					pf->SetLayertile(x, y, 0, 115);
					pf->SetLayertile(x, y, 1, 115);
					pf->SetLayertile(x, y, 2, 115);
				}
			}

			pf->GetPathNodes().clear();
			AStar::FindBestPath(pf->gridWidth - 1, pf->gridHeight / 2, 0, pf->gridHeight / 2);
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

	PlayField* pf = Globals::gGame->GetPlayField();

	if (ImGui::Button("Save"))
		pf->Save(LevelEditor::mFileName);

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		pf->Load(LevelEditor::mFileName);

		AStar::FindBestPath(pf->gridWidth - 1, pf->gridHeight / 2, 0, pf->gridHeight / 2);
	}
}

void LevelEditor::HandleCursor()
{
	if (Globals::gIO->WantCaptureMouse)
		return;

	PlayField* pf = Globals::gGame->GetPlayField();

	ImVec2 mouse = Globals::gIO->MousePos;
	uint8_t tileX;
	uint8_t tileY;

	pf->GetGridPositionFromPixels(mouse.x - Globals::gGridX, mouse.y - Globals::gGridY, tileX, tileY);
	if (tileX != UCHAR_MAX && tileY != UCHAR_MAX)
	{
		ImVec2 pMin(Globals::gGridX + tileX * GRID_SQUARE_SIZE, Globals::gGridY + tileY * GRID_SQUARE_SIZE);
		ImVec2 pMax(pMin.x + GRID_SQUARE_SIZE, pMin.y + GRID_SQUARE_SIZE);

		Globals::gDrawList->AddRectFilled(pMin, pMax, IM_COL32(0xFF, 0, 0, 0x50));
		ImGui::Text("Cursor position : (%d ; %d)", tileX, tileY);

		if (!LevelEditor::mCanPlaceTile)
			return;

		if (LevelEditor::mPathEditInfo.active)
		{
			LevelEditor::HandleNodePlacement(tileX, tileY);
			return;
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
			(LevelEditor::mDragEnabled && ImGui::IsMouseDown(ImGuiMouseButton_Left)))
		{
			if (LevelEditor::mEditingClipdata)
				pf->SetClipdataTile(tileX, tileY, static_cast<ClipdataType>(LevelEditor::mCurrentBlockType));

			if (LevelEditor::mCurrentLayer != -1)
				pf->SetLayertile(tileX, tileY, LevelEditor::mCurrentLayer, LevelEditor::mCurrentTileValue);

			if (LevelEditor::mUpdateAStar)
				AStar::FindBestPath(pf->gridWidth - 1, pf->gridHeight / 2, 0, pf->gridHeight / 2);
		}
	}
}


void LevelEditor::HandleSelection()
{
	if (Globals::gIO->WantCaptureMouse)
		return;

	PlayField* pf = Globals::gGame->GetPlayField();

	ImVec2 mouse = Globals::gIO->MousePos;
	mouse.x -= Globals::gGridX;
	mouse.y -= Globals::gGridY;
	// Update selection positions
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		pf->GetGridPositionFromPixels(mouse.x, mouse.y, mSelectionInfo.startX, mSelectionInfo.startY);
		if (mSelectionInfo.startX == UCHAR_MAX || mSelectionInfo.startY == UCHAR_MAX)
			return;
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		uint8_t prevEndX = mSelectionInfo.endX;
		uint8_t prevEndY = mSelectionInfo.endY;

		pf->GetGridPositionFromPixels(mouse.x, mouse.y, mSelectionInfo.endX, mSelectionInfo.endY);
		if (mSelectionInfo.endX == UCHAR_MAX)
			mSelectionInfo.endX = prevEndX;
		if (mSelectionInfo.endY == UCHAR_MAX)
			mSelectionInfo.endY = prevEndY;

		if (mSelectionInfo.endX < mSelectionInfo.startX)
			mSelectionInfo.endX--;

		if (mSelectionInfo.endY < mSelectionInfo.startY)
			mSelectionInfo.endY--;

		if (mSelectionInfo.startX == mSelectionInfo.endX && mSelectionInfo.startY == mSelectionInfo.endY)
			mSelectionInfo.active = false;
		else
			mSelectionInfo.active = true;
	}

	if (!mSelectionInfo.active)
		return;


	mSelectionInfo.width = std::abs(mSelectionInfo.endX - mSelectionInfo.startX) + 1;
	mSelectionInfo.height = std::abs(mSelectionInfo.endY - mSelectionInfo.startY) + 1;

	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_C))
	{
		mSelectionInfo.clipdataData.resize((size_t)(mSelectionInfo.width * mSelectionInfo.height));
		mSelectionInfo.layerData.resize((size_t)(mSelectionInfo.width * mSelectionInfo.height));

		mSelectionInfo.layer = mCurrentLayer;

		uint8_t startX = mSelectionInfo.startX < mSelectionInfo.endX ? mSelectionInfo.startX : mSelectionInfo.endX;
		uint8_t startY = mSelectionInfo.startY < mSelectionInfo.endY ? mSelectionInfo.startY : mSelectionInfo.endY;

		mSelectionInfo.copyWidth = mSelectionInfo.width;
		mSelectionInfo.copyHeight = mSelectionInfo.height;

		for (int32_t y = 0; y < mSelectionInfo.height; y++)
		{
			for (int32_t x = 0; x < mSelectionInfo.width; x++)
			{
				ClipdataType clipdata = pf->GetClipdataTile(startX + x, startY + y);
				uint16_t tile = pf->GetLayerTile(startX + x, startY + y, mSelectionInfo.layer);

				size_t offset = (size_t)y * mSelectionInfo.copyWidth + x;
				mSelectionInfo.clipdataData[offset] = clipdata;
				mSelectionInfo.layerData[offset] = tile;
			}
		}
	}

	// Draw outline
	ImVec2 start((float_t)Globals::gGridX + mSelectionInfo.startX * GRID_SQUARE_SIZE,
		(float_t)Globals::gGridY + mSelectionInfo.startY * GRID_SQUARE_SIZE);
	ImVec2 end(Globals::gGridX + mSelectionInfo.endX * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE - 1.f,
		Globals::gGridY + mSelectionInfo.endY * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE - 1.f);

	Globals::gDrawList->AddRect(start, end, IM_COL32(0x80, 0x80, 0xB0, 0xC0), 0, 0, 4);

	LevelEditor::VerticalSpace();

	ImGui::Text("Selection:");
	ImGui::Text("\tStart: (%d ; %d)", mSelectionInfo.startX, mSelectionInfo.startY);
	ImGui::Text("\tEnd:   (%d ; %d)", mSelectionInfo.endX, mSelectionInfo.endY);
	ImGui::Text("\tSize:  (%d ; %d)", mSelectionInfo.width, mSelectionInfo.height);
}

void LevelEditor::HandleTileset()
{
	if (ImGui::Begin("Tileset", nullptr, ImGuiWindowFlags_NoDecoration))
	{
		PlayField* pf = Globals::gGame->GetPlayField();

		ImGui::SetScrollY(GRID_SQUARE_SIZE * (int)(ImGui::GetScrollY() / GRID_SQUARE_SIZE));

		ImGui::Image(pf->tileset->id, ImVec2(pf->tileset->width, pf->tileset->height));

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
					(Globals::gGame->GetPlayField()->tileset->width / GRID_SQUARE_SIZE) + tileX;
			}
		}
	}

	ImGui::End();
}

void LevelEditor::HandleHotkeys()
{
	if (!ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		LevelEditor::mCanPlaceTile = true;
		return;
	}

	PlayField* pf = Globals::gGame->GetPlayField();

	LevelEditor::mCanPlaceTile = false;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		uint8_t tileX;
		uint8_t tileY;
		ImVec2 mouse = Globals::gIO->MousePos;

		pf->GetGridPositionFromPixels((int32_t)mouse.x, (int32_t)mouse.y, tileX, tileY);

		if (tileX != UCHAR_MAX && tileY != UCHAR_MAX)
		{
			LevelEditor::mCurrentBlockType = pf->GetClipdataTile(tileX, tileY);
			LevelEditor::mCurrentTileValue = pf->GetLayerTile(tileX, tileY, LevelEditor::mCurrentLayer);
		}
		return;
	}

	if (ImGui::IsKeyPressed(ImGuiKey_V) && mSelectionInfo.clipdataData.size() != 0)
	{
		uint8_t tileX;
		uint8_t tileY;
		ImVec2 mouse = Globals::gIO->MousePos;

		pf->GetGridPositionFromPixels(mouse.x - Globals::gGridX, mouse.y - Globals::gGridY, tileX, tileY);

		if (tileX == UCHAR_MAX || tileY == UCHAR_MAX)
			return;

		if (tileX + mSelectionInfo.copyWidth > pf->gridWidth)
			return;

		if (tileY + mSelectionInfo.copyHeight > pf->gridHeight)
			return;

		for (int32_t y = 0; y < mSelectionInfo.height; y++)
		{
			for (int32_t x = 0; x < mSelectionInfo.copyWidth; x++)
			{
				size_t offset = (size_t)y * mSelectionInfo.copyWidth + x;

				ClipdataType clipdata = mSelectionInfo.clipdataData[offset];
				uint16_t tile = mSelectionInfo.layerData[offset];

				if (LevelEditor::mEditingClipdata)
					pf->SetClipdataTile(tileX + x, tileY + y, clipdata);

				if (LevelEditor::mCurrentLayer != -1)
					pf->SetLayertile(tileX + x, tileY + y, mSelectionInfo.layer, tile);
			}
		}

		AStar::FindBestPath(pf->gridWidth - 1, pf->gridHeight / 2, 0, pf->gridHeight / 2);
	}
}
