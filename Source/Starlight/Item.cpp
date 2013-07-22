#include "allegro5\allegro_primitives.h"

#include "Item.h"
#include "Armour.h"
#include "Potion.h"
#include "Weapon.h"

namespace starlight
{	
	const std::string Item::qualityName[MAX_ITEM_QUALITIES] = {"Poor", "Common", "Good", "Excellent"};
	const std::string Item::elementName[MAX_ITEM_ELEMENTS] = {"wooden", "iron", "steel", "silver", "magic-metal"};
	const double Item::conditionMod[MAX_ITEM_QUALITIES] = {0.7, 0.5, 1.10, 1.30};

	Item::Item(ALLEGRO_BITMAP *image, 
		const Vector2F &position, 
		const Vector2 &size,	
		const Vector2F &boundsOffset,
		const Vector2 &frameCoords) :
	GameObject(image, position, size, boundsOffset, frameCoords)
	{
	}

	Item::~Item()
	{
	}

	void Item::Load(std::ifstream &fileIn)
	{
	}

	void Item::Save(std::ofstream &fileOut)
	{
		fileOut << mainType <<" ";

		//if(mainType == ARMOUR_ITEM)
		//{
		//	Armour *armour = dynamic_cast<Armour*>(this);
		//	armour->Save(fileOut);
		//}
		//else if(mainType == POTION_ITEM)
		//{
		//	Potion *potion = dynamic_cast<Potion*>(this);
		//	potion->Save(fileOut);
		//}
		//else if(mainType == WEAPON_ITEM)
		//{
		//	Weapon *weapon = dynamic_cast<Weapon*>(this);
		//	weapon->Save(fileOut);
		//}
	}
}