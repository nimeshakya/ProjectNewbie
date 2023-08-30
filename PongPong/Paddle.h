#include <SDL.h>
#include "Window.h"

#ifndef PADDLE
#define PADDLE

class Paddle
{
public:
	Paddle(int posX = UNIT_SIZE);
	~Paddle();

	void render();
private:
	SDL_Rect mPaddleRect;
};

#endif // !PADDLE
