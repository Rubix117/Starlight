#include <sstream>

#include <allegro5\allegro_font.h>

#include "Player.h"
#include "Input.h"
#include "Game.h"
#include "Enemy.h"
#include "GameObjectFactory.h"

namespace starlight
{
	Player::Player(ALLEGRO_BITMAP *image,
		const Vector2F &position,
		const Vector2 &size,
		const Vector2F &boundsOffset,
		const Direction &direction,
		const Vector2 &frameArraySize,
		const float &speed) :
	Character(image, projectileImage, position, size, boundsOffset, direction, frameArraySize, speed),
		INVENTORY_SIZE(4, 5),		
        weaponEquipped(false),
        helmetEquipped(false),
        breastPlateEquipped(false),
        defence(0)
	{
	}

	Player::~Player()
	{
	}

	void Player::Load(std::ifstream &fileIn)
	{
		fileIn >> level
			>> experience
			>> health
			>> maxHealth
			>> mana
			>> maxMana
			>> strength
			>> dexterity
			>> constitution
			>> intelligence
			>> wisdom;

		int numInventoryItems;
		fileIn >> numInventoryItems;

		GameObjectFactory *factory = GameObjectFactory::GetInstance();
		for (int i = 0; i < numInventoryItems; i++)
		{
			int mainType;
			fileIn >> mainType;
			std::cout <<"Load "<< mainType << std::endl;

			if(mainType == Item::ARMOUR_ITEM)
			{
				Armour *armour = factory->CreateRandomArmour(Vector2F(), level);
				armour->Load(fileIn);
				inventory.push_back(armour);
			}
			else if(mainType == Item::POTION_ITEM)
			{
				Potion *potion = factory->CreateRandomPotion(Vector2F(), level);
				potion->Load(fileIn);
				inventory.push_back(potion);
			}
			else if(mainType == Item::WEAPON_ITEM)
			{
				Weapon *weapon = factory->CreateRandomWeapon(Vector2F(), level);
				weapon->Load(fileIn);
				inventory.push_back(weapon);
			}
		}
	}

	void Player::Save(std::ofstream &fileOut)
	{
		fileOut << level <<" "
			<< experience <<" "
			<< health <<" "
			<< maxHealth <<" "
			<< mana <<" "
			<< maxMana <<" "
			<< strength <<" "
			<< dexterity <<" "
			<< constitution <<" "
			<< intelligence <<" "
			<< wisdom <<" "
			<< inventory.size() <<" ";

		for(std::vector<Item*>::iterator it = inventory.begin(); it != inventory.end(); ++it)
		{
			(*it)->Save(fileOut);
		}
	}

	void Player::Update(bool keys[], Map &map)
	{
		const int FRAMES_PER_ANIMATION = frameArraySize.x * frameArraySize.x;
		hasMoved = false;

		if(health <= 0 && state != DYING && state != DEAD)
		{
			Die();
			state = DYING;
		}

		const bool moveUp		=	keys[KEY_UP]	|| keys[KEY_W];
		const bool moveLeft		=	keys[KEY_LEFT]	|| keys[KEY_A];
		const bool moveDown		=	keys[KEY_DOWN]	|| keys[KEY_S];
		const bool moveRight	=	keys[KEY_RIGHT] || keys[KEY_D];

		const bool attack		=	keys[KEY_SPACE];

		if(state == DYING || state == DEAD)
		{
			if(attackCounter++ == FRAMES_PER_ANIMATION)
			{
				state = DEAD;
				attackCounter = 0;
			}
		}
		else if(attack)
		{
			if(attacking == false)
			{
				attackCounter = 0;
				attacking = true;
				Attack();
			}
		}

		if(experience >= pow(2.0, level + 1))
		{
			LevelUp();   
		}

		if(state != DYING && state != DEAD)
		{
			if((moveUp || moveLeft || moveDown || moveRight || attacking) == false)
			{
				Rest();
			}
			else if(attacking)
			{
				if(attackCounter++ == FRAMES_PER_ANIMATION) 
				{
					for(std::vector<Enemy*>::iterator it = map.enemies.begin(); it != map.enemies.end(); ++it)
					{
						if(GenerateHitBox(direction).Intersects((*it)->GetBounds()))
						{
							(*it)->health -= 10;
						}
					}

					attacking = false;
					attackCounter = 0;
				}
			}
			else
			{
				Vector2 coords = GetCoords();
				if((coords == previousCoords) == false)
				{
					previousCoords = coords;
					hasMoved = true;
				}

				if(moveUp)
				{
					if(moveLeft)
					{
						Move(map, NORTH_WEST);
					}
					else if(moveRight)
					{
						Move(map, NORTH_EAST);
					}
					else
					{
						Move(map, NORTH);
					}
				}
				else if(moveDown)
				{
					if(moveLeft)
					{
						Move(map, SOUTH_WEST);
					}
					else if(moveRight)
					{
						Move(map, SOUTH_EAST);
					}
					else
					{
						Move(map, SOUTH);
					}
				}
				else if(moveLeft)
				{
					Move(map, WEST);
				}
				else if(moveRight)
				{
					Move(map, EAST);
				}
			}
		}
	}

	void Player::Draw()
	{
		Character::Draw();
	}

	void Player::LevelUp()
	{
		experience -= pow(2.0, level + 1);
		level++;

		maxHealth += constitution/2;
		health = maxHealth;

		maxMana += wisdom/2;
		mana = maxMana;

		strength++;
		dexterity++;
		constitution++;
		intelligence++;
		wisdom++;
	}

	int Player::GetInventoryMaxSize()
	{
		return INVENTORY_SIZE.x * INVENTORY_SIZE.y;
	}

    void Player::EquipWeapon(Weapon* selectedWeapon)
    {
        equippedWeapon = selectedWeapon;
        std::cout << equippedWeapon->damage;
        damage += equippedWeapon->damage;
    }

    void Player::EquipBreastPlate(Armour* selectedArmour)
    {
        breastPlateEquipped = true;
        equippedBreastPlate = selectedArmour;
        defence += equippedBreastPlate->defence;
    }

    void Player::EquipHelmet(Armour* selectedHelemet)
    {
        helmetEquipped = true;
        equippedHelmet = selectedHelemet;
        defence += equippedHelmet->defence;
    }

   void Player::UseHealth(Potion* healthPotion)
   {
        health += healthPotion->potency;
        if(health > maxHealth)
            health = maxHealth;
   }

   void Player::UseMana(Potion* manaPotion)
   {
       mana += manaPotion->potency;
       if(mana > maxMana)
           mana = maxMana;
   }
}