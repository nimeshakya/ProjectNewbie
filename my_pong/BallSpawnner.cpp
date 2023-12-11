#include "BallSpawnner.h"

void BallSpawnner::Render()
{
	SDL_SetRenderDrawColor(g_renderer, GameObjColor::OB_RED, GameObjColor::OB_GREEN, GameObjColor::OB_BLUE, 0xFF);

	for (auto position : m_positions)
	{
		SDL_Rect spawnRect{ position.x, position.y, UNIT_LENGTH, UNIT_LENGTH };
		SDL_RenderFillRect(g_renderer, &spawnRect);
	}
}