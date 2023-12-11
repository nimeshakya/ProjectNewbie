#ifndef BORDER_H
#define BORDER_H

#include "Window.h"
#include "GameObject.h"

class Border : public GameObject
{
public:
	Border(int x = 0, int y = 0, int w = SCREEN_WIDTH, int h = UNIT_LENGTH)
		: GameObject{ x, y, w, h }
	{}
};

#endif // !BORDER_H
