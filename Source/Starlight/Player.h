#ifndef STARLIGHT_PLAYER_H_
#define STARLIGHT_PLAYER_H_

#include <fstream>

#include "Character.h"
#include "Map.h"

namespace starlight
{
	class Player : public Character
	{
	public:
		Player(ALLEGRO_BITMAP *image,
			const Vector2F &position,
			const Vector2 &size,
			const Vector2F &boundsOffset,
			const Direction &direction,
			const Vector2 &frameArraySize,
			const float &speed);
		~Player();

		void Load(std::ifstream &fileIn);
		void Save(std::ofstream &fileOut);
		void Update(bool keys[], Map &map);
		void Draw();
		void LevelUp();
		int GetInventoryMaxSize();
		void EquipWeapon(Weapon *selectedWeapon);
		void EquipBreastPlate(Armour *selectedArmour);
		void EquipHelmet(Armour *selectedHelmet);
		void UseHealth(Potion* healthPotion);
		void UseMana(Potion* manaPotion);

		void UsePotion(Potion *selectedPotion);
		bool weaponEquipped;
		bool helmetEquipped;
		bool breastPlateEquipped;

		const Vector2 INVENTORY_SIZE;
		bool hasMoved;
		Vector2 previousCoords;
		Weapon* equippedWeapon;
		Armour* equippedBreastPlate;
		Armour* equippedHelmet;
		int defence;

		std::vector<Item*> inventory;
		std::vector<Item*> equipped;
	};
}

#endif //STARLIGHT_PLAYER_H_