#ifndef PADDLE_H
#define PADDLE_H

#include "Window.h"
#include "GameObject.h"

class Paddle : public GameObject
{
public:
	Paddle(int x = 0, int y = 0, int w = UNIT_LENGTH, int h = UNIT_LENGTH * 5)
		: GameObject{ x, y, w, h }
	{}
};

#endif // !PADDLE_H
