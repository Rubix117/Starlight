#ifndef STARLIGHT_ARMOUR_H_
#define STARLIGHT_ARMOUR_H_

#include <string>
#include <random>
#include <iostream>

#include <allegro5\allegro_primitives.h>

#include "Item.h"
#include "Vector2.h"

namespace starlight 
{
	class Armour : public Item
	{
	public:
		friend class GameObjectFactory;
		~Armour();
		void Load(std::ifstream &fileIn);
		void Save(std::ofstream &fileOut);

		int defence;
		int level;

	private:
		Armour(
			ALLEGRO_BITMAP *image, 
			const Vector2F &position, 
			const Vector2 &size,	
			const Vector2F &boundsOffset,	
			const Vector2 &frameCoords,
			int playerLevel);
		void RandomiseArmour();

		static const std::string typeName[MAX_ARMOUR_TYPES];
		static const int baseDefence[MAX_ITEM_ELEMENTS];
	};
}

#endif // STARLIGHT_ARMOUR_H_