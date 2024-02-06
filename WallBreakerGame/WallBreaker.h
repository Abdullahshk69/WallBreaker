#pragma once
#include <vector>
#include "raylib.h"

#include <memory>

#ifndef WALLBREAKER_H
#define WALLBREAKER_H



// C++ random library
class WallBreaker
{
public:

#define MAX_LIVES 5
#define ROWS_OF_BRICKS 5
#define BRICKS_PER_ROW 8
#define GAP 8 // pixels
#define BRICK_HEIGHT 24
#define DELTA_TIME GetFrameTime() * 50	// Multiplying by a number because the speed becomes too slow
#define PLAYER_MOVEMENT_SPEED 5
#define POWERUP_SPEED Vector2{ 0, 3 * DELTA_TIME }

	

	typedef struct StatusAffect {
		bool hasGoThrough = false;
		int goThroughTimer = 5;
		int goThroughTimerCounter = 0;
	};

	typedef struct Player {
		Vector2 position; // it's the center
		Vector2 size;
		int curLife;
		StatusAffect statusAffect;

		Rectangle GetRect()
		{
			return Rectangle{ position.x - size.x / 2,
										 position.y - size.y / 2,
										 size.x,
										 size.y };
		}

		void Draw()
		{
			DrawRectangleRounded(GetRect(), 0.3, 8, DARKGREEN);
		}

	} Player;

	typedef struct Ball {
		Vector2 position;
		Vector2 prevPosition;
		Vector2 speed = Vector2{ 0, -5 };
		int radius = 8;
		bool active = false;


		void Draw()
		{
			DrawCircle(position.x, position.y, radius, MAROON);
		}
	} Ball;

	typedef struct Brick {
		Color color;
		Rectangle rect;
		Rectangle hitBoxes[8];
		void Draw() { DrawRectangleRounded(rect, 0.3f, 8, color); }
		void DrawHitBoxes()
		{
			for (int i = 0; i < 8; i++)
			{
				DrawRectangleRec(hitBoxes[i], WHITE);
			}
		}
	} Brick;
	
	typedef struct LifePickUp {
		Vector2 position;
		Vector2 speed;
		int radius;
		Color color;

		void Draw() { DrawCircle(position.x, position.y, radius, color); }

	} LifePickUp;

	typedef struct CheatPowerUp {
		Vector2 position;
		Vector2 speed;
		int radius;
		Color color;

		void Draw() { DrawCircle(position.x, position.y, radius, color); }

	} CheatPowerUp;

	enum Box {
		topLeftCorner,
		topRightCorner,
		botLeftCorner,
		botRightCorner,
		top,
		bottom,
		left,
		right,
		end
	};

	Color colors[ROWS_OF_BRICKS] = { RED, PURPLE, ORANGE, BLUE, GREEN };

	std::vector<Brick> bricks;
	std::vector<LifePickUp> life;
	

	Player player = { 0 };
	Ball ball = { 0 };

	Vector2 brickSize;

	bool gamePaused = false;
	bool gameOver = false;
	bool levelWon = false;


	static const int screenWidth = 800;
	static const int screenHeight = 450;


	// Default GOOD functions
	void Main();
	void Start();
	void EvalCurFrame();
	void DrawCurFrame();
	void Update();

	// Additional Functions
	void LevelGeneration();
	void LifeMovement();
	void PlayerMovement();
	void BallMovement();

	void TimerGoThrough();
	void SpawnLife(Vector2 position);

	void CollisionWithBricks();
	void CollisionWithPedal();
	void CollisionWithWalls();
	void CollisionWithLife();
	int CollisionWithHitBox(Brick brick);

	void EndScenario();
	void Restart();
};

#endif // !WALLBREAKER_H
