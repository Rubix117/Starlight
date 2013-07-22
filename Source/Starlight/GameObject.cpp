#include "allegro5\allegro_primitives.h"

#include "GameObject.h"
#include "Game.h"

namespace starlight
{
	GameObject::GameObject(ALLEGRO_BITMAP *image, const Vector2F &position, const Vector2 &size, const Vector2F &boundsOffset, const Vector2 &frameCoords) :
		position(position),
		size(size),
		boundsOffset(boundsOffset),
		frameCoords(frameCoords),
		image(image)
	{
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Update()
	{
	}

	void GameObject::Draw()
	{
		Vector2 framePosition = frameCoords * size;

		al_draw_bitmap_region(image, framePosition.x, framePosition.y, size.x, size.y, position.x, position.y, 0);

		if(Game::showBoundsOn)
		{
			Rectangle bounds = GetBounds();
			al_draw_rectangle(bounds.Left(), bounds.Top(), bounds.Right(), bounds.Bottom(), al_map_rgba(255, 0, 255, 5), 2);
		}
	}

	Rectangle GameObject::GetBounds()
	{
		return Rectangle(position + boundsOffset, size - (boundsOffset.toV2() * 2));
	}

	Vector2 GameObject::GetCoords() const
	{
		return (position.toV2() + size/2) / Tile::TILE_SIZE;
	}

	void GameObject::SetMomentumToDirection(Direction direction)
	{
		switch(direction)
		{
		case NORTH:
			momentum = Vector2(0, -1);
			break;
		case NORTH_EAST:
			momentum = Vector2(1, -1);
			break;
		case EAST:
			momentum = Vector2(1, 0);
			break;
		case SOUTH_EAST:
			momentum = Vector2(1, 1);
			break;
		case SOUTH:	
			momentum = Vector2(0, 1);
			break;
		case SOUTH_WEST:
			momentum = Vector2(-1, 1);
			break;
		case WEST:
			momentum = Vector2(-1, 0);
			break;
		case NORTH_WEST:
			momentum = Vector2(-1, -1);
			break;
		}
	}
}