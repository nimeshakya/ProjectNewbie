#include <SDL.h>
#include "Window.h"
#include <iostream>

#ifndef PADDLE
#define PADDLE

class Paddle
{
public:
	Paddle(int posX = UNIT_SIZE);
	~Paddle();

	// moves the bat according to input
	void movePaddle(SDL_Event& e, PlayerType playerType);

	// update state of paddle according to changes
	void update(float deltaTime);
	
	// renders the bat on screen
	void render();

	// return rect of paddle
	inline SDL_Rect getRect()
	{
		return mPaddleRect;
	}
private:
	SDL_Rect mPaddleRect;
	float mVelY{};
	int mDirY{};
};

#endif // !PADDLE
