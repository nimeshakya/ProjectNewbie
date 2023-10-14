#pragma once
#include <SDL.h>
#include "Window.h"
#include "BallSpawnner.h"
#include "Paddle.h"
#include "Border.h"
#include "Vector2.h"

enum class CollisionType
{
	NONE,
	TOP,
	MIDDLE,
	BOTTOM,
	LEFT,
	RIGHT
};

struct Contact
{
	CollisionType type;
	float penetration;
};

class Ball
{
public:
	const float BALL_VELOCITY{ 300.0f };

	Ball();
	Ball(SDL_Point spawnPoint);
	~Ball();

	// render ball on screen
	void render();

	// update ball state (mostly position)
	void update(float deltaTime, Paddle const& padLeft, Paddle const& padRight);

	Contact checkPaddleCollision(Paddle paddle);

	Contact checkWallCollision()

	void collideWithPaddle(Contact const& contact);

	inline SDL_Rect getRect()
	{
		return mBallRect;
	}
private: 
	SDL_Rect mBallRect;
	Vector2 position;
	Vector2 velocity;
};