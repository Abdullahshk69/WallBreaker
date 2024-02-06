// Hello, Abdullah here!
// I implemented almost everything required minus the bonuses. 
// I would give myself 13.5, because i did not implement a procedural generation of a level.
// Overall I am happy with what I have created. Learned a lot about modularization and also found a ridiculous amount of bugs
// 
// I have given up on audio. It is just not working. 
// Very rarely collision on corners might work. I hotfixed it and I have not found any new bugs but who knows when it wanna break
//



#include "raylib.h"
#include "WallBreaker.h"
#include <ctime>
#include <iostream>


int main()
{
	srand(time(NULL));
	WallBreaker game;
	game.Main();



	return 0;
}