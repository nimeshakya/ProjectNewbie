#ifndef BALL_H
#define BALL_H

#include "GameObject.h"
#include "Vec2.h"
#include "Paddle.h"
#include "Border.h"

struct Collision
{
	double penetration;
	double directionalChange;
};

class Ball : public GameObject
{
private:
	int BALL_SPEED;
	double m_velX, m_velY;
public:
	Ball(Vec2 position, int w = UNIT_LENGTH, int h = UNIT_LENGTH)
		: GameObject(position.x, position.y, w, h), BALL_SPEED{ 500 }, m_velX(0.0), m_velY(1.0)
	{}

	// handles all update of states
	void Update(double deltaTime);

	// handles all collision
	void HandleCollision(const Paddle& paddle1, const Paddle& paddle2, const Border& borderTop, const Border& borderBot);

	void CollideWithPaddle(Collision* collision);
	void CollideWithBorder(Collision* collision);

	// handles collision with paddle
	Collision* HandlePaddleCollision(const Paddle& paddle);

	// handles collision with border
	Collision* HandleBorderCollision(const Border& paddle);
};

#endif // !BALL_H
