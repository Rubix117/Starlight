#include <iostream>
using std::cout;

#include <allegro5\allegro_primitives.h>

#include "Character.h"
#include "Game.h"

namespace starlight
{
	Character::Character(ALLEGRO_BITMAP *image,
		ALLEGRO_BITMAP *projectileImage,
		const Vector2F &position,
		const Vector2 &size,
		const Vector2F &boundsOffset,
		const Direction &direction,
		const Vector2 &frameArraySize,
		const float &speed) :
	GameObject(image, position, size, boundsOffset,	Vector2(0, direction)),
		projectileImage(projectileImage),
		direction(direction),
		frameArraySize(frameArraySize),
		frameCounter(0),
		speed(speed),
		level(1),
		maxHealth(30), 
		health(30),
		maxMana(20),
		mana(20),
		attackCounter(0), 
		attackDelay(Game::FPS), 
		attacking(false),
		state(IDLE)
	{
		//HACK - Temp values
		experience = 0;
		strength = 10;
		dexterity = 10;
		constitution = 10;
		intelligence = 10;
		wisdom = 10;

		damage = strength/2;
	}

	Character::~Character()
	{
	}

	void Character::Draw()
	{
		GameObject::Draw();

		int healthBarSize = Tile::TILE_SIZE * (float(health)/maxHealth);
		al_draw_filled_rectangle(position.x, position.y, position.x + healthBarSize, position.y + 4, al_map_rgb(255, 0, 0));

		if(!Game::gamePausedOn && state != DEAD && frameCounter++ == frameArraySize.x)
		{
			frameCounter = 0;		
			++frameCoords.x %= frameArraySize.x;
		}	
	}

	void Character::SetAction(ActionState state)
	{
		this->state = state;
		frameCoords.y = (state * MAX_DIRECTIONS) + direction;

		if(state == IDLE)
		{
			momentum = Vector2(0, 0);
		}
		else if(state == MOVING)
		{
			attackCounter = 0;
		}
		else if(state == ATTACKING)
		{
			frameCoords.x = 0;
			momentum = Vector2(0, 0);
		}
		else if(state == DYING)
		{
			frameCoords.x = 0;
			momentum = Vector2(0, 0);
			attackCounter = 0;
			frameCounter = 0;
		}
		//else if(state == DEAD)
		//{
		//}
	}

	void Character::ChangeDirection(Direction direction)
	{
		this->direction = direction;
		SetMomentumToDirection(direction);
	}

	void Character::Rest()
	{
		frameCoords.y = (IDLE * MAX_DIRECTIONS) + direction;
		momentum = Vector2(0, 0);
	}

	void Character::Move(Map &map, Direction direction)
	{
		ChangeDirection(direction);
		frameCoords.y = (MOVING * MAX_DIRECTIONS) + direction;

		// Handle Horizontal Movement
		if (momentum.x != 0)
		{
			if(momentum.y != 0)
				position.x += momentum.x * speed * 0.705f; //0.705 represents the percentage of speed for diagonal movement
			else
				position.x += momentum.x * speed;

			handleCollisions(map, HORIZONTAL);
		}

		// Handle Vertical Movement
		if(momentum.y != 0)
		{
			if (momentum.x != 0)
				position.y += momentum.y * speed * 0.705f;
			else
				position.y += momentum.y * speed;

			handleCollisions(map, VERTICAL);
		}
	}

	void Character::Attack()
	{
		frameCoords.x = 0;
		frameCoords.y = (ATTACKING * MAX_DIRECTIONS) + direction;
		momentum = Vector2(0, 0);
	}

	void Character::Die()
	{
		frameCoords.x = 0;
		frameCoords.y = (DYING * MAX_DIRECTIONS) + direction;
		momentum = Vector2(0, 0);
	}

	void Character::handleCollisions(Map &map, CollisionType collisionType)
	{
		Rectangle bounds = GetBounds();
		int leftTile = bounds.Left() < 0 ? -1 : bounds.Left() / Tile::TILE_SIZE;
		int rightTile = ceil(bounds.Right() / Tile::TILE_SIZE) - 1;
		int topTile = bounds.Top() < 0 ? -1 : bounds.Top() / Tile::TILE_SIZE;
		int bottomTile = ceil(bounds.Bottom() / Tile::TILE_SIZE) - 1;

		for (int y = topTile; y <= bottomTile; y++)
		{
			for (int x = leftTile; x <= rightTile; x++)
			{
				if(map.IsOutOfBounds(x, y) || map.At(x, y).IsObstacle())
				{
					Vector2F depth;
					if (tileIntersectsPlayer(bounds, getTileBounds(x, y), collisionType, depth))
					{
						position += depth;
						bounds.position += depth;
						return;
					}
				}
			}
		}
	}

	Rectangle Character::getTileBounds(int x, int y)
	{
		return Rectangle(x * Tile::TILE_SIZE, y * Tile::TILE_SIZE, Tile::TILE_SIZE, Tile::TILE_SIZE);
	}

	bool Character::tileIntersectsPlayer(Rectangle characterBounds, Rectangle tileBounds, CollisionType collisionType, Vector2F &depth)
	{
		if(collisionType == VERTICAL)
			depth.y = characterBounds.GetVerticalIntersectionDepth(tileBounds);
		else
			depth.x = characterBounds.GetHorizontalIntersectionDepth(tileBounds);

		return depth.y != 0 || depth.x != 0;
	}

	Rectangle Character::GenerateHitBox(Direction direction)
	{
		const Vector2 HIT_THRESHOLD = size/3;
		const Vector2F hitPosition = position;

		switch(direction)
		{
		case NORTH:
			return Rectangle(Vector2F(position.x + HIT_THRESHOLD.x, position.y), HIT_THRESHOLD);
		case NORTH_EAST:
			return Rectangle(Vector2F(position.x + (HIT_THRESHOLD.x * 2), position.y), HIT_THRESHOLD);
		case EAST:
			return Rectangle(Vector2F(position.x + (HIT_THRESHOLD.x * 2), position.y + HIT_THRESHOLD.y), HIT_THRESHOLD);
		case SOUTH_EAST:
			return Rectangle(Vector2F(position.x + (HIT_THRESHOLD.x * 2), position.y + (HIT_THRESHOLD.y * 2)), HIT_THRESHOLD);
		case SOUTH:
			return Rectangle(Vector2F(position.x + HIT_THRESHOLD.x, position.y + (HIT_THRESHOLD.y * 2)), HIT_THRESHOLD);
		case SOUTH_WEST:
			return Rectangle(Vector2F(position.x, position.y + (HIT_THRESHOLD.y * 2)), HIT_THRESHOLD);
		case WEST:
			return Rectangle(Vector2F(position.x , position.y + HIT_THRESHOLD.y), HIT_THRESHOLD);
		case NORTH_WEST:
			return Rectangle(Vector2F(position.x, position.y), HIT_THRESHOLD);
		default:
			;
		}
	}
}