#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>

const int SCREEN_WIDTH{ 800 };
const int SCREEN_HEIGHT{ 800 };

class Surfer
{
public:
	const float SURFER_VELOCITY{ 200 };
	Surfer();
	~Surfer();

	void update(float deltaTime);
	void collideWithBorder();
	void render();

private:
	SDL_Rect mSurferRect;
	float mVelX, mVelY;
	std::vector<SDL_Point> trailPoints;
};

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// surfer methods
Surfer::Surfer()
{
	mSurferRect = { 0, 0, 120, 80 };
	mVelX = SURFER_VELOCITY;
	mVelY = SURFER_VELOCITY;
}

Surfer::~Surfer(){}

void Surfer::update(float deltaTime)
{
	SDL_Point newTrailPoint{ mSurferRect.x, mSurferRect.y };
	trailPoints.push_back(newTrailPoint);

	mSurferRect.x += mVelX * deltaTime;
	mSurferRect.y += mVelY * deltaTime;

	collideWithBorder();
}

void Surfer::collideWithBorder()
{
	int surferLeft{ mSurferRect.x };
	int surferRight{ mSurferRect.x + mSurferRect.w };
	int surferTop{ mSurferRect.y };
	int surferBottom{ mSurferRect.y + mSurferRect.h };

	int borderLeft{ 0 };
	int borderRight{ SCREEN_WIDTH };
	int borderTop{ 0 };
	int borderBottom{ SCREEN_HEIGHT };

	if (surferLeft < borderLeft || surferRight > borderRight)
	{
		mVelX = -mVelX;
	}
	if (surferTop < borderTop || surferBottom > borderBottom)
	{
		mVelY = -mVelY;
	}
}

void Surfer::render()
{
	SDL_SetRenderDrawColor(gRenderer, 0xAA, 0x00, 0xFF, 0xFF);
	for (int i = 0; i < trailPoints.size(); ++i)
	{
		SDL_Point currPoint = trailPoints[i];
		SDL_RenderDrawPoint(gRenderer, currPoint.x, currPoint.y);
	}
	SDL_RenderFillRect(gRenderer, &mSurferRect);
}

bool init()
{
	bool success{ true };
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not be initialized! SDL error: " << SDL_GetError() << '\n';
		return (success = false);
	}
	gWindow = SDL_CreateWindow("Screen Surfer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		std::cout << "Window could not be initialized! SDL error: " << SDL_GetError() << '\n';
		return (success = false);
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		std::cout << "Renderer could not be initialized! SDL error: " << SDL_GetError() << '\n';
		return (success = false);
				
	}
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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

	Mix_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (!init())
	{
		std::cout << "Failed to initialize SDL!\n";
	}
	else if (!loadMedia)
	{
		std::cout << "Failed to load media!\n";
	}
	else
	{
		bool quit{ false };
		SDL_Event e;

		// Surfer
		Surfer surfer;

		// deltatime
		Uint32 mTicksCount{};
		float deltaTime{};
 
		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
				;

			deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

			if (deltaTime > 0.05f)
			{
				deltaTime = 0.05f;
			}
			mTicksCount = SDL_GetTicks();

			surfer.update(deltaTime);

			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			surfer.render();

			SDL_RenderPresent(gRenderer);
		}
	}

	return 0;
}