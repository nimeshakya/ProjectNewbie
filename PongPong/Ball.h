#pragma once
#include <SDL.h>
#include "Window.h"
#include "BallSpawnner.h"

class Ball
{
public:
	Ball();
	Ball(SDL_Point spawnPoint);
	~Ball();

	// render ball on screen
	void render();

	inline SDL_Rect getRect()
	{
		return mBallRect;
	}
private: 
	SDL_Rect mBallRect;
};