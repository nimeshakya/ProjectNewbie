#ifndef BALL_H
#define BALL_H

#include "GameObject.h"
#include "Vec2.h"

class Ball : public GameObject
{
public:
	Ball(Vec2 position, int w = UNIT_LENGTH, int h = UNIT_LENGTH)
		: GameObject(position.x, position.y, w, h)
	{}
	inline void Render()
	{
		SDL_SetRenderDrawColor(g_renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_Rect objRect{ m_position.x, m_position.y, m_width, m_height };
		SDL_RenderFillRect(g_renderer, &objRect);
	}
};

#endif // !BALL_H
