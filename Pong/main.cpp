#include <iostream>
#include <SDL.h>

#include "Window.h"

// initialize sdl and subsystems
bool init();
// load all media
bool loadMedia();
// shut down sdl and subsystems
void close();

SDL_Window* g_window;
SDL_Renderer* g_renderer;

bool init()
{
	bool success{ true }; // flag

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not be initialized! SDL ERROR: " << SDL_GetError() << '\n';
		return (success = false);
	}

	g_window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_constant::WINDOW_WIDTH, screen_constant::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (g_window == NULL)
	{
		std::cout << "Window could not be created! SDL ERROR: " << SDL_GetError() << '\n';
		return (success = false);
	}

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_renderer == NULL)
	{
		std::cout << "Renderer could not be created! SDL ERROR: " << SDL_GetError() << '\n';
		return (success = false);
	}

	return success;
}

bool loadMedia()
{
	bool success{ true };

	// laod media here

	return success;
}

void close()
{
	SDL_DestroyWindow(g_window);
	SDL_DestroyRenderer(g_renderer);
	g_window = NULL;
	g_renderer = NULL;

	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (!init())
	{
		std::cout << "Failed to initialize SDL!\n";
	}
	else
	{
		if (!loadMedia())
		{
			std::cout << "Failed to load media!\n";
		}
		else
		{
			bool quit{ false };
			// SDL events
			SDL_Event event;
			while (!quit) // main loop
			{
				while (SDL_PollEvent(&event) != 0) // event loop
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				// clear render
				SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(g_renderer);

				// update

				// render
				SDL_RenderPresent(g_renderer);
			}
		}
	}

	close();
	return 0;
}