#ifndef STARLIGHT_PROJECTILE_H_
#define STARLIGHT_PROJECTILE_H_

#include "GameObject.h"
#include "Vector2.h"
#include "Map.h"
#include "Player.h"
#include "Character.h"

namespace starlight
{
	class Projectile : public GameObject
	{
	public:
		Projectile(ALLEGRO_BITMAP *image, 
			const Vector2F &position, 
			const Vector2 &size,	
			const Vector2F &boundsOffset,
			const Direction &direction,
			const Vector2 &frameArraySize,
			const int &speed);
		~Projectile();
		void Update(Map &map, Player &player);
		void Draw();

		bool isAlive;
		const Vector2 frameArraySize;
		int speed;
		int frameCounter;
	};
}

#endif // STARLIGHT_PROJECTILE_H_