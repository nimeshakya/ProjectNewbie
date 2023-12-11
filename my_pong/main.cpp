#include <iostream>
#include <SDL.h>

#include "Window.h"
#include "Paddle.h"
#include "Border.h"
#include "BallSpawnner.h"

// initializes SDL and subsystems
bool init();
// loads medias like images, sound and textures
bool loadMedia();
// shuts down SDL and subsystems
void close();

SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;


// initializes SDL and subsystems
bool init()
{
	bool success{ true };

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not be initialized! SDL ERROR: " << SDL_GetError() << '\n';
		success = false;
	}
	else
	{
		g_window = SDL_CreateWindow("my pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (g_window == NULL)
		{
			std::cout << "Window could not be created! SDL ERROR: " << SDL_GetError() << '\n';
			success = false;
		}
		else
		{
			g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
			if (g_renderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL ERROR: " << SDL_GetError() << '\n';
				success = false;
			}
		}
	}

	return success;
}

// loads medias like images, sound and textures
bool loadMedia()
{
	bool success{ true };

	// load media here

	return success;
}

// shuts down SDL and subsystems
void close()
{
	SDL_DestroyWindow(g_window);
	SDL_DestroyRenderer(g_renderer);
	g_window = NULL;
	g_renderer = NULL;

	// shut down sdl
	SDL_Quit();
}


int main(int argc, char* argv[])
{
	std::cout << SCREEN_WIDTH << '\t' << SCREEN_HEIGHT << '\n';
	if (!init())
	{
		std::cout << "Failed to initialize SDL!\n";
	}
	else
	{
		if (!loadMedia)
		{
			std::cout << "Failed to load media!\n";
		}
		else
		{
			bool quit{ false };
			SDL_Event e;

			// game assets
			Border borderTop{ 0, 0 };
			Border borderBot{ 0, SCREEN_HEIGHT - UNIT_LENGTH };

			Paddle paddle1{ 2 * UNIT_LENGTH, (SCREEN_HEIGHT - 5 * UNIT_LENGTH) / 2 };
			Paddle paddle2{ SCREEN_WIDTH - 3 * UNIT_LENGTH, (SCREEN_HEIGHT - 5 * UNIT_LENGTH) / 2 };

			BallSpawnner ballSpawnner{};

			while (!quit) // main loop
			{
				while (SDL_PollEvent(&e) != 0) // event loop
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

				}
				// clear renderer
				SDL_SetRenderDrawColor(g_renderer, GameObjColor::BG_RED , GameObjColor::BG_GREEN, GameObjColor::BG_BLUE, 255);
				SDL_RenderClear(g_renderer);
			
				borderTop.Render();
				borderBot.Render();

				paddle1.Render();
				paddle2.Render();

				ballSpawnner.Render();

				// present render
				SDL_RenderPresent(g_renderer);
			}
		}
	}

	close();
	return 0;
}