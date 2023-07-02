#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BALL_LENGTH = 15; // square ball

// color used
const Uint8 RED = 0xBB;
const Uint8 GREEN = 0xBB;
const Uint8 BLUE = 0xBB;

class Wall {
public:
	// wall constructor sets default position to (0, 0) as well as width and height to 0
	Wall(int x = 0, int y = 0);
	// draws a wall on position with specified width and height
	void drawWall();
private:
	int posX;
	int posY;
	int w;
	int h;
};

class BallSpawnner
{
public:
	// that middle line from where the ball will be spawnned
	// finds each position from where the ball can be spawnned
	void findSpawnPos();
	// draws the spawn line with boxes
	void drawSpawnLine();
private:
	// points where the ball can be spawnned
	std::vector<SDL_Point> spawnPositions;
};

class Bat
{
public:
	// by default position x is BALL_LENGTH * 2
	Bat(int x = BALL_LENGTH * 2, int y = (SCREEN_HEIGHT - BALL_LENGTH * 5) / 2);
	void renderBat();
private:
	SDL_Rect* batRect;
};

// initialize SDL and subsystems; returns boolean success
bool init();
// load all involved media; returns boolean success
bool loadMedia();
// close all SDL subsystems and quit SDL
void close();

/* GLOBAL VARIABLES */
SDL_Window* gWindow = NULL; // window
SDL_Surface* gScreenSurface = NULL; // default screen surface
SDL_Renderer* gRenderer = NULL; // default renderer

// Wall Functions

Wall::Wall(int x, int y)
{
	posX = x;
	posY = y;
	w = SCREEN_WIDTH;
	h = BALL_LENGTH;
}

void Wall::drawWall()
{
	SDL_SetRenderDrawColor(gRenderer, RED, GREEN, BLUE, 0xFF);
	SDL_Rect wallRect = { posX, posY, w, h };
	SDL_RenderFillRect(gRenderer, &wallRect);
}

// ball spawnner functions

void BallSpawnner::findSpawnPos()
{
	const int spawnGapCount{ (SCREEN_HEIGHT - BALL_LENGTH * 2) / BALL_LENGTH }; // total spawnner and gap count according to play area height
	const int posX{ (SCREEN_WIDTH - BALL_LENGTH) / 2 };
	int posY;
	for (int i = 0; i < spawnGapCount; ++i)
	{
		// if odd number
		if (i % 2 != 0)
		{
			posY = BALL_LENGTH * i;
			SDL_Point newPoint = { posX, posY };
			spawnPositions.push_back(newPoint);
		}
	}
}

void BallSpawnner::drawSpawnLine()
{
	SDL_Rect currSpawnPosition;
	SDL_SetRenderDrawColor(gRenderer, RED, GREEN, BLUE, 0xFF);
	for (int i = 0; i < spawnPositions.size(); ++i)
	{
		currSpawnPosition = { spawnPositions[i].x, spawnPositions[i].y, BALL_LENGTH, BALL_LENGTH };
		SDL_RenderFillRect(gRenderer, &currSpawnPosition);
	}
}

// Bat functions

Bat::Bat(int x, int y)
{
	batRect = new SDL_Rect();
	batRect->x = x;
	batRect->y = y;
	batRect->w = BALL_LENGTH;
	batRect->h = BALL_LENGTH * 5;
}

void Bat::renderBat()
{
	SDL_SetRenderDrawColor(gRenderer, RED, GREEN, BLUE, 0xFF);
	SDL_RenderFillRect(gRenderer, batRect);
}

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
		// set linear texture filtering
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			std::cout << "Warning: Linear texture filtering not enabled!\n";
		}

		// create window
		gWindow = SDL_CreateWindow("pong pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			std::cout << "Window could not be created! SDL ERROR: " << SDL_GetError() << '\n';
			success = false;
		}
		else
		{
			// create renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL ERROR: " << SDL_GetError() << '\n';
				success = false;
			}
			else
			{
				// set default color of renderer
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// enable png loading
				int imgFlag = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlag) & imgFlag))
				{
					std::cout << "SDL_image could not be initialized! SDL_image ERROR: " << IMG_GetError() << '\n';
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia()
{
	bool success{ true };

	return success;
}

void close()
{
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;

	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;

	// shut down SDL_image
	IMG_Quit();
	// shut down SDL
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (!init())
	{
		std::cout << "Failed to initialze SDL!\n";
	}
	else
	{
		if (!loadMedia())
		{
			std::cout << "Failed to load media\n";
		}
		else
		{
			bool quit{ false };
			SDL_Event e;

			/* Game Objects */
			Wall topWall(0, 0);
			Wall bottomWall(0, SCREEN_HEIGHT - BALL_LENGTH);

			BallSpawnner spawnLine;
			spawnLine.findSpawnPos();

			Bat bat1;
			Bat bat2(SCREEN_WIDTH - BALL_LENGTH * 3);

			while (!quit) // game loop
			{
				while (SDL_PollEvent(&e)) // event loop
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				// clear renderer
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(gRenderer);

				topWall.drawWall();
				bottomWall.drawWall();

				spawnLine.drawSpawnLine();

				bat1.renderBat();
				bat2.renderBat();

				// present renders
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();
	return 0;
}