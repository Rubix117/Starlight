#ifndef STARLIGHT_CHARACTER_H_
#define STARLIGHT_CHARACTER_H_

#include <string>
#include <vector>

#include "GameObject.h"
#include "Item.h"
#include "Map.h"
#include "World.h"

namespace starlight
{
	class Character : public GameObject
	{
	public:
		enum ActionState
		{
			IDLE = 0,
			MOVING,
			ATTACKING, 
			DYING,
			DEAD
		};

		enum CollisionType
		{
			HORIZONTAL = 0, 
			VERTICAL
		};	

		Character(ALLEGRO_BITMAP *image,
			ALLEGRO_BITMAP *projectileImage,
			const Vector2F &position,
			const Vector2 &size,
			const Vector2F &boundsOffset,
			const Direction &direction,
			const Vector2 &frameArraySize,
			const float &speed
			);
		~Character();
		virtual void Draw();

		void SetAction(ActionState action);
		void ChangeDirection(Direction direction);
		void Rest();
		void Move(Map &map, Direction direction);
		void Attack();
		void Die();
		
		void handleCollisions(Map &map, CollisionType collisionType);
		Rectangle getTileBounds(int x, int y);
		bool tileIntersectsPlayer(Rectangle characterBounds, Rectangle tileBounds, CollisionType collisionType, Vector2F &depth);
		Rectangle GenerateHitBox(Direction direction);

		ALLEGRO_BITMAP *projectileImage;
		int level;
		int experience;
		int health;
		int maxHealth;
		int mana;
		int maxMana;

		int strength;
		int dexterity;
		int constitution; //health
		int intelligence;
		int wisdom; //mana

		int speed;
		int damage;
		
		ActionState state;
        int attackCounter;
        int attackDelay;
        bool attacking;

		Direction direction;
		const Vector2 frameArraySize;
		int frameCounter;
		int frameDelay;
	};
}

#endif // STARLIGHT_CHARACTER_H_