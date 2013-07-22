#include "SearchCell.h"

namespace starlight
{
//NEED TO INCLUDE MAP SIZE FOR ID OF SEARCHCELL
	SearchCell::SearchCell(void) : parent(0)
	{}

	SearchCell::SearchCell(Vector2 pos, SearchCell *parent) :  
		position(pos),
		parent(parent), 
		G(0), 
		H(0)
	{}

	SearchCell::~SearchCell(void)
	{}

	int SearchCell::ManhattanDistance(SearchCell *targetCell)
	{
		int x = abs(position.x - targetCell->position.x);
		int y = abs(position.y - targetCell->position.y);
		return (x + y);
	}
	bool SearchCell::operator==(SearchCell &rhs)
	{
		return ((position.x == rhs.position.x) && (position.y == rhs.position.y));
	}

	float SearchCell::GetF()
	{
		return (G + H);
	}
}