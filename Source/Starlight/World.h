#ifndef STARLIGHT_WORLDMAP_H_
#define STARLIGHT_WORLDMAP_H_

#include "Map.h"

namespace starlight
{
	class World : public Map
	{
	public:
		World(const int &ROWS, const int &COLS, const int &SMOOTH_LEVEL = 3, const int &GRASS_CHANCE = 55);
		~World();
		void GenerateEnemies(Player &player);
        void GenerateWeapons(Player &player);

	private:
		void CellularAutomata();
		void CreateBorder();
		Tile::TileType GetRandomTileType();
		Tile::TileType GetAvgTileTypeAround(const int &x, const int &y);		

		const int GRASS_CHANCE;
		const int SMOOTH_LEVEL;
	};
}

#endif //STARLIGHT_WORLDMAP_H_