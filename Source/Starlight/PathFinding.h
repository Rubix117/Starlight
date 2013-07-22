#ifndef STARLIGHT_PATHFINDING_H_
#define STARLIGHT_PATHFINDING_H_

#include <vector>

#include "Map.h"
#include "SearchCell.h"

namespace starlight
{
	class PathFinding
	{
	public:
		PathFinding(Map &map, bool isEnemy);
		~PathFinding(void);
		std::vector<Vector2F*> FindPath(Vector2F currentPos, Vector2F goalPos);
		bool goalReached;
		
		std::vector<Vector2F*> pathList;

		void ClearOpenList();
		void ClearClosedList();
		void ClearPathList();
	private:
		void SetStartAndGoal(SearchCell start, SearchCell goal);
		void ContinuePath();
		SearchCell* GetNextCell();
		void OpenCells(Vector2 newCell, float newCost, SearchCell *parent);

		Tile **tiles;
		bool isEnemy;

		const int ROWS;
		const int COLS;

		bool initStartAndGoal;
		

		SearchCell *startCell;
		SearchCell *goalCell;

		std::vector<SearchCell*> openList;
		std::vector<SearchCell*> closedList;		
	};
}

#endif // STARLIGHT_PATHFINDING_H_