#include "WallBreaker.h"
#include "raylib.h"
#include <iostream>

/*
This is our first serious attempt at creating a game using Raylib + C++.

To do that, we will take the game created in class on Session 4 (Wall Breaker) and expand it with the following:

1- Include deltaTime in all speed, movement calculations to make sure
consistent speed is applied across devices


2- Develop a CircleToRoundRectangleCollision to detect the collision that happens
with the corners of the bricks


3- Depending on where the collision happens with the bricks, come up with a direction
that will affect the x and y of the ball's speed


4- Include an unlimited number of levels that are created procedurally.
Each time you spawn the game, a new layout should be displayed


5- Have pickups such as life that fall from certain bricks upon impact.
Collecting them by the player, will activate the reward.


6- Include a cheat. Such as a pickup that temporarily (for instance for the next 5 seconds)
[with a time feedback for the user] eliminates the reversal of speed upon impact with bricks.


7- Bonus -> Include sound effects
8- Bonus -> Find a way to put the game on your phone
*/




void WallBreaker::Main()
{

	InitWindow(screenWidth, screenHeight, "Wall Breaker");

	Start();

	SetTargetFPS(60); // This will become important

	while (!WindowShouldClose())
	{
		Update();
	}

	CloseWindow();
}

void WallBreaker::Start()
{
	player.statusAffect.hasGoThrough = false;
	player.statusAffect.goThroughTimerCounter = 0.0;
	timerCounter = 0.0;
	isBreakable = true;

	// Clearing out old resources
	lives.clear();
	cheats.clear();
	bricks.clear();

	// Level Generation
	LevelGeneration();

	// Player declaration
	player.position = Vector2{ screenWidth / 2, screenHeight * 9 / 10 };
	player.size = Vector2{ screenWidth / 10 , 20 };
	player.curLife = MAX_LIVES; // at the beginning
	player.statusAffect.goThroughTimerCounter = 0.0;
	player.statusAffect.hasGoThrough = false;

	// Ball position declaration
	ball.position = Vector2{ screenWidth / 2, screenHeight * 9 / 10 - 30 };
}

void WallBreaker::EvalCurFrame()
{
	if (gameOver || levelWon)
	{
		Restart();

		return;
	}

	if (IsKeyPressed(KEY_P))
		gamePaused = !gamePaused;

	if (gamePaused) return;

	// Ball position
	BallMovement();

	// Player position
	PlayerMovement();

	// Life powerup position
	LifeMovement();

	// Cheat powerup position
	CheatMovement();


	// Collision with the Bricks
	CollisionWithBricks();

	// Colision with the pedal
	CollisionWithPedal();

	// collision with the walls
	CollisionWithWalls();

	// Collision with the Life Powerup
	CollisionWithLife();

	// Collision with the Cheat Powerup
	CollisionWithCheat();


	// Cheat power up timer
	TimerCheatPowerUp();

	// Brick timer
	TimerBrick();

	// Check whether a player won or lost
	EndScenario();

	ball.prevPosition = ball.position;

}


void WallBreaker::DrawCurFrame()
{
	BeginDrawing();
	ClearBackground(BLACK);

	if (gameOver)
	{
		DrawText("Press Enter to play again",
			GetScreenWidth() / 2 - MeasureText("Press Enter to play again", 30) / 2,
			GetScreenHeight() / 2 - 15,
			30, GRAY);
	}
	else if (levelWon)
	{
		DrawText("You Won! Press Enter to go to next level!",
			GetScreenWidth() / 2 - MeasureText("You Won! Press Enter to go to next level!", 30) / 2,
			GetScreenHeight() / 2 - 15,
			30, GRAY);
	}
	else
	{
		player.Draw();
		ball.Draw();


		for (Brick b : bricks)
		{
			b.Draw();
			b.DrawHitBoxes();
		}

		for (LifePickUp l : lives)
		{
			l.Draw();
		}

		for (CheatPowerUp c : cheats)
		{
			c.Draw();
		}

		// draw player lives!
		for (int i = 0; i < MAX_LIVES; i++)
		{
			if (i < player.curLife)
				DrawRectangle(10 + 40 * i, screenHeight - 20, 30, 10, LIGHTGRAY);
			else
				DrawRectangle(10 + 40 * i, screenHeight - 20, 30, 10, GRAY);
		}

		if (player.statusAffect.hasGoThrough)
		{
			char buf[8];
			sprintf_s(buf, "%d", int(player.statusAffect.goThroughTimer - player.statusAffect.goThroughTimerCounter + 1));
			DrawText(buf, screenWidth - 100, screenHeight - 100, 12, BLUE);
		}
	}





	EndDrawing();
}

void WallBreaker::Update()
{
	EvalCurFrame();
	DrawCurFrame();
}

void WallBreaker::LevelGeneration()
{
	float spaceForBricks = screenWidth - (BRICKS_PER_ROW * GAP + GAP);
	brickSize = Vector2{ spaceForBricks / BRICKS_PER_ROW, BRICK_HEIGHT };

	bricks.clear();

	for (int row = 0; row < ROWS_OF_BRICKS; row++)
	{
		for (int col = 0; col < BRICKS_PER_ROW; col++)
		{
			if (rand() % 100 > row * 15)
			{
				float x = GAP + (GAP + brickSize.x) * col;
				float y = GAP + (GAP + brickSize.y) * row;

				Rectangle rect = Rectangle{ x, y, brickSize.x, brickSize.y };

				Brick brick = Brick{ colors[row], rect };

				// Corners

				// Making the corners 10th size of the box
				// By dividing brickSize x and y by 10

				brick.hitBoxes[topLeftCorner] = Rectangle{ x,y,brickSize.x / 10,brickSize.y / 10 };		// x and y are in default location
				brick.hitBoxes[botLeftCorner] = Rectangle{ x,y + brickSize.y - (brickSize.y / 10),brickSize.x / 10,brickSize.y / 10 };
				brick.hitBoxes[topRightCorner] = Rectangle{ x + brickSize.x - (brickSize.x / 10),y ,brickSize.x / 10,brickSize.y / 10 };
				brick.hitBoxes[botRightCorner] = Rectangle{ x + brickSize.x - (brickSize.x / 10), y + brickSize.y - (brickSize.y / 10),  brickSize.x / 10,brickSize.y / 10 };

				// Leaving a gap in between 

				brick.hitBoxes[top] = Rectangle{ x + brickSize.x / 9 , y ,  brickSize.x - float(brickSize.x / 4.5),  1 };
				brick.hitBoxes[bottom] = Rectangle{ x + brickSize.x / 9, y + brickSize.y,  brickSize.x - float(brickSize.x / 4.5),  1 };
				brick.hitBoxes[left] = Rectangle{ x , y + (brickSize.y / 9), 1, brickSize.y - float(brickSize.y / 4.5) };
				brick.hitBoxes[right] = Rectangle{ x + brickSize.x , y + (brickSize.y / 9), 1, brickSize.y - float(brickSize.y / 4.5) };


				bricks.push_back(brick);
			}
		}
	}
}

void WallBreaker::LifeMovement()
{
	for (int i = 0; i < lives.size(); i++)
	{
		lives[i].position.x += lives[i].speed.x;
		lives[i].position.y += lives[i].speed.y;
	}
}

void WallBreaker::CheatMovement()
{
	for (int i = 0; i < cheats.size(); i++)
	{
		cheats[i].position.x += cheats[i].speed.x;
		cheats[i].position.y += cheats[i].speed.y;
	}
}

void WallBreaker::PlayerMovement()
{
	if (IsKeyDown(KEY_LEFT))
		player.position.x -= PLAYER_MOVEMENT_SPEED * DELTA_TIME;

	// we have reached the far left
	if (player.position.x - player.size.x / 2 <= 0)
		player.position.x = player.size.x / 2; // stick it to the far left

	// we have reached the far right
	if (player.position.x + player.size.x / 2 >= screenWidth)
		player.position.x = screenWidth - player.size.x / 2; // stick it to the far right

	if (IsKeyDown(KEY_RIGHT))
		player.position.x += PLAYER_MOVEMENT_SPEED * DELTA_TIME;
}

void WallBreaker::EndScenario()
{
	if (player.curLife == 0)
		gameOver = true;
	else
	{
		if (bricks.size() == 0)
			levelWon = true;
	}
}

void WallBreaker::Restart()
{
	if (IsKeyPressed(KEY_ENTER))
	{
		gameOver = false;
		levelWon = false;
		ball.active = false;

		Start();

	}
}

void WallBreaker::BallMovement()
{
	if (!ball.active)
	{
		ball.position = Vector2{ player.position.x, screenHeight * 9 / 10 - 30 };

		if (IsKeyPressed(KEY_SPACE))
		{
			ball.active = true;
			ball.speed = Vector2{ 0, -5 * DELTA_TIME };
		}
	}
	else
	{
		ball.position.x += ball.speed.x;
		ball.position.y += ball.speed.y;
	}
}

void WallBreaker::TimerCheatPowerUp()
{
	if (player.statusAffect.hasGoThrough)
	{
		player.statusAffect.goThroughTimerCounter += GetFrameTime();

		if (player.statusAffect.goThroughTimerCounter >= player.statusAffect.goThroughTimer)
		{
			player.statusAffect.hasGoThrough = false;
			player.statusAffect.goThroughTimerCounter = 0.0;
		}
	}
}

void WallBreaker::SpawnPowerUp(Vector2 position)
{
	if (rand() % 100 > 75)
		SpawnLife(position);
	else if (rand() % 100 > 75)
		SpawnCheat(position);
}

void WallBreaker::SpawnLife(Vector2 position)
{
	lives.push_back(LifePickUp{ position, POWERUP_SPEED, 10, YELLOW });
}

void WallBreaker::SpawnCheat(Vector2 position)
{
	cheats.push_back(CheatPowerUp{ position, POWERUP_SPEED, 10, BLUE });
}

void WallBreaker::CollisionWithBricks()
{
	for (int i = 0; i < bricks.size(); i++)
	{
		if (CheckCollisionCircleRec(ball.position, ball.radius, bricks[i].rect))
		{
			if (isBreakable)
			{
				isBreakable = false;
				SpawnPowerUp(Vector2{ bricks[i].rect.x + brickSize.x/2, bricks[i].rect.y+brickSize.y/2 });
				int scenario = CollisionWithHitBox(bricks[i]);

				Vector2 hitBoxesPositions[8]{};
				for (int j = 0; j != end; j++)
				{
					hitBoxesPositions[j].x = bricks[i].hitBoxes[j].x;
					hitBoxesPositions[j].y = bricks[i].hitBoxes[j].y;
				}

				// delete the brick
				bricks.erase(bricks.begin() + i);

				if (!player.statusAffect.hasGoThrough)
					switch (scenario)
					{
					case topLeftCorner:
						if (ball.prevPosition.x < hitBoxesPositions[topLeftCorner].x)
						{
							ball.speed.x *= -1;
						}

						if (ball.prevPosition.y < hitBoxesPositions[topLeftCorner].y)
							ball.speed.y *= -1;

						break;

					case topRightCorner:
						if (ball.prevPosition.x > hitBoxesPositions[topRightCorner].x + (brickSize.x / 10))
						{
							ball.speed.x *= -1;
						}

						if (ball.prevPosition.y < hitBoxesPositions[topRightCorner].y)
							ball.speed.y *= -1;

						break;

					case botLeftCorner:
						if (ball.prevPosition.x < hitBoxesPositions[botLeftCorner].x)
						{
							ball.speed.x *= -1;
						}

						if (ball.prevPosition.y > hitBoxesPositions[botLeftCorner].y + (brickSize.y / 10))
							ball.speed.y *= -1;

						break;

					case botRightCorner:
						if (ball.prevPosition.x > hitBoxesPositions[botRightCorner].x + (brickSize.x / 10))
						{
							ball.speed.x *= -1;
						}

						if (ball.prevPosition.y > hitBoxesPositions[botRightCorner].y + (brickSize.y / 10))
							ball.speed.y *= -1;

						break;

					case top:
						ball.speed.y *= -1;
						break;

					case bottom:
						ball.speed.y *= -1;
						break;

					case left:
						ball.speed.x *= -1;
						break;

					case right:
						ball.speed.x *= -1;
						break;

					default:
						break;
					}

				break; // Because the ball might hit two bricks at the same frame		
			}

		}
	}
}

void WallBreaker::TimerBrick()
{
	if (!isBreakable)
	{
		timerCounter += GetFrameTime();
		if (timerCounter >= timer)
		{
			isBreakable = true;
			timerCounter = 0.0;
		}
	}
}

void WallBreaker::CollisionWithPedal()
{
	if (CheckCollisionCircleRec(ball.position, ball.radius, player.GetRect()))
	{
		if (ball.speed.y > 0) // we are going downwards
		{
			ball.speed.y *= -1;
			ball.speed.x = (ball.position.x - player.position.x) / (player.size.x / 10) * DELTA_TIME;
		}
	}
}

void WallBreaker::CollisionWithWalls()
{
	if ((ball.position.x + ball.radius >= screenWidth) || ball.position.x - ball.radius <= 0)
		ball.speed.x *= -1;
	if (ball.position.y - ball.radius <= 0) // we hit the top
		ball.speed.y *= -1;
	if (ball.position.y + ball.radius >= screenHeight)
	{
		player.curLife--;
		ball.active = false;
		ball.speed = Vector2{ 0 , 0 };
	}
}

void WallBreaker::CollisionWithLife()
{
	for (int i = 0; i < lives.size(); i++)
	{
		if (CheckCollisionCircleRec(lives[i].position, lives[i].radius, player.GetRect()))
		{
			lives.erase(lives.begin() + i);
			// Max 5 lives
			player.curLife = std::min(player.curLife + 1, 5);
		}
		else if (lives[i].position.y + lives[i].radius >= screenHeight)
			lives.erase(lives.begin() + i);
	}
}

void WallBreaker::CollisionWithCheat()
{
	for (int i = 0; i < cheats.size(); i++)
	{
		if (CheckCollisionCircleRec(cheats[i].position, cheats[i].radius, player.GetRect()))
		{
			cheats.erase(cheats.begin() + i);
			// Max 5 lives
			player.statusAffect.hasGoThrough = true;
			player.statusAffect.goThroughTimerCounter = 0.0;
		}
		else if (cheats[i].position.y + cheats[i].radius >= screenHeight)
			cheats.erase(cheats.begin() + i);
	}
}

int WallBreaker::CollisionWithHitBox(Brick brick)
{
	for (int i = 0; i != end; i++)
	{
		if (CheckCollisionCircleRec(ball.position, ball.radius, brick.hitBoxes[i]))
		{
			return i;
		}
	}
	// Just returning this so that the scenario does not go into default
	return botLeftCorner;
}
