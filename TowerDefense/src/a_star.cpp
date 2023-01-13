#include "a_star.hpp"
#include "globals.hpp"
#include <list>
#include <chrono>

std::vector<AStar::AStarNode> AStar::mNodesField;

std::vector<ImVec2> AStar::recordPositions;

bool AStar::CompareHeuristics(const AStarNode* n1, const AStarNode* n2)
{
	return n1->globalGoal < n2->globalGoal;
}

double_t AStar::GetDistance(const AStarNode* n1, const AStarNode* n2)
{
	uint8_t distX = n2->x - n1->x;
	uint8_t distY = n2->y - n1->y;
	return std::sqrt(distX * distX + distY * distY);
}

bool AStar::FindBestPath(uint8_t dstX, uint8_t dstY, uint8_t startX, uint8_t startY)
{
	//auto startTime = std::chrono::high_resolution_clock::now();

	uint16_t width = Globals::gGame->GetPlayField()->gridWidth;
	uint16_t height = Globals::gGame->GetPlayField()->gridHeight;

	mNodesField.resize(width * height);

	for (int32_t y = 0; y < height; y++)
	{
		for (int32_t x = 0; x < width; x++)
		{
			mNodesField[y * width + x] = AStarNode(x, y, Globals::gGame->GetPlayField()->GetClipdataTile(x, y));
		}
	}

	for (int32_t y = 0; y < height; y++)
	{
		for (int32_t x = 0; x < width; x++)
		{
			if (y > 0)
				mNodesField[y * width + x].neighbours.push_back(&mNodesField[(y - 1) * width + x]);

			if (y < height - 1)
				mNodesField[y * width + x].neighbours.push_back(&mNodesField[(y + 1) * width + x]);

			if (x > 0)
				mNodesField[y * width + x].neighbours.push_back(&mNodesField[y * width + (x - 1)]);

			if (x < width - 1)
				mNodesField[y * width + x].neighbours.push_back(&mNodesField[y * width + (x + 1)]);
		}
	}

	recordPositions.clear();
	
	AStarNode* start = &mNodesField[startY * width + startX];
	AStarNode* end = &mNodesField[dstY * width + dstX];
	AStarNode* curr = start;
	start->localGoal = 0.f;
	start->globalGoal = AStar::GetDistance(curr, start);


	std::list<AStarNode*> notTestedNodes;
	notTestedNodes.push_back(start);

	while (!notTestedNodes.empty())
	{
		notTestedNodes.sort(AStar::CompareHeuristics);

		while (!notTestedNodes.empty() && notTestedNodes.front()->visited)
			notTestedNodes.pop_front();

		if (notTestedNodes.empty())
			break;

		curr = notTestedNodes.front();
		curr->visited = true;
	
		for (std::vector<AStarNode*>::iterator _neighbour = curr->neighbours.begin(); _neighbour != curr->neighbours.end(); _neighbour++)
		{
			AStarNode* neighbour = *_neighbour;
			
			if (!neighbour->visited && (neighbour->type == CLIPDATA_TYPE_ENEMY_ONLY || neighbour->type == CLIPDATA_TYPE_EMPTY))
				notTestedNodes.push_back(neighbour);

			double_t possiblyLowerGoal = curr->localGoal + GetDistance(curr, neighbour);

			if (possiblyLowerGoal < neighbour->localGoal)
			{
				neighbour->parent = curr;
				neighbour->localGoal = possiblyLowerGoal;
			
				neighbour->globalGoal = neighbour->localGoal + GetDistance(neighbour, end); // heuristic ?
			}
		}
	}

	//auto endTime = std::chrono::high_resolution_clock::now();
	//std::cout << "Execution time: " << (endTime - startTime).count() << "ns" << std::endl;

	// Check could finish the path
	if (end->parent == nullptr)
		return false;

	for (AStarNode* node = end; node->parent != nullptr; node = node->parent)
	{
		recordPositions.insert(recordPositions.begin(), ImVec2(node->x * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2.f,
			node->y * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2.f));
	}
	return true;
}
