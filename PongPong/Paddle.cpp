#include "Paddle.h"

Paddle::Paddle(int posX)
{
	mPaddleRect = { posX, (SCREEN_HEIGHT - UNIT_SIZE * 5) / 2, UNIT_SIZE, UNIT_SIZE * 5 };
	mVelY = 400.0f;
	mDirY = 0;
}

Paddle::~Paddle() {};

void Paddle::movePaddle(SDL_Event& e, PlayerType playerType)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	switch (playerType)
	{
	case PlayerType::PLAYER_ONE:
		mDirY = 0;
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			mDirY -= 1;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			mDirY += 1;
		}
		break;
	case PlayerType::PLAYER_TWO:
		mDirY = 0;
		if (currentKeyStates[SDL_SCANCODE_W])
		{
			mDirY -= 1;
		}
		else if (currentKeyStates[SDL_SCANCODE_S])
		{
			mDirY += 1;
		}
		break;
	default:
		break;
	}
}

// update state of paddle according to changes
void Paddle::update(float deltaTime)
{
	mPaddleRect.y += mDirY * mVelY * deltaTime;
}

void Paddle::render()
{
	SDL_SetRenderDrawColor(gRenderer, gObjColor.r, gObjColor.g, gObjColor.b, gObjColor.a);
	SDL_RenderFillRect(gRenderer, &mPaddleRect);
}

bool Paddle::isCollidedWithBorder(Border& border)
{
	SDL_Rect borderRect{ border.getRect() };

	int borderLeft{ borderRect.x };
	int broderRight{ borderRect.x + borderRect.w };
	int borderTop{ borderRect.y };
	int borderBottom{ borderRect.y + borderRect.h };

	int paddleLeft{ mPaddleRect.x };
	int paddleRight{ mPaddleRect.x + mPaddleRect.w };
	int paddleTop{ mPaddleRect.y };
	int paddleBottom{ mPaddleRect.y + mPaddleRect.h };

	return false;
}
