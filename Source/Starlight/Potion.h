#ifndef STARLIGHT_POTION_H_
#define STARLIGHT_POTION_H_

#include <string>
#include <random>
#include <iostream>

#include <allegro5\allegro_primitives.h>

#include "Item.h"
#include "Vector2.h"

namespace starlight 
{
	class Potion : public Item
	{
	public:
		friend class GameObjectFactory;
		~Potion();
		void Load(std::ifstream &fileIn);
		void Save(std::ofstream &fileOut);

		int potency;
		int level;

	private:
		Potion(
			ALLEGRO_BITMAP *image, 
			const Vector2F &position, 
			const Vector2 &size,	
			const Vector2F &boundsOffset,	
			const Vector2 &frameCoords,
			int playerLevel);
		void RandomisePotion();

		static const std::string typeName[MAX_POTION_TYPES];
		static const int basePotency[MAX_ITEM_ELEMENTS];
	};
}

#endif // STARLIGHT_POTION_H_