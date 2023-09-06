#include "Paddle.h"

Paddle::Paddle(int posX)
{
	mPaddleRect = { posX, (SCREEN_HEIGHT - UNIT_SIZE * 5) / 2, UNIT_SIZE, UNIT_SIZE * 5 };
	mVelY = 0.0f;
	moveY = 0;
}

Paddle::~Paddle() {};

void Paddle::movePaddle(SDL_Event& e, PlayerType playerType, float deltaTime)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	switch (playerType)
	{
	case PlayerType::PLAYER_ONE:
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			moveY = 1;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			moveY = -1;
		}
		else
		{
			moveY = 0;
		}
		break;
	case PlayerType::PLAYER_TWO:
		if (currentKeyStates[SDL_SCANCODE_W])
		{
			moveY = 1;
		}
		else if (currentKeyStates[SDL_SCANCODE_S])
		{
			moveY = -1;
		}
		else
		{
			moveY = 0;
		}
		break;
	default:
		break;
	}

	/*if (playerType == PlayerType::PLAYER_ONE)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			std::cout << "this happening!\n";
			moveY = 1;
			break;
		case SDLK_s:
			moveY = -1;
			break;
		default:
			break;
		}
	}
	else if (playerType == PlayerType::PLAYER_TWO)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			std::cout << "this running!\n";
			moveY = 1;
			break;
		case SDLK_DOWN:
			moveY = -1;
			break;
		default:
			break;
		}
	}*/

	// update velocity
}

// clears the mVel
void Paddle::clearVelocity(SDL_Event& e)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_UP || SDLK_DOWN || SDLK_w || SDLK_s:
		moveY = 0;
		break;
	default:
		break;
	}
}

// update state of paddle according to changes
void Paddle::update()
{
	std::cout << moveY << '\t' << mVelY << '\n';
}

void Paddle::render()
{
	SDL_SetRenderDrawColor(gRenderer, gObjColor.r, gObjColor.g, gObjColor.b, gObjColor.a);
	SDL_RenderFillRect(gRenderer, &mPaddleRect);
}

