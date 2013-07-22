#ifndef STARLIGHT_GAMEOBJECT_H_
#define STARLIGHT_GAMEOBJECT_H_

#include <allegro5\allegro.h>

#include "Vector2.h"
#include "Rectangle.h"

namespace starlight
{
	class GameObject
	{
	public:
		enum Direction
		{
			NORTH = 0, 
			NORTH_EAST, 
			EAST, 
			SOUTH_EAST, 
			SOUTH, 
			SOUTH_WEST, 
			WEST, 
			NORTH_WEST,

			MAX_DIRECTIONS
		};

		enum ObjectType
		{
			PLAYER_MELEE = 0,
			PLAYER_RANGED,
			PLAYER_MAGIC,
			PLAYER_END_INDEX,

			SPIDER,
			LAVA_TROLL,
			BARBARIAN,
			SKELETON,
			ZOMBIE,
			NINJA,
			ENEMY_END_INDEX,

			RANDOM_WEAPON,
			RANDOM_ARMOUR,
			RANDOM_POTION,

			MAX_OBJECT_TYPES
		};			

		GameObject(ALLEGRO_BITMAP *image,
			const Vector2F &position,
			const Vector2 &size,
			const Vector2F &boundsOffset,
			const Vector2 &frameCoords);

		virtual ~GameObject();
		virtual void Update();
		virtual void Draw();
		Rectangle GetBounds();
		Vector2 GetCoords() const;
		void SetMomentumToDirection(Direction direction);

		bool isVisible;
		Vector2F position;
		const Vector2 size;
		const Vector2F boundsOffset;
		Vector2 momentum;
		Vector2 frameCoords;

		ALLEGRO_BITMAP *image;
	};
}

#endif //STARLIGHT_GAMEOBJECT_H_