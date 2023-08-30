#include "Paddle.h"

Paddle::Paddle(int posX)
{
	mPaddleRect = { posX, (SCREEN_HEIGHT - UNIT_SIZE * 5) / 2, UNIT_SIZE, UNIT_SIZE * 5 };
}

Paddle::~Paddle() {};

void Paddle::render()
{
	SDL_SetRenderDrawColor(gRenderer, gObjColor.r, gObjColor.g, gObjColor.b, gObjColor.a);
	SDL_RenderFillRect(gRenderer, &mPaddleRect);
}