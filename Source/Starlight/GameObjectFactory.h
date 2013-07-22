#ifndef STARLIGHT_GameObjectFactory_H_
#define STARLIGHT_GameObjectFactory_H_

#include <map>

#include <allegro5\allegro_primitives.h>

#include "GameObject.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Armour.h"
#include "Potion.h"
#include "Item.h"

namespace starlight
{
	class GameObjectFactory
	{
	public:
		~GameObjectFactory();
		static GameObjectFactory *GetInstance();
		
		void SetSpriteSheet(GameObject::ObjectType type, ALLEGRO_BITMAP *image);
		void SetProjectileSpriteSheet(GameObject::ObjectType type, ALLEGRO_BITMAP *image);
		Player* CreatePlayer(GameObject::ObjectType playerType, Vector2 coords, int level);
		Enemy* CreateEnemy(GameObject::ObjectType enemyType, Vector2 coords, int level);
		Enemy* CreateRandomWorldEnemy(Vector2 coords, int level);
		Enemy* CreateRandomDungeonEnemy(Vector2 coords, int level);
		Weapon* CreateRandomWeapon(Vector2F position, int level);
		Armour* CreateRandomArmour(Vector2F position, int level);
		Potion* CreateRandomPotion(Vector2F position, int level);
		bool AreAllSpriteSheetsSet();

	private:
		GameObjectFactory();

		template<typename T>
		T* GetCopyOf(GameObject::ObjectType playerType);
		
		static GameObjectFactory *instance;
		static ALLEGRO_BITMAP *weaponsSpriteSheet;
		static ALLEGRO_BITMAP *armourSpriteSheet;
		static ALLEGRO_BITMAP *potionsSpriteSheet;

		std::map<GameObject::ObjectType, Character*> prototypes;
	};
}

#endif // STARLIGHT_GameObjectFactory_H_