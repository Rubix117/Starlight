#ifndef STARLIGHT_ITEM_H_
#define STARLIGHT_ITEM_H_

#include <string>

#include "GameObject.h"

namespace starlight
{
	class Item : public GameObject
	{
	public:
		enum ItemType
		{
			HELMET = 0,
			BREASTPLATE,
			GAUNTLETS,
			GREAVES,
			BOOTS,

			MAX_ARMOUR_TYPES,

			MACE = 0,
			AXE,
			SWORD,
			BOW,

			MAX_WEAPON_TYPES,

			SMALL_HEALTH = 0,
			BIG_HEALTH,
			SMALL_MANA,
			BIG_MANA,

			MAX_POTION_TYPES,

			WEAPON_ITEM = 0,
			ARMOUR_ITEM,
			POTION_ITEM
		};

		enum ItemQuality
		{
			POOR,
			COMMON,
			GOOD,
			EXCELLENT,

			MAX_ITEM_QUALITIES
		};

		enum ItemElement
		{
			WOODEN,
			IRON,
			STEEL,
			SILVER,
			MAGIC_METAL,

			MAX_ITEM_ELEMENTS
		};

		~Item();
		virtual void Load(std::ifstream &fileIn);
		virtual void Save(std::ofstream &fileOut);

		std::string name;
		ItemType mainType;
		ItemType subtype;

	protected:
		Item(ALLEGRO_BITMAP *image, 
			const Vector2F &position, 
			const Vector2 &size,	
			const Vector2F &boundsOffset,
			const Vector2 &frameCoords);

		static const std::string qualityName[MAX_ITEM_QUALITIES];
		static const std::string elementName[MAX_ITEM_ELEMENTS];
		static const double conditionMod[MAX_ITEM_QUALITIES];

		ItemElement element;
		ItemQuality quality;
	};
}

#endif // STARLIGHT_ITEM_H_