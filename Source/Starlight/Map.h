#ifndef STARLIGHT_MAP_H_
#define STARLIGHT_MAP_H_

#include <iostream>
#include <vector>

#include <allegro5\allegro_primitives.h>

#include "Vector2.h"
#include "Tile.h"
#include "Weapon.h"
#include "Armour.h"
#include "Potion.h"

namespace starlight
{	
	class Player;
	class Enemy;

	class Map
	{
	public:
        Map();
		Map(const int &ROWS, const int &COLS);
		virtual ~Map();
		void Update(Player &player);
		void Draw(ALLEGRO_BITMAP *mapTileSheet, Player &player);
		bool IsOutOfBounds(const Vector2 &coords);
		bool IsOutOfBounds(const int &x, const int &y);
		
		Tile& At(const Vector2 &vec);
		Tile& At(const int &x, const int &y);
		const Tile& At(const Vector2 &vec) const;
		const Tile& At(const int &x, const int &y) const;
		friend std::ostream& operator<<(std::ostream &out, const Map &map);

		virtual void GenerateEnemies(Player &player);
		virtual void GenerateWeapons(Player &player);

		const int ROWS;
		const int COLS;

		Tile **tiles;
		Vector2 *entryCoords;
		Vector2 *exitCoords;
		Vector2 *spawnCoords;
		std::vector<Enemy*> enemies;
        std::vector<Weapon*> weapons;
        std::vector<Armour*> armour;
        std::vector<Potion*> potions;

	protected:
		Vector2* GetFreeTileCoords(const int &tilesWide, const int &tilesHigh, const int &srcX = 0, const int &srcY = 0);

	private:
		void DrawMap(ALLEGRO_BITMAP *mapTileSheet, Player &player);
		void DrawEnemies();
        void DrawWeapons();
        void DrawArmour();
        void DrawPotions();
		void DrawProjectiles();
	};
}

#endif //STARLIGHT_MAP_H_