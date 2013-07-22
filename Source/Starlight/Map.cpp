#include <conio.h>

#include "Map.h"
#include "Player.h"
#include "Game.h"
#include "Enemy.h"
#include "GameMath.h"

namespace starlight
{
	Map::Map(const int &ROWS, const int &COLS) :
		ROWS(ROWS), 
		COLS(COLS),
		entryCoords(NULL),
		exitCoords(NULL)
	{
		tiles = new Tile*[ROWS];
		for(int i = 0; i < ROWS; i++)
		{
			tiles[i] = new Tile[COLS];
		}
	}

	Map::~Map()
	{
		for(int i = 0; i < ROWS; i++)
		{
			delete [] tiles[i];
		}
		delete [] tiles;

		for(size_t i = 0; i < enemies.size(); i++)
		{
			delete enemies[i];
		}
		enemies.clear();

		for(size_t i = 0; i < weapons.size(); i++)
		{
			delete weapons[i];
		}
		weapons.clear();

		delete entryCoords;
		delete exitCoords;
	}

	void Map::Update(Player &player)
	{
		for(std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); )
		{
			if((*it)->state != Character::DEAD)
			{
				(*it)->Update(*this, player);
				++it;
			}
			else
			{
				player.experience += (*it)->level * 2;
				it = enemies.erase(it);
			}
		}
	}

	void Map::Draw(ALLEGRO_BITMAP *mapTileSheet, Player &player)
	{
		DrawMap(mapTileSheet, player);
		DrawWeapons();
		DrawEnemies();
        DrawArmour();
        DrawPotions();
	}

	bool Map::IsOutOfBounds(const Vector2 &coords)
	{
		return IsOutOfBounds(coords.x, coords.y);
	}

	bool Map::IsOutOfBounds(const int &x, const int &y)
	{
		if(x < 0 || x >= COLS || y < 0 || y >= ROWS)
			return true;
		else
			return false;
	}

	Vector2* Map::GetFreeTileCoords(const int &tilesWide, const int &tilesHigh, const int &srcX, const int &srcY)
	{
		int x;
		int y;
		do
		{
			x = GameMath::Random(srcX + tilesWide, srcX);
			y = GameMath::Random(srcY + tilesHigh, srcY);

		}while(At(x, y).IsObstacle());

		return new Vector2(x, y);
	}

	Tile& Map::At(const Vector2 &vec)
	{
		return tiles[vec.y][vec.x];
	}

	Tile& Map::At(const int &x, const int &y)
	{
		return tiles[y][x];
	}

	const Tile& Map::At(const Vector2 &vec) const
	{
		return tiles[vec.y][vec.x];
	}

	const Tile& Map::At(const int &x, const int &y) const
	{
		return tiles[y][x];
	}

	std::ostream& operator<<(std::ostream &out, const Map &map)
	{
		static HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
		GetConsoleScreenBufferInfo(hstdout, &consoleBufferInfo); // store current console buffer attributes

		for (int y = 0; y < map.ROWS; y++)
		{
			for (int x = 0; x < map.COLS; x++)
			{
				WORD colourAttribute;
				switch(map.At(x, y).type)
				{
				case Tile::FLOOR:
				case Tile::DOOR:
					colourAttribute = BACKGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_RED ;
					break;
				case Tile::GRASS:
					colourAttribute = BACKGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_GREEN;
					break;
				case Tile::CORRIDOR:
					colourAttribute = BACKGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_RED;
					break;
				case Tile::WALL:
				case Tile::ROCK:
					colourAttribute = BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
					break;
				case Tile::TREE:
					colourAttribute = BACKGROUND_GREEN | FOREGROUND_BLUE;
					break;
				default:
					colourAttribute = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				}

				SetConsoleTextAttribute(hstdout, colourAttribute);

				out << map.At(x, y);
			}
			out << std::endl;
		}

		SetConsoleTextAttribute(hstdout, consoleBufferInfo.wAttributes); // Reset console text colouring

		return out;
	}

	void Map::GenerateEnemies(Player &player)
	{
	}

	void Map::GenerateWeapons(Player &player)
	{
	}

	void Map::DrawMap(ALLEGRO_BITMAP *mapTileSheet, Player &player)
	{
		static const Vector2 DRAW_PADDING = Vector2(Game::WIDTH, Game::HEIGHT) / Tile::TILE_SIZE; // Stores how many tiles around the player to draw

		const int tileX = (player.position.x + player.size.x/2) / Tile::TILE_SIZE;
		const int tileY = (player.position.y + player.size.y/2) / Tile::TILE_SIZE;

		al_hold_bitmap_drawing(true);

		for(int y = tileY - DRAW_PADDING.y; y < tileY + DRAW_PADDING.y; y++)
		{
			for(int x = tileX - DRAW_PADDING.x; x < tileX + DRAW_PADDING.x; x++)
			{
				if(IsOutOfBounds(x, y) == false)
				{
					At(x, y).Draw(mapTileSheet);
				}
			}
		}

		al_hold_bitmap_drawing(false);
	}

	void Map::DrawEnemies()
	{
		for(std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		{
			if((*it)->isVisible)
			{
				(*it)->Draw();
			}
		}
	}

	void Map::DrawWeapons()
	{
		for(std::vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it)
		{
			(*it)->Draw();
		}
	}
	
    void Map::DrawArmour()
	{
        for(std::vector<Armour*>::iterator it = armour.begin(); it != armour.end(); ++it)
		{
			(*it)->Draw();
		}
	}

    void Map::DrawPotions()
	{
        for(std::vector<Potion*>::iterator it = potions.begin(); it != potions.end(); ++it)
		{
			(*it)->Draw();
		}
	}
}