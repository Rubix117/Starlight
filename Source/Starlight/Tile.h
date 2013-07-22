#ifndef STARLIGHT_TILE_H_
#define STARLIGHT_TILE_H_

#include <iostream>

#include <allegro5\allegro_primitives.h>

namespace starlight
{
	class Tile
	{
		friend class Map;
	public:	
		enum TileType
		{
			GRASS = 0,
			ROCK,
			WALL,
			CORRIDOR,
			FLOOR,
            DOOR,
			TREE,
            EMPTY,
			TILE_TYPES_MAX
		};
		static const int TILE_SIZE;
		static const char tileAscii[TILE_TYPES_MAX];
		static ALLEGRO_BITMAP* treeSpriteSheet;
		
	public:
		Tile();
		Tile(const int &x, const int &y, const TileType &type);
		~Tile();
		void Draw(ALLEGRO_BITMAP *mapTileSheet);
		bool IsObstacle();
		friend std::ostream& operator<<(std::ostream &out, const Tile &tile);
		
		int x, y;
		TileType type;
	};
}

#endif //STARLIGHT_TILE_H_