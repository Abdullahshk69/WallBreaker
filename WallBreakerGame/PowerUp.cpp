#include "PowerUp.h"
#include "WallBreaker.h"

PowerUp::PowerUp()
{
	speed = POWERUP_SPEED;
	radius = 10;
}

PowerUp::PowerUp(Vector2 position)
{
	this->position = position;
	speed = POWERUP_SPEED;
	radius = 10;
	color = BROWN;
}

void PowerUp::PickUp(typename Player& player)
{
	// This is just a placeholder
}

void PowerUp::Draw()
{
	DrawCircle(position.x, position.y, radius, color);
}

PU_Life::PU_Life(Vector2 position)
{
	this->position = position;
	color = YELLOW;
}

void PU_Life::PickUp(typename Player& player)
{
	player.curLife = std::min(player.curLife + 1, 5);
}

PU_GoThrough::PU_GoThrough(Vector2 position)
{
	this->position = position;
	color = PURPLE;
}

void PU_GoThrough::PickUp(typename Player& player)
{
	player.statusAffect.hasGoThrough = true;
}
