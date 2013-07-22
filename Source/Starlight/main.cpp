#include "Game.h"

int main(int argc, char **argv)
{
	srand(time(NULL));		// Set the seed for the random number generator to the current time
	starlight::Game game(50, 79);

	return 0;
}