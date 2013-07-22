#include <iostream>
using namespace std;

#include "Dungeon.h"
#include "PathFinding.h"
#include "GameObject.h"
#include "GameMath.h"
#include "Rectangle.h"
#include "Enemy.h"
#include "Weapon.h"
#include "GameObjectFactory.h"

namespace starlight
{
	Room::Room(Rectangle roomBounds) :
		coords(roomBounds.position.toV2()), 
		width(roomBounds.size.x),
		height(roomBounds.size.y),
		doorCoords(coords)
	{
		AddDoor();
	}

	Room::Room()
	{
	}

	void Room::AddDoor()
	{
		int randomSide = GameMath::Random(Room::NUM_SIDES) * 2; // North = 0, East = 2, South = 4, West = 6
		doorCoords = coords;

		switch(randomSide)
		{
		case GameObject::WEST:
			doorCoords.y = GameMath::Random(height + coords.y - 2, coords.y + 2);
			break;
		case GameObject::NORTH:
			doorCoords.x = GameMath::Random(width + coords.x - 2, coords.x + 2);
			break;
		case GameObject::EAST:
			doorCoords.x += width - 1;
			doorCoords.y = GameMath::Random(height + coords.y - 2, coords.y + 2);
			break;
		case GameObject::SOUTH:
			doorCoords.x = GameMath::Random(width + coords.x - 2, coords.x + 2);
			doorCoords.y += height - 1;
			break;
		}
	}

	int Room::GetArea()
	{
		return (width - 2) * (height - 2); // width and height currently represent the length of the wall surrounding the room so both are offset by 2 to take out the WALL tiles from the equation
	}

	Dungeon::Dungeon(const int &ROWS, const int &COLS, const int &totalRooms, const int &ROOM_WALL_MAX_LENGTH, const int &ROOM_WALL_MIN_LENGTH, const int &ROOM_PADDING, const int &ROOM_CHANCE, const int &CORRIDOR_CHANCE) :
		Map(ROWS, COLS), 
		totalRooms(totalRooms),
		ROOM_WALL_MAX_LENGTH(ROOM_WALL_MAX_LENGTH), 
		ROOM_WALL_MIN_LENGTH(ROOM_WALL_MIN_LENGTH),
		ROOM_PADDING(ROOM_PADDING),
		ROOM_CHANCE(ROOM_CHANCE), 
		CORRIDOR_CHANCE(CORRIDOR_CHANCE)
	{
		for(int y = 0; y < ROWS; y++)
		{
			for(int x = 0; x < COLS; x++)
			{
				At(x, y) = Tile(x, y, Tile::EMPTY);
			}
		}

		roomList = new Room[totalRooms];

		PopulateMap();
		ConnectRooms();

		// Replace empty tiles with wall tiles
		for (int y = 0; y < ROWS; y++)
		{
			for (int x = 0; x < COLS; x++)
			{
				if(At(x, y).type == Tile::EMPTY)
				{
					At(x, y).type = Tile::WALL;
				}
			}
		}

		int tempIndex = GameMath::Random(totalRooms);
		Room tempRoom = roomList[tempIndex];
		entryCoords = GetFreeTileCoords(tempRoom.width - 2, tempRoom.height - 2, tempRoom.coords.x + 1, tempRoom.coords.y + 1);
		upperLevel.coords = entryCoords;
		At(*upperLevel.coords).type = Tile::DOOR;
		upperLevel.map = NULL;
		
		tempIndex = GameMath::Random(totalRooms);
		tempRoom = roomList[tempIndex];
		exitCoords = GetFreeTileCoords(tempRoom.width - 2, tempRoom.height - 2, tempRoom.coords.x + 1, tempRoom.coords.y + 1);
		lowerLevel.coords = exitCoords;
		At(*lowerLevel.coords).type = Tile::DOOR;
		lowerLevel.map = NULL;

		spawnCoords = entryCoords;
		//cout << *this << endl;
	}

	Dungeon::~Dungeon()
	{
		delete [] roomList;

		delete upperLevel.coords;
		delete upperLevel.map;

		delete lowerLevel.coords;
		delete lowerLevel.map;
	}

	void Dungeon::PopulateMap()
	{
		for (int i = 0; i < totalRooms; i++)
		{
			DrawRandomRoom(roomList[i]);
		}
	}

	void Dungeon::DrawRandomRoom(Room &room)
	{
		Rectangle roomBounds;
		bool isRoomObstructed = true;

		while(isRoomObstructed)
		{
			isRoomObstructed = false;

			do
			{
				roomBounds.size.x =	GameMath::Random(ROOM_WALL_MAX_LENGTH, ROOM_WALL_MIN_LENGTH);
				roomBounds.size.y = GameMath::Random(ROOM_WALL_MAX_LENGTH, ROOM_WALL_MIN_LENGTH);
				roomBounds.position.x = GameMath::Random(COLS - roomBounds.size.x);
				roomBounds.position.y = GameMath::Random(ROWS - roomBounds.size.y);

			}while(roomBounds.Right() > COLS || roomBounds.Bottom() > ROWS);

			// Checks if there is a room in the way
			for(int y = roomBounds.Top() - ROOM_PADDING ; y < roomBounds.Bottom() + ROOM_PADDING; y++)
			{
				for(int x = roomBounds.Left() - ROOM_PADDING; x < roomBounds.Right() + ROOM_PADDING; x++)
				{
					if(IsOutOfBounds(x, y) || At(x, y).IsObstacle())
					{
						isRoomObstructed = true;
						break;
					}
				}

				if(isRoomObstructed)
				{
					break;
				}
			}
		}

		room = Room(roomBounds);

		for(int y = roomBounds.Top(); y < roomBounds.Bottom(); y++)
		{
			for(int x = roomBounds.Left(); x < roomBounds.Right(); x++)
			{
				if(y == roomBounds.Top() || x == roomBounds.Left() || y == roomBounds.Bottom() - 1 || x == roomBounds.Right() - 1) // if edge of room
				{
					At(x, y).type = Tile::WALL;
				}
				else
				{
					At(x, y).type = Tile::FLOOR;
				}
			}
		}

		At(room.doorCoords).type = Tile::DOOR;
	}

	void Dungeon::ConnectRooms() //this uses path finding to connect the rooms together
	{
		PathFinding findRoomPath(*this, false);
		std::vector<Vector2_<float>*> corridorPath;

		for(int i = 0; i < totalRooms - 1; i++)
		{
			findRoomPath.FindPath(roomList[i].doorCoords.toV2F(), roomList[i+1].doorCoords.toV2F());
			corridorPath = findRoomPath.pathList;

			for(vector<Vector2F*>::iterator it = corridorPath.begin()+1; it != corridorPath.end()-1; ++it) // ...begin()+1 and ...end()-1 prevent the door tiles from being overwritten with corridor tiles
			{
				At( (*it)->toV2() ).type = Tile::CORRIDOR;
			}

			findRoomPath.ClearClosedList();
			findRoomPath.ClearOpenList();
			findRoomPath.ClearPathList();
		}
	}

	void Dungeon::GenerateEnemies(Player &player)
	{
		GameObjectFactory *factory = GameObjectFactory::GetInstance();

		for(int i = 0; i < totalRooms; i++)
		{
			Room room = roomList[i];
			int enemiesToCreate = (int) sqrt(room.GetArea());
			int numEnemiesCreated = 0;

			while(numEnemiesCreated < enemiesToCreate)
			{
				Vector2 enemyCoords = *GetFreeTileCoords(room.width, room.height, room.coords.x, room.coords.y);
				bool positionAlreadyTaken = false;

				for(std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
				{
					if(enemyCoords == (*it)->GetCoords())
					{
						positionAlreadyTaken = true;
						break;
					}
				}

				if(!positionAlreadyTaken)
				{
					Vector2F enemyPosition = enemyCoords.toV2F() * Tile::TILE_SIZE;

					Enemy *newEnemy = factory->CreateRandomDungeonEnemy(enemyCoords, player.level); //factory->CreateEnemy(GameObject::SPIDER, enemyCoords, player.level);
					enemies.push_back(newEnemy);
					numEnemiesCreated++;
				}
			}
		}
	}

	void Dungeon::GenerateWeapons(Player &player)
	{
		GameObjectFactory *factory = GameObjectFactory::GetInstance();

		for(int i = 0; i < totalRooms; i++)
		{
			Room room = roomList[i];
			int weaponsToCreate = (int) sqrt(room.GetArea());
			int numWeaponsCreated = 0;

			while(numWeaponsCreated < weaponsToCreate)
			{
				int x;
				int y;

				do
				{
					x = GameMath::Random(room.coords.x + room.width, room.coords.x);
					y = GameMath::Random(room.coords.y + room.height, room.coords.y);

				}while(At(x, y).IsObstacle());

				Vector2 weaponCoords(x, y);
				bool positionAlreadyTaken = false;

				for(std::vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it)
				{
					if(weaponCoords == (*it)->GetCoords())
					{
						positionAlreadyTaken = true;
						break;
					}
				}

				if(!positionAlreadyTaken)
				{
					Vector2F position = (weaponCoords*Tile::TILE_SIZE).toV2F();
					Weapon *newWeapon = factory->CreateRandomWeapon(position, player.level);

					weapons.push_back(newWeapon);
					numWeaponsCreated++;
				}
			}
		}
	}
}