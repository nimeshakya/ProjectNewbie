#include "Border.h"

Border::Border(int posY)
{
	mBorderRect = { 0, posY, SCREEN_WIDTH, UNIT_SIZE};
}

Border::~Border() {};

void Border::render()
{
	SDL_SetRenderDrawColor(gRenderer, gObjColor.r, gObjColor.g, gObjColor.b, gObjColor.a);
	SDL_RenderFillRect(gRenderer, &mBorderRect);
}