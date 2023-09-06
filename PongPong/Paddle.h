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
	void movePaddle(SDL_Event& e, PlayerType playerType, float deltaTime);
	// clears the mVel;
	void clearVelocity(SDL_Event& e);

	// update state of paddle according to changes
	void update();
	
	// renders the bat on screen
	void render();
private:
	SDL_Rect mPaddleRect;
	float mVelY{};
	int moveY{};
};

#endif // !PADDLE
