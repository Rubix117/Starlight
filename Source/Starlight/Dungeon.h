#ifndef STARLIGHT_DUNGEON_H_
#define STARLIGHT_DUNGEON_H_

#include <iostream>

#include "Vector2.h"
#include "PathFinding.h"
#include "Map.h"
#include "Player.h"

namespace starlight
{
	class Room
	{
	public:
		static const int NUM_SIDES = 4;

		Room(Rectangle roomBounds);
		Room();
		void AddDoor();
		int GetArea();

		Vector2 coords;
		int width;
		int height;

		Vector2 doorCoords;
	};

	class Dungeon : public Map
	{
	public:
		struct MAP_LINK
		{
			Vector2 *coords;
			Map *map;
		};

		Dungeon(const int &ROWS, const int &COLS, const int &totalRooms, 
			const int &ROOM_WALL_MAX_LENGTH = 12, 
			const int &ROOM_WALL_MIN_LENGTH = 7, 
			const int &ROOM_PADDING = 2, 
			const int &ROOM_CHANCE = 50, 
			const int &CORRIDOR_CHANCE = 40);
		~Dungeon();
		void PopulateMap();
		void DrawRandomRoom(Room &room);
		void ConnectRooms();
		void GenerateEnemies(Player &player);
        void GenerateWeapons(Player &player);
		
		MAP_LINK upperLevel;
		MAP_LINK lowerLevel;

	private:
		const int ROOM_WALL_MAX_LENGTH;
		const int ROOM_WALL_MIN_LENGTH;
		const int ROOM_PADDING;
		const int ROOM_CHANCE;
		const int CORRIDOR_CHANCE;

		int totalRooms;     
		Room *roomList;       
	};
}

#endif // STARLIGHT_DUNGEON_H_