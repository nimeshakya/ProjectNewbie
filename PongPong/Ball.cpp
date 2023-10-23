#include "Ball.h"
#include <iostream>
#include <SDL.h>

Ball::Ball()
{
	std::cout << "this ran\n";
	mBallRect = { (SCREEN_WIDTH - UNIT_SIZE) / 2, (SCREEN_HEIGHT - UNIT_SIZE) / 2, UNIT_SIZE, UNIT_SIZE };

	// set position of ball to vector we created
	position.x = (float)mBallRect.x;
	position.y = (float)mBallRect.y;

	velocity.x = (float)BALL_VELOCITY;
	velocity.y = 0.0f;
}

Ball::Ball(SDL_Point spawnPoint)
{
	mBallRect = { spawnPoint.x, spawnPoint.y, UNIT_SIZE, UNIT_SIZE };

	// set position of ball to vector we created
	position.x = (float)mBallRect.x;
	position.y = (float)mBallRect.y;

	velocity.x = (float)BALL_VELOCITY;
	velocity.y = 0.0f;
}

Ball::~Ball() {}

void Ball::update(float deltaTime, Paddle const& padLeft, Paddle const& padRight, BallSpawnner const& spawnner, std::mt19937 generator)
{
	position += velocity * deltaTime;

	// check collisions
	if (Contact contact = checkPaddleCollision(padLeft); contact.type != CollisionType::NONE)
	{
		collideWithPaddle(contact);
	}
	else if (contact = checkPaddleCollision(padRight); contact.type != CollisionType::NONE)
	{
		collideWithPaddle(contact);
	}
	else if (contact = checkWallCollision(); contact.type != CollisionType::NONE)
	{
		collideWithWall(contact, spawnner, generator);
	}

}

Contact Ball::checkPaddleCollision(Paddle paddle)
{
	float ballLeft = position.x;
	float ballRight = position.x + mBallRect.w;
	float ballTop = position.y;
	float ballBottom = position.y + mBallRect.h;

	SDL_Rect paddleRect = paddle.getRect();
	float paddleLeft = paddleRect.x;
	float paddleRight = paddleRect.x + paddleRect.w;
	float paddleTop = paddleRect.y;
	float paddleBottom = paddleRect.y + paddleRect.h;

	Contact contact{};

	if (ballLeft >= paddleRight)
	{
		return contact;
	}
	if (ballRight <= paddleLeft)
	{
		return contact;
	}
	if (ballTop >= paddleBottom)
	{
		return contact;
	}
	if (ballBottom <= paddleTop)
	{
		return contact;
	}

	float paddleRangeUpper{ paddleBottom - (2.0f * paddleRect.h / 3.0f) };
	float paddleRangeMiddle{ paddleBottom - (paddleRect.h) / 3.0f };

	if (velocity.x < 0.0f)
	{
		// left paddle
		contact.penetration = paddleRight - ballLeft;
	}
	else if (velocity.x > 0.0f)
	{
		// right paddle
		contact.penetration = paddleLeft - ballRight;
	}

	if ((ballBottom > paddleTop) && (ballBottom < paddleRangeUpper))
	{
		contact.type = CollisionType::TOP;
	}
	else if ((ballBottom > paddleRangeUpper) && (ballBottom < paddleRangeMiddle))
	{
		contact.type = CollisionType::MIDDLE;
	}
	else
	{
		contact.type = CollisionType::BOTTOM;
	}

	return contact;
}

Contact Ball::checkWallCollision()
{
	float ballLeft = position.x;
	float ballRight = position.x + mBallRect.w;
	float ballTop = position.y;
	float ballBottom = position.y + mBallRect.h;

	float borderLeft = 0;
	float borderRight = SCREEN_WIDTH;
	float borderTop = UNIT_SIZE;
	float borderBottom = SCREEN_HEIGHT - UNIT_SIZE;

	Contact contact{};

	if (ballLeft < borderLeft)
	{
		contact.type = CollisionType::LEFT;
	}
	else if (ballRight > borderRight)
	{
		contact.type = CollisionType::RIGHT;
	}
	else if (ballTop < borderTop)
	{
		contact.type = CollisionType::TOP;
		contact.penetration = borderTop - ballTop;
	}
	else if (ballBottom > borderBottom)
	{
		contact.type = CollisionType::BOTTOM;
		contact.penetration = borderBottom - ballBottom;
	}

	return contact;
}

void Ball::collideWithPaddle(Contact const& contact)
{
	position.x += contact.penetration;
	velocity.x = -velocity.x;

	if (contact.type == CollisionType::TOP)
	{
		velocity.y = -.75f * BALL_VELOCITY;
	}
	else if (contact.type == CollisionType::BOTTOM)
	{
		velocity.y = 0.75f * BALL_VELOCITY;
	}
}

void Ball::collideWithWall(Contact const& contact, BallSpawnner spawnner, std::mt19937 generator)
{
	if ((contact.type == CollisionType::TOP) || (contact.type == CollisionType::BOTTOM))
	{
		position.y += contact.penetration;
		velocity.y = -velocity.y;
	}
	else if (contact.type == CollisionType::LEFT)
	{
		SDL_Point newSpawnPoint{ spawnner.getSpawnPoint(generator) };
		position.x = newSpawnPoint.x;
		position.y = newSpawnPoint.y;
		velocity.x = BALL_VELOCITY;
		velocity.y = 0.75f * BALL_VELOCITY;
	}
	else if (contact.type == CollisionType::RIGHT)
	{
		SDL_Point newSpawnPoint{ spawnner.getSpawnPoint(generator) };
		position.x = newSpawnPoint.x;
		position.y = newSpawnPoint.y;
		velocity.x = -BALL_VELOCITY;
		velocity.y = 0.75 * BALL_VELOCITY;
	}
}

void Ball::render()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	mBallRect.x = static_cast<int>(position.x);
	mBallRect.y = static_cast<int>(position.y);
	SDL_RenderFillRect(gRenderer, &mBallRect);
}