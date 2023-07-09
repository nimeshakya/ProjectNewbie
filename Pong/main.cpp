#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cmath>

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
	SDL_Rect* getRect();
private:
	SDL_Rect* wallRect;
};

class BallSpawnner
{
public:
	// that middle line from where the ball will be spawnned
	// finds each position from where the ball can be spawnned
	void findSpawnPos();
	// draws the spawn line with boxes
	void drawSpawnLine();
	// returns a spawn position
	SDL_Point getSpawnPosition();
private:
	// points where the ball can be spawnned
	std::vector<SDL_Point> mSpawnPositions;
};

class Bat
{
public:
	// maximum axis velocity of the bat
	static const int BAT_VEL = 5;

	// by default position x is BALL_LENGTH * 2
	Bat(int pMode = 1, int x = BALL_LENGTH * 2, int y = (SCREEN_HEIGHT - BALL_LENGTH * 5) / 2);

	// take key presses and adjust the bat's velocity
	void handleEvent(SDL_Event& e);
	// moves the bat
	void move();

	// renders the bat to the screen
	void renderBat();

	SDL_Rect* getRect();
private:
	SDL_Rect* batRect;
	// velocity of bat
	int mVelY;
	int playerMode;
};

class Ball
{
public:
	Ball();
	Ball(BallSpawnner ballSpawnner, int y = (SCREEN_HEIGHT - BALL_LENGTH) / 2);

	// moves the ball around the screen dealing with collistions
	void moveBall(Wall wallTop, Wall wallBot, Bat bat1, Bat bat2, double deltaTime);

	// render the ball to the screen
	void renderBall();
private:
	SDL_Rect* ballRect;

	// velocity of ball;
	float BALL_VEL;
	// velocity of ball
	float mVelX, mVelY;
	// direction of ball
	int xDir, yDir; // -1 or +1
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
	wallRect = new SDL_Rect();
	wallRect->x = x;
	wallRect->y = y;
	wallRect->w = SCREEN_WIDTH;
	wallRect->h = BALL_LENGTH;
}

void Wall::drawWall()
{
	SDL_SetRenderDrawColor(gRenderer, RED, GREEN, BLUE, 0xFF);
	SDL_RenderFillRect(gRenderer, wallRect);
}

SDL_Rect* Wall::getRect()
{
	return wallRect;
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
			mSpawnPositions.push_back(newPoint);
		}
	}
}

void BallSpawnner::drawSpawnLine()
{
	SDL_Rect currSpawnPosition;
	SDL_SetRenderDrawColor(gRenderer, RED, GREEN, BLUE, 0xFF);
	for (int i = 0; i < mSpawnPositions.size(); ++i)
	{
		currSpawnPosition = { mSpawnPositions[i].x, mSpawnPositions[i].y, BALL_LENGTH, BALL_LENGTH };
		SDL_RenderFillRect(gRenderer, &currSpawnPosition);
	}
}

SDL_Point BallSpawnner::getSpawnPosition()
{
	SDL_Point spawnPosition;
	spawnPosition = mSpawnPositions[mSpawnPositions.size() / 2];
	return spawnPosition;
}

// Bat functions

Bat::Bat(int pMode, int x, int y)
{
	// initialize bat's offsets
	batRect = new SDL_Rect();
	batRect->x = x;
	batRect->y = y;
	batRect->w = BALL_LENGTH;
	batRect->h = BALL_LENGTH * 5;

	// initialize the velocity
	mVelY = 0;

	// player mode
	playerMode = pMode;
}

void Bat::handleEvent(SDL_Event& e)
{
	//if key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		if (playerMode == 1)
		{
			// adjust the velocity
			switch (e.key.keysym.sym)
			{
			case SDLK_w: mVelY -= BAT_VEL; break;
			case SDLK_s: mVelY += BAT_VEL; break;
			default:
				break;
			}
		}
		else if (playerMode == 2)
		{
			// adjust the velocity
			switch (e.key.keysym.sym)
			{
			case SDLK_UP: mVelY -= BAT_VEL; break;
			case SDLK_DOWN: mVelY += BAT_VEL; break;
			default:
				break;
			}
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		if (playerMode == 1)
		{
			// Adjust the velocity
			switch (e.key.keysym.sym)
			{
			case SDLK_w: mVelY += BAT_VEL; break;
			case SDLK_s: mVelY -= BAT_VEL; break;
			default:
				break;
			}
		}
		else if (playerMode == 2)
		{
			// Adjust the velocity
			switch (e.key.keysym.sym)
			{
			case SDLK_UP: mVelY += BAT_VEL; break;
			case SDLK_DOWN: mVelY -= BAT_VEL; break;
			default:
				break;
			}
		}
	}
}

void Bat::move()
{
	// move the bat up or down
	batRect->y += mVelY;

	// if dot went too far up or down
	if (batRect->y < BALL_LENGTH) // using ball wall height instead of ball length would be better but for now it's fine
	{
		// move back
		batRect->y = BALL_LENGTH;
	}
	else if ((batRect->y + batRect->h) > (SCREEN_HEIGHT - BALL_LENGTH))
	{
		batRect->y = SCREEN_HEIGHT - BALL_LENGTH - batRect->h;
	}
}

void Bat::renderBat()
{
	SDL_SetRenderDrawColor(gRenderer, RED, GREEN, BLUE, 0xFF);
	SDL_RenderFillRect(gRenderer, batRect);
}

SDL_Rect* Bat::getRect()
{
	return batRect;
}

// ball functions

Ball::Ball()
{
	ballRect = new SDL_Rect();
	ballRect->x = 0;
	ballRect->y = 0;
	ballRect->w = BALL_LENGTH;
	ballRect->h = BALL_LENGTH;

	BALL_VEL = 0;

	// ball velocity in x and y direction
	mVelX = 0;
	mVelY = 0;

	// ball direction
	xDir = 1;
	yDir = 1;
}

Ball::Ball(BallSpawnner ballSpawnner, int y)
{
	SDL_Point ballSpawnPoint = ballSpawnner.getSpawnPosition();

	ballRect = new SDL_Rect();
	ballRect->x = ballSpawnPoint.x;
	ballRect->y = ballSpawnPoint.y;
	ballRect->w = BALL_LENGTH;
	ballRect->h = BALL_LENGTH;

	BALL_VEL = 0.6f;

	// ball velocity in x and y direction
	mVelX = BALL_VEL;
	mVelY = BALL_VEL;

	// ball direction
	xDir = 1;
	yDir = 1;
}

void Ball::renderBall()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, ballRect);
}

void Ball::moveBall(Wall wallTop, Wall wallBot, Bat bat1, Bat bat2, double deltaTime)
{
	SDL_Rect* bat1Rect = bat1.getRect();
	SDL_Rect* bat2Rect = bat2.getRect();
	SDL_Rect* wallTopRect = wallTop.getRect();
	SDL_Rect* wallBotRect = wallBot.getRect();

	if ((ballRect->x + ballRect->w > bat2Rect->x) && (ballRect->y > bat2Rect->y && ballRect->y < bat2Rect->y + bat2Rect->h))
	{
		xDir = -1;
	}
	else if (ballRect->x < bat1Rect->x + bat1Rect->w)
	{
		xDir = 1;
	}

	if (ballRect->y < wallTopRect->y + wallTopRect->h)
	{
		std::cout << "this happened";
		yDir = 1;
	}
	else if (ballRect->y + ballRect->h > wallBotRect->y)
	{
		yDir = -1;
	}

	ballRect->x += xDir * (int)(mVelX * deltaTime);
	//ballRect->y += yDir * (int)(mVelY * deltaTime);
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

			Bat bat1(1); // with pmode 1
			Bat bat2(2, SCREEN_WIDTH - BALL_LENGTH * 3); // with pmode 2

			Ball ball(spawnLine);

			// calculation delat time
			Uint64 THIS_FRAME = SDL_GetPerformanceCounter();
			Uint64 LAST_FRAME = 0;
			double deltaTime = 0;

			while (!quit) // game loop
			{
				while (SDL_PollEvent(&e)) // event loop
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					// handle input for the bat
					bat1.handleEvent(e);
					bat2.handleEvent(e);
				}
				LAST_FRAME = THIS_FRAME;
				THIS_FRAME = SDL_GetPerformanceCounter();
				deltaTime = (double)((THIS_FRAME - LAST_FRAME) * 1000 / (double)SDL_GetPerformanceFrequency());

				// move the bat
				bat1.move();
				bat2.move();

				ball.moveBall(topWall, bottomWall, bat1, bat2, deltaTime);

				// clear renderer
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(gRenderer);

				topWall.drawWall();
				bottomWall.drawWall();

				spawnLine.drawSpawnLine();

				bat1.renderBat();
				bat2.renderBat();

				ball.renderBall();

				// present renders
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();
	return 0;
}