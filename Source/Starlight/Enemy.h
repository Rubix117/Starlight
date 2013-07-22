#ifndef STARLIGHT_ENEMY_H_
#define STARLIGHT_ENEMY_H_

#include "Character.h"
#include "Map.h"
#include "World.h"
#include "Player.h"
#include "PathFinding.h"
#include "Projectile.h"

namespace starlight
{
	class Enemy : public Character
	{
	public:
		Enemy();
		Enemy(ALLEGRO_BITMAP *image,
			ALLEGRO_BITMAP *projectileImage,
			const Vector2F &position,
			const Vector2 &size,
			const Vector2F &boundsOffset,
			const Direction &direction,
			const Vector2 &frameArraySize,
			const float &speed,
			const std::string &name);
		~Enemy();

		void SetDirectionToTarget(Vector2F target);
		void Update(Map &map, Player &player);
		void Draw();
		Vector2F NextPathPosition();
		void ClearPath();
		void DropLoot(Map &map, const Player &player);
		void PickRandomDirection();

		void FireProjectile();
		void UpdateProjectile(Map &map, Player &player);
		void DrawProjectile();

		bool isChasing;
		std::string name;
		ALLEGRO_BITMAP *projectileImage;
		Projectile *projectile;
		std::vector<Vector2F*> myPath;
	};
}

#endif //STARLIGHT_ENEMY_H_