#include <allegro5\allegro.h>

#include "GameMath.h"

namespace starlight
{
	const float GameMath::DEGREES_PER_RADIAN = 180 / ALLEGRO_PI;

	int GameMath::Random(int max, int min)
	{
		if(max == min)
			return max;

		return rand() % (max - min) + min;
	}

	int GameMath::Mod(int n, int modulus)
	{
		if(modulus < 0)
			return -1;

		n %= modulus;

		if(n >= 0)
			return n;
		else
			return n + modulus;
	}

	int GameMath::GetAngleInDegrees(Vector2F centre, Vector2F target)
	{
		Vector2F delta = target - centre;
		return atan2(delta.y, delta.x) * DEGREES_PER_RADIAN;
	}
}