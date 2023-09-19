#include <SDL.h>
#include "Window.h"
#ifndef BORDER
#define BORDER

class Border
{
public:
	Border(int posY = 0); // defines members default y position is 0 or takes in y
	~Border();

	// renders the border to the screen
	void render();

	// returns the rect of the border
	inline SDL_Rect getRect()
	{
		return mBorderRect;
	}
private:
	SDL_Rect mBorderRect;
};

#endif // !BORDER
