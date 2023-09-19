#include "Ball.h"
#include <iostream>
#include <SDL.h>

Ball::Ball()
{
	std::cout << "this ran\n";
	mBallRect = { (SCREEN_WIDTH - UNIT_SIZE) / 2, (SCREEN_HEIGHT - UNIT_SIZE) / 2, UNIT_SIZE, UNIT_SIZE };
}

Ball::Ball(SDL_Point spawnPoint)
{
	mBallRect = { spawnPoint.x, spawnPoint.y, UNIT_SIZE, UNIT_SIZE };
}

Ball::~Ball() {}

void Ball::render()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &mBallRect);
	//std::cout << mBallRect.x << '\t' << mBallRect.y << '\n';
}