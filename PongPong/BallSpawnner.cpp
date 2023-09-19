#include "BallSpawnner.h"
#include "Window.h"
#include <iostream>
#include <random>
#include <SDL.h>

BallSpawnner::BallSpawnner()
{
	mPosX = (SCREEN_WIDTH - UNIT_SIZE) / 2;
}

BallSpawnner::~BallSpawnner() {};


// adds to vector array posY the y-coordinate of spawn positions
void BallSpawnner::calculateSpawnPosition()
{
	int totalSpawnPositions{ SCREEN_HEIGHT / UNIT_SIZE };
	int currYpos = 0;

	for (int i = 1; i < totalSpawnPositions; ++i)
	{
		if (i % 2 == 0)
		{
			mYpositions.push_back(currYpos);
			std::cout << currYpos << '\t';
		}
		currYpos += UNIT_SIZE;
	}
}

// renders a sqare to indicate spawn position
void BallSpawnner::render()
{
	SDL_SetRenderDrawColor(gRenderer, gObjColor.r, gObjColor.g, gObjColor.b, gObjColor.a);
	for (int i = 0; i < mYpositions.size(); ++i)
	{
		SDL_Rect newRect = { mPosX, mYpositions[i], UNIT_SIZE, UNIT_SIZE };
		SDL_RenderFillRect(gRenderer, &newRect);
	}
}

SDL_Point BallSpawnner::getSpawnPoint(std::mt19937 generator)
{
	// random number from range 0 to (size of mYpositions - 1) (-1 because uniform_int_distribution is takes range as [a, b] and not [a, b)
	std::uniform_int_distribution<int> distribution(0, mYpositions.size() - 1);

	SDL_Point spawnPoint;
	spawnPoint.x = mPosX;
	spawnPoint.y = mYpositions[distribution(generator)];

	return spawnPoint;
}