#include <sstream>
#include <ctime>
#include <fstream>

#include "Armour.h"
#include "GameMath.h"

namespace starlight 
{
	const std::string Armour::typeName[MAX_ARMOUR_TYPES] = {"helmet", "breastplate", "gauntlets", "greaves", "boots"};
	const int Armour::baseDefence[MAX_ITEM_ELEMENTS] = {5, 15, 30, 50, 70};

	Armour::Armour(ALLEGRO_BITMAP *image, 
		const Vector2F &position, 
		const Vector2 &size,	
		const Vector2F &boundsOffset,	
		const Vector2 &frameCoords,
		int level) : 
	Item(image, position, size, boundsOffset, frameCoords), level(level)
	{
		mainType = ARMOUR_ITEM;
	}

	Armour::~Armour()
	{
	}

	void Armour::Load(std::ifstream &fileIn)
	{
		fileIn >> level;

		int temp;
		fileIn >> temp;
		element = ItemElement(temp);

		fileIn >> temp;
		subtype = ItemType(temp);

		std::cout << level <<" "<< element <<" "<< subtype << std::endl;
	}

	void Armour::Save(std::ofstream &fileOut)
	{
		Item::Save(fileOut);

		fileOut << level <<" "
			<< element <<" "
			<< subtype <<" ";
	}

	void Armour::RandomiseArmour()
	{
		// Set type, element and subclass. Element and type define the frame coordinates used to draw the weapon.
		subtype = ItemType(GameMath::Random(MAX_ARMOUR_TYPES));
		int elementChance = (level/2) + GameMath::Random(75);
		int subClassChance = (level/2) + GameMath::Random(65);

		if(elementChance < 20)
			element = WOODEN;                   
		else if(elementChance < 50)
			element = IRON;
		else if(elementChance < 65)
			element = STEEL;
		else if(elementChance < 85)
			element = SILVER;
		else
			element = MAGIC_METAL;

		if(subClassChance < 25)
			quality = POOR;
		else if(subClassChance < 50) 
			quality = COMMON;
		else if(subClassChance < 70)
			quality = GOOD;
		else
			quality = EXCELLENT;
		
		frameCoords = Vector2(element, 2*subtype); // Because the spritesheet contains 2 rows per weapon so 2*type specifys to use the second row of each weapon's sprites
		
		// Set Damage
		defence = baseDefence[element] * conditionMod[quality];

		// Set Name
		name = qualityName[quality] +' '+ elementName[element] +' '+ typeName[subtype];
	}
}