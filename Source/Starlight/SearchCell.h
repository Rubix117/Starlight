#ifndef STARLIGHT_SEARCHCELL_H_
#define STARLIGHT_SEARCHCELL_H_
#include <math.h>
#include "Vector2.h"

namespace starlight
{
	class SearchCell
	{
	public:
		SearchCell(void);
		SearchCell(Vector2 position, SearchCell *parent);
		~SearchCell(void);

		SearchCell *parent;
		float G;
		float H;
		float GetF();

		int ManhattanDistance(SearchCell *targetCell);
		bool operator==(SearchCell &rhs);
		Vector2 position;
	};
}
#endif

