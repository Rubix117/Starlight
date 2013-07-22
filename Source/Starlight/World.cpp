#include "World.h"
#include "GameMath.h"
#include "Enemy.h"
#include "Weapon.h"
#include "GameObjectFactory.h"

namespace starlight
{
	World::World(const int &ROWS, const int &COLS, const int &SMOOTH_LEVEL, const int &GRASS_CHANCE) :
		Map(ROWS, COLS), 
		SMOOTH_LEVEL(SMOOTH_LEVEL), 
		GRASS_CHANCE(GRASS_CHANCE)
	{	
		for(int y = 0; y < ROWS; y++)
		{
			for(int x = 0; x < COLS; x++)
			{
				Tile::TileType type = GetRandomTileType();
				At(x, y) = Tile(x, y, type);
			}
		}

		for(int i = 0; i < SMOOTH_LEVEL; i++)
		{
			if(i == SMOOTH_LEVEL - 1)
			{
				CreateBorder();
			}
			CellularAutomata();
		}

		int numDungeonsToCreate = GameMath::Random(ROWS/2, ROWS/3);
		for (int i = 0; i < numDungeonsToCreate; i++)
		{
			Vector2 newDungeonCoords;
			bool otherDungeonNear = true;

			while(otherDungeonNear)
			{
				otherDungeonNear = false;
				newDungeonCoords = *GetFreeTileCoords(COLS, ROWS);

				const int DUNGEON_PADDING = 3;
				int x = newDungeonCoords.x - DUNGEON_PADDING;
				int y = newDungeonCoords.y - DUNGEON_PADDING;
				int xMax = newDungeonCoords.x + DUNGEON_PADDING;
				int yMax = newDungeonCoords.y + DUNGEON_PADDING;

				for (; y < yMax; y++)
				{
					for (; x < xMax; x++)
					{
						if(!IsOutOfBounds(x, y) && At(x, y).type == Tile::DOOR 
							&& (x != newDungeonCoords.x && y != newDungeonCoords.y))
						{
							otherDungeonNear = true;
							break;
						}
					}
				}
			}

			At(newDungeonCoords).type = Tile::DOOR;
		}

		entryCoords = GetFreeTileCoords(COLS, ROWS);
		spawnCoords = entryCoords;
	}

	World::~World()
	{
	}

	Tile::TileType World::GetRandomTileType()
	{
		int randNum = GameMath::Random(100);

		if(randNum < GRASS_CHANCE)
		{
			return Tile::GRASS;
		}
		else
		{
			randNum = GameMath::Random(2);
			switch(randNum)
			{
			case 0:
				return Tile::ROCK;
			case 1:
				return Tile::TREE;
			}
		}
	}

	void World::CellularAutomata()
	{
		for(int x = 0; x < COLS; x++)
		{
			for(int y = 0; y < ROWS; y++)
			{
				At(x, y).type = GetAvgTileTypeAround(x, y);
			}
		}
	}

	void World::CreateBorder()
	{
		for (int y = 0; y < ROWS; y++)
		{
			for (int x = 0; x < COLS; x++)
			{
				if(x == 0 || y == 0 || x == COLS-1 || y == ROWS-1)
				{
					At(x, y).type = Tile::ROCK;
				}
			}
		}
	}

	Tile::TileType World::GetAvgTileTypeAround(const int &xCoord, const int &yCoord)
	{
		int adjacentGrass = 0;
		int adjacentRocks = 0;
		int adjacentTrees = 0;

		for(int y = yCoord-1; y <= yCoord+1; y++)
		{
			for(int x = xCoord-1; x <= xCoord+1; x++)
			{
				if(IsOutOfBounds(x, y) == false)
				{
					if(At(x, y).type == Tile::GRASS) 
					{
						adjacentGrass++;
					}
					else if(At(x, y).type == Tile::ROCK)
					{
						adjacentRocks++;
					}
					else
					{
						adjacentTrees++;
					}
				}
			}
		}

		if(adjacentGrass > (adjacentRocks + adjacentTrees))
		{
			return Tile::GRASS;
		}
		else if(adjacentRocks > adjacentTrees)
		{
			return Tile::ROCK;
		}
		else
		{
			return Tile::TREE;
		}
	}

	void World::GenerateEnemies(Player &player)
	{
		GameObjectFactory *factory = GameObjectFactory::GetInstance();
		const int numEnemies = GameMath::Random(100, 50);
		int x;
		int y;

		for(int i = 0; i < numEnemies; i++)
		{
			bool valid = false;

			while(!valid)
			{
				valid = true;

				do
				{
					x = rand() % COLS;
					y = rand() % ROWS;

				}while(At(x, y).IsObstacle());

				Vector2 enemyCoords(x, y);

				for(std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
				{
					if(enemyCoords == (*it)->GetCoords())
					{
						valid = false;
						break;
					}
				}

				if(valid)
				{
					Vector2F enemyPosition = enemyCoords.toV2F() * Tile::TILE_SIZE;

					if(enemyPosition.DistanceTo(player.position) >= 500)
					{
						Enemy *newEnemy = factory->CreateRandomWorldEnemy(enemyCoords, player.level);
						enemies.push_back(newEnemy);
					}
				}
			}
		}
	}

	void World::GenerateWeapons(Player &player)
	{
		GameObjectFactory *factory = GameObjectFactory::GetInstance();
		const int numberOfWeapons = GameMath::Random(100);
		int x;
		int y;

		for(int i = 0; i < numberOfWeapons; i++)
		{
			bool valid = false;

			while(!valid)
			{
				valid = true;
				do
				{
					x = rand() % COLS;
					y = rand() % ROWS;

				}while(At(x, y).IsObstacle());

				Vector2 weaponCoords(x, y);

				for(std::vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it)
				{
					if(weaponCoords == (*it)->GetCoords())
					{
						valid = false;
						break;
					}
				}

				if(valid)
				{
					Vector2F position = (weaponCoords*Tile::TILE_SIZE).toV2F();

					Weapon *newWeapon = factory->CreateRandomWeapon(position, player.level);
					weapons.push_back(newWeapon);
				}            
			}
		}
	}
}