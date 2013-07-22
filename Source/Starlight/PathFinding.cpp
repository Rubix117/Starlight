#include "PathFinding.h"

namespace starlight
{
	PathFinding::PathFinding(Map &map, bool isEnemy) : 
		tiles(map.tiles), 
		isEnemy(isEnemy),
		ROWS(map.ROWS),
		COLS(map.COLS),
		initStartAndGoal(false),
		goalReached(false)
	{
	}


	PathFinding::~PathFinding()
	{
	}

	std::vector<Vector2F*> PathFinding::FindPath(Vector2F currentPos, Vector2F goalPos)
	{
		if(!initStartAndGoal)
		{
			ClearOpenList();
			ClearClosedList();
			ClearPathList();

			SearchCell start(Vector2(currentPos.x, currentPos.y), NULL);
			SearchCell goal(Vector2(goalPos.x, goalPos.y), NULL);
			
			SetStartAndGoal(start, goal);			
		}

		if(initStartAndGoal)
		{			
			ContinuePath();
		}

		return pathList;
	}

	void PathFinding::SetStartAndGoal(SearchCell start, SearchCell goal)
	{
		startCell = new SearchCell(start.position, NULL);
		goalCell = new SearchCell(goal.position, NULL);
		startCell->G = 0;
		startCell->H = startCell->ManhattanDistance(goalCell);
		openList.push_back(startCell);
		initStartAndGoal = true;
	}

	void PathFinding::ContinuePath()
	{
		if(openList.empty())
		{
			return;
		}

		while(pathList.empty())
		{
			SearchCell *currentCell = GetNextCell();
			
			//std::cout << "Current Cell "<< currentCell->position.x << " " << currentCell->position.y << std::endl;
			//std::cin.get();
			if(currentCell->position == goalCell->position)
			{
				goalCell->parent = currentCell->parent;
				SearchCell *pathCell;
				//std::cout << "Found path\n";
				for(pathCell = goalCell; pathCell != NULL; pathCell = pathCell->parent)
				{
					Vector2F *path = new Vector2F(pathCell->position.x, pathCell->position.y);
					pathList.push_back(path);
				}
				goalReached = true;
				initStartAndGoal = false;
				return;
			}
			else
			{
				//std::cout << "Opening Cells\n";
				OpenCells(Vector2(currentCell->position.x + 1, currentCell->position.y), currentCell->G + 1, currentCell);
				OpenCells(Vector2(currentCell->position.x - 1, currentCell->position.y), currentCell->G + 1, currentCell);
				OpenCells(Vector2(currentCell->position.x, currentCell->position.y + 1), currentCell->G + 1, currentCell);
				OpenCells(Vector2(currentCell->position.x, currentCell->position.y - 1), currentCell->G + 1, currentCell);
			
				if(tiles[currentCell->position.y][currentCell->position.x].type != Tile::DOOR && tiles[currentCell->position.y][currentCell->position.x].type != Tile::CORRIDOR && isEnemy)
				{
					OpenCells(Vector2(currentCell->position.x + 1, currentCell->position.y + 1), currentCell->G + 1.41, currentCell);
					OpenCells(Vector2(currentCell->position.x - 1, currentCell->position.y - 1), currentCell->G + 1.41, currentCell);
					OpenCells(Vector2(currentCell->position.x + 1, currentCell->position.y - 1), currentCell->G + 1.41, currentCell);
					OpenCells(Vector2(currentCell->position.x - 1, currentCell->position.y + 1), currentCell->G + 1.41, currentCell);
				}
			}
			for(unsigned int i = 0; i < openList.size(); i++)
			{
				if(currentCell->position == openList[i]->position)
				{
					delete openList[i];
					openList.erase(openList.begin() + i);
				}
			}
		}
	}

	SearchCell* PathFinding::GetNextCell()
	{
		float bestF = 9999.0f;
		int cellIndex = -1;
        SearchCell *nextCell;

		for(unsigned int i = 0; i < openList.size(); i++)
		{
			if(openList[i]->GetF() < bestF)
			{
				bestF = openList[i]->GetF();
				cellIndex = i;
			}
		}
		if(cellIndex >= 0)
		{
			//std::cout << "Moving open to closed\n";
			nextCell = openList[cellIndex];
			openList.erase(openList.begin() + cellIndex);
			closedList.push_back(nextCell);
		}
		//std::cin.get();
		return nextCell;
	}

	void PathFinding::OpenCells(Vector2 newCell, float newCost, SearchCell *parent)
	{
		//std::cout << "new Cell: " << newCell.x << " " << newCell.y << std::endl;
		//std::cout << "goalCell: " << goalCell->position.x << " " << goalCell->position.y << std::endl;
		////std::cin.get();
		if(newCell.x < 0 || newCell.x > COLS -1 || newCell.y < 0 || newCell.y > ROWS - 1)
		{
			//std::cout << newCell.x << " " << newCell.y << "out of bounds\n";
			return;
		}

		if(tiles[newCell.y][newCell.x].IsObstacle())
		{
			//std::cout << "is obstacle\n";
			return;
		}

		for(unsigned int i = 0; i < closedList.size(); i++)
		{
			if(newCell == closedList[i]->position)
			{
				//std::cout << "In closed list\n";
				return;
			}
		}

		SearchCell *nextCell = new SearchCell(Vector2(newCell.x, newCell.y), parent);
		nextCell->G = newCost;
		nextCell->H = parent->ManhattanDistance(goalCell);

		for(unsigned int i = 0; i < openList.size(); i++)
		{
			if(nextCell->position == openList[i]->position)
			{
				float newF = nextCell->G + newCost + openList[i]->H;
				
				if(openList[i]->GetF() > newF)
				{
					openList[i]->G = nextCell->G + newCost;
					openList[i]->parent = nextCell;
				}
				else
				{
					delete nextCell;
					return;
				}
			}
		}
		openList.push_back(nextCell);
		//std::cout << "added nextCell to open list\n";
		//std::cin.get();
	}

	void PathFinding::ClearOpenList()
	{
		for(unsigned int i = 0; i < openList.size(); i++)
		{
			delete openList[i];
		}
		openList.clear();
	}
	
	void PathFinding::ClearClosedList()
	{
		for(unsigned int i = 0; i < closedList.size(); i++)
		{
			delete closedList[i];
		}
		closedList.clear();
	}
		
	void PathFinding::ClearPathList()
	{
		for(unsigned int i = 0; i < pathList.size(); i++)
		{
			delete pathList[i];
		}
		pathList.clear();
	}

}