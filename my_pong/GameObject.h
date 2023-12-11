#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL.h>

#include "Vec2.h"
#include "Window.h"

class GameObject
{
protected:
	Vec2 m_position;
	int m_width, m_height;
public:
	GameObject(double x = 0, double y = 0, int w = 0, int h = 0)
		: m_position{ Vec2{x, y} }, m_width{ w }, m_height{ h }
	{}

	inline void Render()
	{
		SDL_SetRenderDrawColor(g_renderer, GameObjColor::OB_RED, GameObjColor::OB_GREEN, GameObjColor::OB_BLUE, 0xFF);
		SDL_Rect objRect{ m_position.x, m_position.y, m_width, m_height };
		SDL_RenderFillRect(g_renderer, &objRect);
	}

	const Vec2& getPosition() const { return m_position; }
	const int& getWidth() const { return m_width; }
	const int& getHeight() const { return m_height; }
};

#endif // !GAMEOBJECT_H
