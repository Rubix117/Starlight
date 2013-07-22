#include "Tile.h"
#include "Game.h"

namespace starlight
{
	const int Tile::TILE_SIZE = 96;
	const char Tile::tileAscii[TILE_TYPES_MAX] = {'.', 'r', 'w', 'c', '_', '/', 't', '*'};
	ALLEGRO_BITMAP* Tile::treeSpriteSheet = NULL;

	Tile::Tile()
	{
	}

	Tile::Tile(const int &x, const int &y, const TileType &type) :
		x(x), y(y), 
		type(type)
	{
	}

	Tile::~Tile()
	{
	}

	void Tile::Draw(ALLEGRO_BITMAP *mapTileSheet)
	{
		if(Game::showWallsOn && IsObstacle())
		{
			al_draw_tinted_bitmap_region(mapTileSheet, al_map_rgba_f(1, 0, 1, 0.02), type*Tile::TILE_SIZE, 0, Tile::TILE_SIZE, Tile::TILE_SIZE, x*Tile::TILE_SIZE, y*Tile::TILE_SIZE, 0);
		}
		else if(type == TREE)
		{
			al_draw_bitmap_region(mapTileSheet, Tile::GRASS*Tile::TILE_SIZE, 0, Tile::TILE_SIZE, Tile::TILE_SIZE, x*Tile::TILE_SIZE, y*Tile::TILE_SIZE, 0);
			al_draw_bitmap_region(treeSpriteSheet, 0*Tile::TILE_SIZE, 0, Tile::TILE_SIZE, Tile::TILE_SIZE, x*Tile::TILE_SIZE, y*Tile::TILE_SIZE, 0);
		}
		else
		{
			al_draw_bitmap_region(mapTileSheet, type*Tile::TILE_SIZE, 0, Tile::TILE_SIZE, Tile::TILE_SIZE, x*Tile::TILE_SIZE, y*Tile::TILE_SIZE, 0);
		}
	}

	bool Tile::IsObstacle()
	{
		switch(type)
		{
		case FLOOR:
		case GRASS:
		case CORRIDOR:
		case DOOR:
		case EMPTY:
			return false;
		case WALL:
		case ROCK:
		case TREE:
		default:
			return true;
		}
	}

	std::ostream& operator<<(std::ostream &out, const Tile &tile)
	{
		return out << Tile::tileAscii[tile.type];
	}
}