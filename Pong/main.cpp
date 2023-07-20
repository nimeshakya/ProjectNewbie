#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>

const int SCREEN_WIDTH{ 640 };
const int SCREEN_HEIGHT{ 480 };
const int UNIT_SQARE_LENGTH(15);

class Wall
{
public:
	// constructor
	Wall(int posY = 0);
	// render the wall
	void renderWall();
	// returns the wallRect
	SDL_Rect getRect();
private:
	SDL_Rect wallRect;
};

class Bat
{
public:
	// contructor with default parameter of pos x set
	Bat(int posX = UNIT_SQARE_LENGTH * 2);
	// render bat
	void renderBat();
	// returns the bat rect
	SDL_Rect getRect();
private:
	SDL_Rect batRect;
};

class BallSpawnner
{
public:
	// constructor
	BallSpawnner();
	// calculates and finds spawn positions
	void calcSpawnPosition();
	// render the ball spawnner
	void render();
	// returns spawn position
	int getSpawnPosition();
private:
	std::vector<int> posArr;
	int posX;
};

// initializes SDL and subsystems
bool init();
// loads all the media
bool loadmedia();
// close SDL and subsystems
void close();

SDL_Window* gWindow{ NULL };
SDL_Renderer* gRenderer{ NULL };

// wall functions

Wall::Wall(int posY)
{
	wallRect = { 0, posY, SCREEN_WIDTH, UNIT_SQARE_LENGTH };
}

void Wall::renderWall()
{
	SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 0xFF);
	SDL_RenderFillRect(gRenderer, &wallRect);
}

SDL_Rect Wall::getRect()
{
	return wallRect;
}

// Bat functions

Bat::Bat(int posX)
{
	int batHeight = UNIT_SQARE_LENGTH * 5;
	batRect = { posX, (SCREEN_HEIGHT - batHeight) / 2, UNIT_SQARE_LENGTH, batHeight };
}

void Bat::renderBat()
{
	SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 0xFF);
	SDL_RenderFillRect(gRenderer, &batRect);
}

SDL_Rect Bat::getRect()
{
	return batRect;
}

// BallSpawnner functions

BallSpawnner::BallSpawnner()
{
	posX = (SCREEN_WIDTH - UNIT_SQARE_LENGTH) / 2;
}

void BallSpawnner::calcSpawnPosition()
{
	int noOfPositions{ (SCREEN_HEIGHT - UNIT_SQARE_LENGTH * 2) / UNIT_SQARE_LENGTH };
	int y{ UNIT_SQARE_LENGTH };
	for (int i = 1; i <= noOfPositions; ++i)
	{
		if (i % 2 == 0)
		{
			continue;
		}
		else
		{
			std::cout << "pushed pos: " << y << '\n';
			posArr.push_back(y);
		}
		y += UNIT_SQARE_LENGTH * 2;
	}
}

void BallSpawnner::render()
{
	SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 0xFF);
	for (int i = 0; i < posArr.size(); ++i)
	{
		SDL_Rect spawnnerRect = { posX, posArr[i], UNIT_SQARE_LENGTH, UNIT_SQARE_LENGTH };
		SDL_RenderFillRect(gRenderer, &spawnnerRect);
	}
}

int BallSpawnner::getSpawnPosition()
{
	int totalPosition{ (int)posArr.size() };

	return posArr[totalPosition / 2];
}

bool init()
{
	bool success{ true };

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not be initialized! SDL Error: " << SDL_GetError() << '\n';
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("pong pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// initialize png loading
				int imgFlags{ IMG_INIT_PNG };
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "SDL_image could not be initialized! SDL_image Error: " << IMG_GetError() << '\n';
					success = false;
				}

				// initialize ttf
				if (TTF_Init() == -1)
				{
					std::cout << "SDL_ttf could not be initialized! SDL_ttf Error: " << TTF_GetError() << '\n';
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
			SDL_Event e;

			// Game Objects
			Wall wallTop;
			Wall wallBot(SCREEN_HEIGHT - UNIT_SQARE_LENGTH);

			BallSpawnner spawnner;
			spawnner.calcSpawnPosition();

			Bat bat1;
			Bat bat2 = (SCREEN_WIDTH - UNIT_SQARE_LENGTH * 2);

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
				SDL_SetRenderDrawColor(gRenderer, 0x33, 0x33, 0x33, 0xFF);
				SDL_RenderClear(gRenderer);

				// render
				wallTop.renderWall();
				wallBot.renderWall();

				bat1.renderBat();
				bat2.renderBat();

				spawnner.render();

				// present render
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	return 0;
}