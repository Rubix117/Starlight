#include <sstream>

#include <allegro5\allegro_native_dialog.h>

#include "GameObjectFactory.h"
#include "Vector2.h"
#include "GameMath.h"

namespace starlight
{
	GameObjectFactory *GameObjectFactory::instance = NULL;
	ALLEGRO_BITMAP *GameObjectFactory::weaponsSpriteSheet = NULL;
	ALLEGRO_BITMAP *GameObjectFactory::armourSpriteSheet = NULL;
	ALLEGRO_BITMAP *GameObjectFactory::potionsSpriteSheet = NULL;

	GameObjectFactory* GameObjectFactory::GetInstance()
	{
		if(instance == NULL)
		{
			instance = new GameObjectFactory();
		}

		return instance;
	}

	void GameObjectFactory::SetSpriteSheet(GameObject::ObjectType objectType, ALLEGRO_BITMAP *image)
	{
		if(objectType == GameObject::RANDOM_WEAPON)
		{
			weaponsSpriteSheet = image;
		}
		else if(objectType == GameObject::RANDOM_ARMOUR)
		{
			armourSpriteSheet = image;
		}
		else if(objectType == GameObject::RANDOM_POTION)
		{
			potionsSpriteSheet = image;
		}
		else
		{
			if(prototypes.find(objectType) == prototypes.end()) // Check if prototype exists.
			{
				std::cout <<"GameObjectFactory::SetSpriteSheet : Prototype is NULL. Make sure to setup all prototypes in GameObjectFactory constructor"<< std::endl;
			}
			else
			{
				Character *prototype = prototypes[objectType];
				prototype->image = image;
			}
		}
	}

	void GameObjectFactory::SetProjectileSpriteSheet(GameObject::ObjectType objectType, ALLEGRO_BITMAP *projectileImage)
	{
		Character *prototype = prototypes[objectType];
		if(prototype != NULL)
		{
			prototype->projectileImage = projectileImage;
		}
	}

	Player* GameObjectFactory::CreatePlayer(GameObject::ObjectType playerType, Vector2 coords, int level)
	{
		if(playerType >= 0 && playerType < GameObject::PLAYER_END_INDEX)
		{
			Player *p = GetCopyOf<Player>(playerType);
			p->position = coords.toV2F() * Tile::TILE_SIZE;
			p->level = level;

			return p;
		}
		else
		{
			std::cout <<"GameObjectFactory::CreatePlayer : Non-Player Type Entered"<< std::endl;
			return NULL;
		}
	}

	Enemy* GameObjectFactory::CreateEnemy(GameObject::ObjectType enemyType, Vector2 coords, int level)
	{
		if(enemyType > GameObject::PLAYER_END_INDEX && enemyType < GameObject::ENEMY_END_INDEX)
		{
			Enemy *e = GetCopyOf<Enemy>(enemyType);
			e->position = coords.toV2F() * Tile::TILE_SIZE;
			e->level = level;

			if(enemyType == GameObject::SPIDER)
			{
				e->projectileImage = prototypes[enemyType]->projectileImage;
			}

			return e;
		}
		else
		{
			std::cout <<"GameObjectFactory::CreatePlayer : Non-Enemy Type Entered"<< std::endl;
			return NULL;
		}
	}

	Enemy* GameObjectFactory::CreateRandomWorldEnemy(Vector2 coords, int level)
	{
		const int MAX_TYPES_WORLD_ENEMY = 3;
		int randomNumber = GameMath::Random(MAX_TYPES_WORLD_ENEMY);
		GameObject::ObjectType enemyType;

		switch(randomNumber)
		{
		case 0:
			enemyType = GameObject::SPIDER;
			break;
		case 1:
			enemyType = GameObject::LAVA_TROLL;
			break;
		case 2:
			enemyType = GameObject::BARBARIAN;
			break;
		default:
			std::cout <<"GameObjectFactory::CreateRandomWorldEnemy(...) : Invalid Random Number Selected"<< std::endl;
		}

		return CreateEnemy(enemyType, coords, level);
	}

	Enemy* GameObjectFactory::CreateRandomDungeonEnemy(Vector2 coords, int level)
	{
		const int MAX_TYPES_DUNGEON_ENEMY = 3;
		int randomNumber = GameMath::Random(MAX_TYPES_DUNGEON_ENEMY);
		GameObject::ObjectType enemyType;

		switch(randomNumber)
		{
		case 0:
			enemyType = GameObject::SKELETON;
			break;
		case 1:
			enemyType = GameObject::ZOMBIE;
			break;
		case 2:
			enemyType = GameObject::NINJA;
			break;
		default:
			std::cout <<"GameObjectFactory::CreateRandomDungeonEnemy(...) : Invalid Random Number Selected"<< std::endl;
		}

		return CreateEnemy(enemyType, coords, level);
	}

	Weapon* GameObjectFactory::CreateRandomWeapon(Vector2F position, int level)
	{
		const int WEAPON_WIDTH = 64;
		const int WEAPON_HEIGHT = 48;

		Vector2 positionOffset = (Vector2(Tile::TILE_SIZE, Tile::TILE_SIZE) - Vector2(WEAPON_WIDTH, WEAPON_HEIGHT)) / 2; // Used to place the spawned weapon in the centre of the tile as opposed to its top-left corner.
		position += positionOffset.toV2F();

		Weapon *newWeapon = new Weapon(weaponsSpriteSheet, position, Vector2(WEAPON_WIDTH, WEAPON_HEIGHT), Vector2F(), Vector2(), level);
		newWeapon->RandomiseWeapon();

		return newWeapon;
	}

	Armour* GameObjectFactory::CreateRandomArmour(Vector2F position, int level)
	{
		const int ARMOUR_WIDTH = 64;
		const int ARMOUR_HEIGHT = 48;

		Armour *newArmour = new Armour(armourSpriteSheet, position, Vector2(ARMOUR_WIDTH, ARMOUR_HEIGHT), Vector2F(), Vector2(), level);
		newArmour->RandomiseArmour();

		return newArmour;
	}

	Potion* GameObjectFactory::CreateRandomPotion(Vector2F position, int level)
	{
		const int POTION_WIDTH = 32;
		const int POTION_HEIGHT = 32;

		Potion *newPotion = new Potion(potionsSpriteSheet, position, Vector2(POTION_WIDTH, POTION_HEIGHT), Vector2F(), Vector2(), level);
		newPotion->RandomisePotion();

		return newPotion;
	}

	bool GameObjectFactory::AreAllSpriteSheetsSet()
	{
		if(!weaponsSpriteSheet)
		{
			std::cout << "weapon sprite missing" << std::endl;
			return false;
		}
		else if(!armourSpriteSheet)
		{
			std::cout << "armour sprite missing" << std::endl;
			return false;
		}
		else if(!potionsSpriteSheet)
		{
			std::cout << "potions sprite missing" << std::endl;
			return false;
		}
		else
		{
			for(std::map<GameObject::ObjectType, Character*>::iterator it = prototypes.begin(); it != prototypes.end(); ++it)
			{
				if(!it->second->image)
				{
					std::cout << "character sprite missing. ID: " << it->first << std::endl;
					return false;
				}
			}
		}

		return true;
	}

	GameObjectFactory::GameObjectFactory()
	{
		// All character classes must initialised here
		prototypes[GameObject::PLAYER_MELEE] = new Player(NULL, Vector2F(), Vector2(96, 96), Vector2F(16, 16), GameObject::SOUTH, Vector2(8, 8), 7.0);

		prototypes[GameObject::SPIDER] = new Enemy(NULL, NULL, Vector2F(), Vector2(96, 96), Vector2F(16, 16), GameObject::SOUTH, Vector2(8, 8), 7.0, "Spider");
		prototypes[GameObject::LAVA_TROLL] = new Enemy(NULL, NULL, Vector2F(), Vector2(96, 96), Vector2F(16, 16), GameObject::SOUTH, Vector2(8, 8), 5.0, "Lava Troll");
		prototypes[GameObject::BARBARIAN] = new Enemy(NULL, NULL, Vector2F(), Vector2(96, 96), Vector2F(16, 16), GameObject::SOUTH, Vector2(8, 8), 6.0, "Barbarian");
		prototypes[GameObject::SKELETON] = new Enemy(NULL, NULL, Vector2F(), Vector2(96, 96), Vector2F(16, 16), GameObject::SOUTH, Vector2(8, 8), 6.0, "Skeleton");
		prototypes[GameObject::ZOMBIE] = new Enemy(NULL, NULL, Vector2F(), Vector2(96, 96), Vector2F(16, 16), GameObject::SOUTH, Vector2(8, 8), 3.0, "Zombie");
		prototypes[GameObject::NINJA] = new Enemy(NULL, NULL, Vector2F(), Vector2(96, 96), Vector2F(16, 16), GameObject::SOUTH, Vector2(8, 8), 7.0, "Ninja");
	}

	GameObjectFactory::~GameObjectFactory()
	{
		for(std::map<GameObject::ObjectType, Character*>::iterator it = prototypes.begin(); it != prototypes.end(); ++it)
		{
			it = prototypes.erase(it);
		}
		prototypes.clear();

		delete instance;
	}

	template<typename T>
	T* GameObjectFactory::GetCopyOf(GameObject::ObjectType characterType)
	{
		Character *prototype = prototypes[characterType];
		T* a = new T(*dynamic_cast<T*>(prototype));

		return a;
	}
}