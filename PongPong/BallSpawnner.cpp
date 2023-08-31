#include "BallSpawnner.h"
#include "Window.h"
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