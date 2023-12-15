#include "Ball.h"

#include <iostream>

void Ball::Update(double deltaTime)
{
	m_position += Vec2(m_velX, m_velY) * BALL_SPEED * deltaTime;
}

void Ball::HandleCollision(const Paddle& paddle1, const Paddle& paddle2, const Border& borderTop, const Border& borderBot)
{
	// Collision with paddle
	if (Collision* collision = HandlePaddleCollision(paddle1); collision != NULL)
	{
		CollideWithPaddle(collision);
	}
	else if (collision = HandlePaddleCollision(paddle2); collision != NULL)
	{
		CollideWithPaddle(collision);
	}

	// Collision with border
	if (Collision* collision = HandleBorderCollision(borderTop); collision != NULL)
	{
		CollideWithBorder(collision);
	}
	else if (collision = HandleBorderCollision(borderBot); collision != NULL)
	{
		CollideWithBorder(collision);
	}
}

void Ball::CollideWithPaddle(Collision* collision)
{
	// revert penetration
	m_position.x += collision->penetration > 0 ? collision->penetration + 5.0 : collision->penetration - 5.0;

	if (collision->collisionArea == CollisionArea::TOP)
	{
		std::cout << "COLLISION TOP\n";
		m_velY = -collision->directionalChange;
	}
	else if (collision->collisionArea == CollisionArea::MIDDLE)
	{
		std::cout << "COLLISION MIDDLE\n";
		m_velY = m_velY;
	}
	else
	{
		std::cout << "COLLISION BOTTOM\n";
		m_velY = collision->directionalChange;
	}

	m_velX *= -1;
}

void Ball::CollideWithBorder(Collision* collision)
{
	// revert penetration
	m_position.y += collision->penetration > 0 ? collision->penetration + 5.0 : collision->penetration - 5.0;
	m_velY *= -1;
}

Collision* Ball::HandlePaddleCollision(const Paddle& paddle)
{
	Collision* collision = NULL;

	double paddleLeft = paddle.getPosition().x;
	double paddleRight = paddle.getPosition().x + paddle.getWidth();
	double paddleTop = paddle.getPosition().y;
	double paddleBottom = paddle.getPosition().y + paddle.getHeight();

	double ballLeft = m_position.x;
	double ballRight = m_position.x + getWidth();
	double ballTop = m_position.y;
	double ballBottom = m_position.y + getHeight();

	if (ballLeft > paddleRight)
	{
		return collision;
	}
	if (ballRight < paddleLeft)
	{
		return collision;
	}
	if (ballTop > paddleBottom)
	{
		return collision;
	}
	if (ballBottom < paddleTop)
	{
		return collision;
	}

	collision = new Collision{ 0.0, 0.0, CollisionArea::NONE};

	// calculation of penetration for each paddle (left and right)
	if (m_velX > 0) // mopving right
	{
		// right paddle
		collision->penetration = paddleLeft - ballRight;
	}
	else if (m_velX < 0) // moving left
	{
		// left paddle
		collision->penetration = paddleRight - ballLeft;
	}

	// calculation of where ball hit paddle
	double paddleMiddle{ paddleTop + (paddleBottom - paddleTop) / 2 };

	double ballMiddle{ ballTop + (ballBottom - ballTop) / 2 };

	double maxBallMidPaddleMidDistance{ (ballMiddle - ballTop) + paddleBottom - paddleMiddle };

	if (ballMiddle < paddleMiddle)
	{
		collision->collisionArea = CollisionArea::TOP;
	}
	else if (ballMiddle > paddleMiddle)
	{
		collision->collisionArea = CollisionArea::BOTTOM;
	}
	else
	{
		collision->collisionArea = CollisionArea::MIDDLE;
	}

	collision->directionalChange = abs((paddleMiddle - ballMiddle) / maxBallMidPaddleMidDistance);

	return collision;
}

Collision* Ball::HandleBorderCollision(const Border& border)
{
	Collision* collision = NULL;

	double borderLeft = border.getPosition().x;
	double borderRight = border.getPosition().x + border.getWidth();
	double borderTop = border.getPosition().y;
	double borderBottom = border.getPosition().y + border.getHeight();

	double ballLeft = m_position.x;
	double ballRight = m_position.x + getWidth();
	double ballTop = m_position.y;
	double ballBottom = m_position.y + getHeight();

	if (ballLeft > borderRight)
	{
		return collision;
	}
	if (ballRight < borderLeft)
	{
		return collision;
	}
	if (ballTop > borderBottom)
	{
		return collision;
	}
	if (ballBottom < borderTop)
	{
		return collision;
	}

	collision = new Collision{ 0.0, 0.0 };

	// calculation of penetration
	if (m_velY > 0)
	{
		collision->penetration = borderTop - ballBottom;
	}
	else if (m_velY < 0)
	{
		collision->penetration = borderBottom - ballTop;
	}

	return collision;
}