#ifndef STARLIGHT_WEAPON_H_
#define STARLIGHT_WEAPON_H_

#include <string>
#include <random>
#include <iostream>
#include <fstream>

#include <allegro5\allegro_primitives.h>

#include "Item.h"
#include "Vector2.h"

namespace starlight 
{
	class Weapon : public Item
	{
	public:
		friend class GameObjectFactory;
		~Weapon();
		void Load(std::ifstream &fileIn);
		void Save(std::ofstream &fileOut);

		int damage;
		int level;

	private:
		Weapon(
			ALLEGRO_BITMAP *image, 
			const Vector2F &position, 
			const Vector2 &size,	
			const Vector2F &boundsOffset,	
			const Vector2 &frameCoords,
			int playerLevel);
		void RandomiseWeapon();
		bool isVisible(Vector2F playerCords);

		static const std::string typeName[MAX_WEAPON_TYPES];
		static const int baseDamage[MAX_ITEM_ELEMENTS];
	};
}

#endif // STARLIGHT_WEAPON_H_