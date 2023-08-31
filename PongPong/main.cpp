#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Window.h"
#include "Border.h"
#include "Paddle.h"
#include "BallSpawnner.h"

// rendering variables
SDL_Window* gWindow{ NULL };
SDL_Renderer* gRenderer{ NULL };

// initialize SDL and subsystems
bool init();

// load all the game media (image, sound, texture, etc.)
bool loadmedia();

// shut down SDL and subsystems
void close();

// initialize SDL and subsystems
bool init()
{
	bool success{ true }; // flag

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not be initialize! SDL Error: " << SDL_GetError() << '\n';
		return (success = false);
	}

	// create a window at whatever position on screen with dimension in global var
	gWindow = SDL_CreateWindow("PongPong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
		return success = false;
	}

	// create renderer with accelerated video and vsync
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
	{
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
		return success = false;
	}

	// default colour of rendered screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// init png loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_Image could not be initialized! SDL_image ERROR: " << IMG_GetError() << '\n';
		success = false;
	}

	// true type font initialization
	if (TTF_Init() == -1)
	{
		std::cout << "SDL_ttf could not be initialized! SDL_ttf ERROR: " << TTF_GetError() << '\n';
		success = false;
	}

	return success;
}

// load all the game media (image, sound, texture, etc.)
bool loadmedia()
{
	bool success{ true }; // flag

	// no media yet

	return success;
}

// shut down SDL and subsystems
void close()
{
	// free window and renderer
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;

	// close SDL and subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (!init())
	{
		std::cout << "Failed to initialize SDL!\n";
		return -1;
	}
	if (!loadmedia())
	{
		std::cout << "Failed to load meda!\n";
		return -1;
	}

	bool quit{ false }; // quit flag
	SDL_Event e; // event handler

	/* Game Objects */
	Border topBorder;
	Border botBorder(SCREEN_HEIGHT - UNIT_SIZE);

	Paddle paddleRight;
	Paddle paddleLeft(SCREEN_WIDTH - UNIT_SIZE * 2);

	BallSpawnner ballSpawnner;
	ballSpawnner.calculateSpawnPosition();

	while (!quit) // game loop
	{
		while (SDL_PollEvent(&e) != 0) // event loop
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		// clear renderer
		SDL_SetRenderDrawColor(gRenderer, gBoardColor.r, gBoardColor.g, gBoardColor.b, gBoardColor.a);
		SDL_RenderClear(gRenderer);

		// render
		topBorder.render();
		botBorder.render();

		paddleLeft.render();
		paddleRight.render();

		ballSpawnner.render();

		// present render
		SDL_RenderPresent(gRenderer);
	}

	close();
	return 0;
}