#ifndef STARLIGHT_GAMEMATH_H_
#define STARLIGHT_GAMEMATH_H_
#include <random>
#include "Vector2.h"

namespace starlight
{
	class GameMath
	{
	public:
		static const float GameMath::DEGREES_PER_RADIAN;

		static int Random(int max, int min = 0);
		static int Mod(int n, int modulus);
		static int GetAngleInDegrees(Vector2F centre, Vector2F target);
	};
}

#endif // STARLIGHT_GAMEMATH_H_