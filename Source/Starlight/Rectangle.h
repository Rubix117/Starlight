#ifndef STARLIGHT_RECTANGLE_H_
#define STARLIGHT_RECTANGLE_H_

#include "Vector2.h"

namespace starlight
{
	class Rectangle
	{
	public:
		Rectangle();
		Rectangle(const float x, const float y, const int width, const int height);
		Rectangle(const Vector2F &position, const Vector2 &size);
		~Rectangle();
		const float Top() const;
		const float Bottom() const;
		const float Left() const;
		const float Right() const;
		bool Contains(const int x, const int y) const;
		bool Intersects(const Rectangle &rectB) const;
		float GetHorizontalIntersectionDepth(Rectangle rectB);
		float GetVerticalIntersectionDepth(Rectangle rectB);

		Vector2F position;
		Vector2 size;
	};
}

#endif // STARLIGHT_RECTANGLE_H_