#pragma once

#include "PlayField.h"

#include <utility>
#include <deque>
#include <queue>
#include <imgui.h>

class AStar
{
private:
	struct AStarNode
	{
		uint8_t x;
		uint8_t y;
		double_t globalGoal;
		double_t localGoal;
		ClipdataType type;
		AStarNode* parent = nullptr;
		std::vector<AStarNode*> neighbours;
		bool visited = false;

		AStarNode()
			: x(UCHAR_MAX), y(UCHAR_MAX), localGoal(INFINITY), globalGoal(INFINITY), type(CLIPDATA_TYPE_EMPTY)
		{
		}

		AStarNode(uint8_t x, uint8_t y, ClipdataType type)
			: x(x), y(y), localGoal(INFINITY), globalGoal(INFINITY), type(type)
		{
		}

		AStarNode(uint8_t x, uint8_t y, double_t local, double_t global, ClipdataType type)
			: x(x), y(y), localGoal(local), globalGoal(global), type(type)
		{
		}
	};

	AStar() { }; // Prevent creating an instance of this class

	static std::vector<AStarNode> nodesField;

	static bool compareHeuristics(const AStarNode* n1, const AStarNode* n2);
	static double_t getDistance(const AStarNode* n1, const AStarNode* n2);

public:
	static std::vector<ImVec2> recordPositions;

	static bool findBestPath(uint8_t dstX, uint8_t dstY, uint8_t startX, uint8_t startY);
};
