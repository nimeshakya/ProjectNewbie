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
		double posX = (SCREEN_WIDTH - UNIT_LENGTH) / 2; // at middle of screen
		double posY = 0;
		int spawnPosCount = (SCREEN_HEIGHT / UNIT_LENGTH) - 2;

		for (int i = 0; i < spawnPosCount; ++i)
		{
			posY += (double)UNIT_LENGTH;
			if (i % 2 == 0)
			{
				m_positions.push_back(Vec2(posX, posY));
			}
		}
	}

	// renders a spawnner at spawn position
	void Render();

	// returns a spawn position (randomly)
	Vec2 GetSpawnPosition();
};

#endif // !BALL_SPAWNNER_H
