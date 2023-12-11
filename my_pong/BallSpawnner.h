#ifndef BALL_SPAWNNER_H
#define BALL_SPAWNNER_H

#include <vector>
#include <SDL.h>

#include "Vec2.h"
#include "Window.h"

class BallSpawnner
{
private:
	std::vector<Vec2> m_positions; // array of positions
public:
	BallSpawnner()
	{
		int posX = (SCREEN_WIDTH - UNIT_LENGTH) / 2; // at middle of screen
		int posY = 0;
		int spawnPosCount = (SCREEN_HEIGHT / UNIT_LENGTH) - 2;

		for (int i = 0; i < spawnPosCount; ++i)
		{
			posY += UNIT_LENGTH;
			if (i % 2 == 0)
			{
				m_positions.push_back(Vec2(posX, posY));
			}
		}
	}

	// renders a spawnner at spawn position
	void Render();
};

#endif // !BALL_SPAWNNER_H
