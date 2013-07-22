#include "Projectile.h"
#include "Game.h"
#include "Character.h"

namespace starlight
{
	Projectile::Projectile(ALLEGRO_BITMAP *image, 
		const Vector2F &position, 
		const Vector2 &size,	
		const Vector2F &boundsOffset,
		const Direction &direction,
		const Vector2 &frameArraySize,
		const int &speed) : 
	GameObject(image, position, size, boundsOffset, Vector2(0, direction)),
		frameArraySize(frameArraySize),
		isAlive(false),
		speed(speed),
		frameCounter(0)
	{		
		SetMomentumToDirection(direction);
	}

	Projectile::~Projectile()
	{
	}

	void Projectile::Update(Map &map, Player &player)
	{
		if(GetBounds().Intersects(player.GetBounds()))
		{
			player.health -= 3;
			if(player.health < 0)
			{
				player.health = 0;
			}

			isAlive = false;
		}
		else 
		{
			Vector2 coords = GetCoords();

			const bool OUT_OF_RANGE = position.DistanceTo(player.position) > Game::WIDTH+1000 || map.IsOutOfBounds(coords);

			if(OUT_OF_RANGE || map.At(coords).IsObstacle())
			{
				isAlive = false;
			}
			else
			{
				position += (momentum * speed).toV2F(); // projectiles move at 2 times the source character's speed.
			}
		}
	}

	void Projectile::Draw()
	{
		GameObject::Draw();		

		if(!Game::gamePausedOn && isAlive)
		{	
			if(frameCounter++ == frameArraySize.x)
			{
				frameCounter = 0;

				if(++frameCoords.x == frameArraySize.x)
				{
					frameCoords.x = frameArraySize.x - 1;
				}
			}
		}
	}
}