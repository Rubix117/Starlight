#include "Enemy.h"
#include "Input.h"
#include "Game.h"
#include "GameMath.h"
#include "GameObjectFactory.h"
#include "Projectile.h"

namespace starlight
{
	Enemy::Enemy(ALLEGRO_BITMAP *image,
		ALLEGRO_BITMAP *projectileImage,
		const Vector2F &position,
		const Vector2 &size,
		const Vector2F &boundsOffset,
		const Direction &direction,
		const Vector2 &frameArraySize,
		const float &speed,
		const std::string &name) : 
	Character(image, projectileImage, position, size, boundsOffset, direction, frameArraySize, speed),
		projectileImage(projectileImage),
		projectile(NULL),
		isChasing(false),
		name(name)
	{
		isVisible = false;
	}

	Enemy::~Enemy()
	{
		ClearPath();
	}

	void Enemy::Update(Map &map, Player &player)
	{
		const int	FRAMES_PER_ANIMATION	= frameArraySize.x * frameArraySize.x;
		const float MIN_SCREEN_RADIUS		= (Game::WIDTH < Game::HEIGHT ? Game::WIDTH : Game::HEIGHT) / 2;
		const float DISTANCE_TO_PLAYER		= position.DistanceTo(player.position);
		const float ATTACK_RANGE			= size.x - (size.x/3);
		const float MAX_CHASING_THRESHOLD	= MIN_SCREEN_RADIUS;
		const float ENEMY_DRAW_DISTANCE		= MIN_SCREEN_RADIUS + 300;

		if(projectile != NULL)
		{
			UpdateProjectile(map, player);
		}

		if(health <= 0 && state != DYING)
		{
			SetAction(DYING);
		}

		if(state == IDLE)
		{
			if(DISTANCE_TO_PLAYER < ENEMY_DRAW_DISTANCE)
			{
				isVisible = true;			
			}
			else
			{
				isVisible = false;
			}

			if(DISTANCE_TO_PLAYER <= MAX_CHASING_THRESHOLD) 
			{
				if(projectileImage != NULL /*&& rand() % 100 > 85*/)
				{
					SetDirectionToTarget(player.position);
					FireProjectile();
				}

				state = MOVING;
				Rest();
			}
			else
			{
				if(++attackCounter > FRAMES_PER_ANIMATION)
				{
					PickRandomDirection();
					attackCounter = 0;
				}
				else
				{
					Move(map, direction);
				}
			}
		}
		else if(state == MOVING)
		{
			if(DISTANCE_TO_PLAYER > ATTACK_RANGE)
			{
				if(DISTANCE_TO_PLAYER > MAX_CHASING_THRESHOLD) //Exits threshold
				{
					state = IDLE;
					ClearPath();
					Rest();
				}
				else if(myPath.empty())
				{
					PathFinding path(map, true);
					myPath = path.FindPath(GetCoords().toV2F(), player.GetCoords().toV2F());
				}
				else
				{
					SetDirectionToTarget(NextPathPosition());
					Move(map, direction);
				}
			}
			else
			{
				Rest();
				state = ATTACKING;
			}
		}
		else if(state == ATTACKING)
		{
			SetDirectionToTarget(player.position);

			if(DISTANCE_TO_PLAYER > ATTACK_RANGE) //Exits attack range
			{
				Rest();
				state = MOVING;
				attackCounter = 0;
			}
			else if(attackCounter++ == FRAMES_PER_ANIMATION)
			{
				if(!attacking)
				{
					Attack();
					frameCoords.x = 0;
					frameCounter = 0;
					attacking = true;
				}
				else if(DISTANCE_TO_PLAYER < ATTACK_RANGE)
				{
					if((player.health -= player.strength/2) < 0)
					{
						player.health = 0;
					}
					Rest();
					attacking = false;
				}

				attackCounter = 0;
			}
		}
		else if(state == DYING)
		{
			if(attackCounter++ == FRAMES_PER_ANIMATION)
			{
				state = DEAD;
				DropLoot(map, player);
			}
		}
	}

	void Enemy::SetDirectionToTarget(Vector2F target)
	{
		int angle = GameMath::GetAngleInDegrees(position, target);
		angle -= 247;				// The direction North is between 247 and 292 degrees. This offsets the angle as if the min range of North started at 0 degrees.
		angle = GameMath::Mod(angle, 360);	// Convert angle that is negative or greater than 360 degrees to within the range 0 -> 360.

		ChangeDirection(Direction(angle/45));  // (angle/45) gives the direction to the target equivalent to the enum Direction in GameObject. 0 is NORTH, 1 is NORTH_EAST, ...etc.
	}

	void Enemy::Draw()
	{
		if(Game::showPathsOn && !myPath.empty() && !Game::gamePausedOn)
		{
			for(std::vector<Vector2F*>::iterator it = myPath.begin(); it != myPath.end(); ++it)
			{
				Rectangle tileBounds = getTileBounds((*it)->x, (*it)->y);
				al_draw_filled_rectangle(tileBounds.Left(), tileBounds.Top(), tileBounds.Right(), tileBounds.Bottom(), al_map_rgba(0, 0, 255, 10));
			}
		}

		if(projectile != NULL)
		{
			//al_draw_filled_rectangle(projectile->position.x, projectile->position.y, projectile->position.x+96, projectile->position.y+96, al_map_rgba_f(1, 0, 0, 0.1));
			DrawProjectile();
		}

		Character::Draw();
	}

	void Enemy::ClearPath()
	{
		for(size_t i = 0; i < myPath.size(); i++)
		{
			delete myPath[i];
		}
		myPath.clear();
	}

	Vector2F Enemy::NextPathPosition()
	{
		Vector2F nextPosition = (*myPath.back()) * Tile::TILE_SIZE;

		if(position.DistanceTo(nextPosition) < 5)
		{
			delete myPath.back();
			myPath.pop_back();
		}

		return nextPosition;
	}

	void Enemy::DropLoot(Map &map, const Player &player)
	{
		if(GameMath::Random(100) < 80)
		{
			GameObjectFactory *factory = GameObjectFactory::GetInstance();
			int itemChance = GameMath::Random(100);
			if(itemChance < 33)
			{
				Weapon *newWeapon = factory->CreateRandomWeapon(position, level);
				map.weapons.push_back(newWeapon);
			}
			else if(itemChance < 66) 
			{
				Armour *newArmour = factory->CreateRandomArmour(position, level);
				map.armour.push_back(newArmour);
			}
			else
			{
				Potion *newPotion = factory->CreateRandomPotion(position, level);
				map.potions.push_back(newPotion);
			}
		}
	}

	void Enemy::FireProjectile()
	{
		if(projectile == NULL)
		{
			projectile = new Projectile(projectileImage, position + size.toV2F()/2, Vector2(32, 32), Vector2F(7, 7), Direction(0), Vector2(8, 1), speed + 2);
		}

		if( !projectile->isAlive)
		{
			projectile->isAlive = true;
			projectile->SetMomentumToDirection(direction);
			projectile->position = position + size.toV2F()/2;
		}
	}

	void Enemy::UpdateProjectile(Map &map, Player &player)
	{
		if(projectile->isAlive)
		{
			projectile->Update(map, player);
		}
	}

	void Enemy::DrawProjectile()
	{
		if(projectile->isAlive)
		{
			projectile->Draw();
			//al_draw_filled_rectangle(position.x, position.y, Tile::TILE_SIZE, Tile::TILE_SIZE, al_map_rgba_f(1, 0, 0, 0.1));
		}
	}
	void Enemy::PickRandomDirection()
	{
		int randomDirection = GameMath::Random(GameObject::MAX_DIRECTIONS);
		ChangeDirection(Direction(randomDirection));
	}
}