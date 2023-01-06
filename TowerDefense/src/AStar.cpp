#include "AStar.h"
#include "Globals.h"
#include <list>
#include <chrono>

PlayField* AStar::field;
AStar::AStarNode AStar::nodesField[GRID_HEIGHT][GRID_WIDTH];

std::vector<ImVec2> AStar::recordPositions;

bool AStar::compareHeuristics(const AStarNode* n1, const AStarNode* n2)
{
	return n1->globalGoal < n2->globalGoal;
}

double_t AStar::getDistance(const AStarNode* n1, const AStarNode* n2)
{
	uint8_t distX = n2->x - n1->x;
	uint8_t distY = n2->y - n1->y;
	return std::sqrt(distX * distX + distY * distY);
}

void AStar::bindField(PlayField* field)
{
	AStar::field = field;
}

bool AStar::findBestPath(uint8_t dstX, uint8_t dstY, uint8_t startX, uint8_t startY)
{
	//auto startTime = std::chrono::high_resolution_clock::now();

	for (int32_t y = 0; y < GRID_HEIGHT; y++)
	{
		for (int32_t x = 0; x < GRID_WIDTH; x++)
		{
			nodesField[y][x] = AStarNode(x, y, AStar::field->getClipdataTile(x, y));
		}
	}

	for (int32_t y = 0; y < GRID_HEIGHT; y++)
	{
		for (int32_t x = 0; x < GRID_WIDTH; x++)
		{
			if (y > 0)
				nodesField[y][x].neighbours.push_back(&nodesField[y - 1][x]);

			if (y < GRID_HEIGHT - 1)
				nodesField[y][x].neighbours.push_back(&nodesField[y + 1][x]);

			if (x > 0)
				nodesField[y][x].neighbours.push_back(&nodesField[y][x - 1]);

			if (x < GRID_WIDTH - 1)
				nodesField[y][x].neighbours.push_back(&nodesField[y][x + 1]);
		}
	}

	recordPositions.clear();
	
	AStarNode* start = &nodesField[startY][startX];
	AStarNode* end = &nodesField[dstY][dstX];
	AStarNode* curr = start;
	start->localGoal = 0.f;
	start->globalGoal = AStar::getDistance(curr, start);


	std::list<AStarNode*> notTestedNodes;
	notTestedNodes.push_back(start);

	while (!notTestedNodes.empty())
	{
		notTestedNodes.sort(AStar::compareHeuristics);

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

			double_t possiblyLowerGoal = curr->localGoal + getDistance(curr, neighbour);

			if (possiblyLowerGoal < neighbour->localGoal)
			{
				neighbour->parent = curr;
				neighbour->localGoal = possiblyLowerGoal;
			
				neighbour->globalGoal = neighbour->localGoal + getDistance(neighbour, end); // heuristic ?
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
		recordPositions.push_back(ImVec2(node->x * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2.f,
			node->y * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2.f));
	}
	return true;
}
