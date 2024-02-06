#pragma once
#include "WallBreaker.h"

class PowerUp
{
public:
	PowerUp();
	PowerUp(Vector2 position);
	virtual void PickUp(typename Player &player);
	void Draw();

	Vector2 position;
	Vector2 speed;
	int radius;
	Color color;
};

class PU_Life : PowerUp
{
public:
	PU_Life(Vector2 position);
	void PickUp(typename Player& player);

	
};

class PU_GoThrough : PowerUp
{
public:
	PU_GoThrough(Vector2 position);
	void PickUp(Player& player);
};