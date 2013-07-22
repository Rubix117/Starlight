#include "Rectangle.h"
#include <cmath>

namespace starlight
{
	Rectangle::Rectangle()
	{
	}

	Rectangle::Rectangle(const float x, const float y, const int width, const int height) :
		position(x, y), size(width, height)
	{
	}

	Rectangle::Rectangle(const Vector2F &position, const Vector2 &size) :
		position(position), size(size)
	{
	}

	Rectangle::~Rectangle()
	{
	}

	const float Rectangle::Top() const
	{
		return position.y;
	}

	const float Rectangle::Bottom() const
	{
		return position.y + size.y;
	}

	const float Rectangle::Left() const
	{
		return position.x;
	}

	const float Rectangle::Right() const
	{
		return position.x + size.x;
	}

	bool Rectangle::Contains(const int x, const int y) const
	{
		return x >= Left() && x < Right() && y >= Top() && y < Bottom();
	}

	bool Rectangle::Intersects(const Rectangle &rectB) const
	{
		return Left() < rectB.Right() 
			&& Right() > rectB.Left()
			&& Top() < rectB.Bottom()
			&& Bottom() > rectB.Top();
	}

	float Rectangle::GetHorizontalIntersectionDepth(Rectangle rectB)
	{
		// Calculate half size of tiles.
		float halfWidthA = size.x/2.0f;
		float halfWidthB = rectB.size.x/2.0f;

		// Calculate centers.
		float centerA = Left() + halfWidthA;
		float centerB = rectB.Left() + halfWidthB;

		// Calculate actual and minimum non-intersecting distances between centers.
		float distanceX = centerA - centerB;
		float minDistanceX = halfWidthA + halfWidthB;

		// If we are not intersecting at all, return 0.0F.
		if (abs(distanceX) >= minDistanceX)
			return 0.0F;

		// Calculate and return intersection depths.
		return distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
	}

	float Rectangle::GetVerticalIntersectionDepth(Rectangle rectB)
	{
		// Calculate half sizes.
		float halfHeightA = size.y / 2.0f;
		float halfHeightB = rectB.size.y / 2.0f;

		// Calculate centers.
		float centerA = Top() + halfHeightA;
		float centerB = rectB.Top() + halfHeightB;

		// Calculate actual and minimum non-intersecting distances between centers.
		float distanceY = centerA - centerB;
		float minDistanceY = halfHeightA + halfHeightB;

		// If we are not intersecting at all, return 0.0F.
		if (abs(distanceY) >= minDistanceY)
			return 0.0F;

		// Calculate and return intersection depths.
		return distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
	}
}