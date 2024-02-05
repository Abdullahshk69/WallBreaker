#include "raylib.h"
#include "WallBreaker.h"
#include <ctime>


int main()
{
	srand(time(NULL));
	WallBreaker game;
	game.Main();



	return 0;
}