#ifndef PADDLE_H
#define PADDLE_H

#include "Window.h"
#include "GameObject.h"

class Paddle : public GameObject
{
private:
	int PADDLE_SPEED;
	double m_velY;
public:
	Paddle(double x = 0, double y = 0, int w = UNIT_LENGTH, int h = UNIT_LENGTH * 5)
		: GameObject{ x, y, w, h }, m_velY{ 0.0 }, PADDLE_SPEED{ 500 }
	{}

	// updates any changes to the state of object
	void Update(double deltaTime);

	// handles movement through input (isPlayerOne decides which keys to take as input)
	void HandleMovement(bool isPlayerOne);
};

#endif // !PADDLE_H
