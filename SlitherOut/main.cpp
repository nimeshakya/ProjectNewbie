#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

const int OBJECT_DIMENSION = 20;
const int SCREEN_WIDTH = OBJECT_DIMENSION * 20 + 20; // + 20 to fit the parts inside
const int SCREEN_HEIGHT = OBJECT_DIMENSION * 20 + 20;
const int GAME_AREA_WIDTH = SCREEN_WIDTH - (OBJECT_DIMENSION * 4);
const int GAME_AREA_HEIGHT = SCREEN_HEIGHT - (OBJECT_DIMENSION * 4);
const int GAREA_LOWER_BOUND_X = (SCREEN_WIDTH - GAME_AREA_WIDTH) / 2;
const int GAREA_UPPER_BOUND_X = (SCREEN_WIDTH - GAME_AREA_WIDTH) / 2 + GAME_AREA_WIDTH;
const int GAREA_LOWER_BOUND_Y = (SCREEN_HEIGHT - GAME_AREA_HEIGHT) / 2;
const int GAREA_UPPER_BOUND_Y = (SCREEN_HEIGHT - GAME_AREA_HEIGHT) / 2 + GAME_AREA_HEIGHT;

class BodyPart
{
public:
	BodyPart();
	void renderPart();
	// returns false if collided
	bool updatePos(int x = 0, int y = 0);
	void moveToPos(int x, int y);
	// returns true if the position (x, y) is already of body
	bool isPositionOfPart(int x, int y);
	
	// get new position of where it will be, takes in direction
	int getNewPosX(int x);
	int getNewPosY(int y);
	// get position
	int getPosX();
	int getPosY();
private:
	int posX;
	int posY;
	int w;
	int h;
};

class GameTexture
{
public:
	GameTexture(); // initialize members
	~GameTexture(); // deallocate memory
	// free memory
	void free();
	bool loadTextTextureFromFile(std::string textTexture, SDL_Color textColor);
	// renders the texture on screen
	void render(int x, int y);

	// get dimensions
	int getWidth();
	int getHeight();
private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

class Food
{
public:
	Food();
	void renderFood();
	void renderFoodOnPos(int x, int y);
private:
	int w;
	int h;
	int posX;
	int posY;
};

bool init(); // initialize SDL
bool loadMedia(); // load all media
void close(); // shut down SDL and free memory

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

// font
TTF_Font* gFont = NULL;
GameTexture gGameOverTextPrompt;
GameTexture gRestartGuidePrompt;
GameTexture gScoreTexture;

// parts of snake
std::vector<BodyPart> partsArr;

BodyPart::BodyPart()
{
	w = OBJECT_DIMENSION;
	h = OBJECT_DIMENSION;
	posX = (SCREEN_WIDTH - OBJECT_DIMENSION) / 2;
	posY = (SCREEN_HEIGHT - OBJECT_DIMENSION) / 2;
}

void BodyPart::renderPart()
{
	SDL_SetRenderDrawColor(gRenderer, 0xAA, 0x00, 0xFF, 0xFF);
	SDL_Rect bodyRect = { posX, posY, w, h };
	SDL_RenderDrawRect(gRenderer, &bodyRect);
}

bool BodyPart::updatePos(int x, int y)
{
	bool updated{ true };
	int newPosX, newPosY;
	if (x == 1)
	{
		newPosX = posX + w;
		if (newPosX >= SCREEN_WIDTH) updated = false;
		else posX = newPosX;
	}
	else if (y == 1)
	{
		newPosY = posY + h;
		if (newPosY >= SCREEN_HEIGHT) updated = false;
		else posY = newPosY;
	}
	else if (x == -1)
	{
		newPosX = posX - w;
		if (newPosX < 0) updated = false;
		else posX = newPosX;
	}
	else if (y == -1)
	{
		newPosY = posY - h;
		if (newPosY < 0) updated = false;
		else posY = newPosY;
	}
	return updated;
}

void BodyPart::moveToPos(int x, int y)
{
	posX = x;
	posY = y;
}

bool BodyPart::isPositionOfPart(int x, int y)
{
	bool isOfPart{ false };

	for (BodyPart part : partsArr)
	{
		if (x == part.getPosX() && y == part.getPosY())
		{
			isOfPart = true;
		}
	}

	return isOfPart;
}

int BodyPart::getNewPosX(int x)
{
	int newPosX = 0;
	if (x == 1)
	{
		newPosX = posX + w;
	}
	else if (x == -1)
	{
		newPosX = posX - w;
	}
	else
	{
		newPosX = posX;
	}
	return newPosX;
}

int BodyPart::getNewPosY(int y)
{
	int newPosY = 0;
	if (y == 1)
	{
		newPosY = posY + h;
	}
	else if (y == -1)
	{
		newPosY = posY - h;
	}
	else
	{
		newPosY = posY;
	}
	return newPosY;
}

int BodyPart::getPosX()
{
	return posX;
}

int BodyPart::getPosY()
{
	return posY;
}

// game texture functions
GameTexture::GameTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

GameTexture::~GameTexture()
{
	free();
}

void GameTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

bool GameTexture::loadTextTextureFromFile(std::string textTexture, SDL_Color textColor)
{
	free();
	// texture surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textTexture.c_str(), textColor);
	if (textSurface == NULL)
	{
		std::cout << "Unable to render text surface! SDL_ttf ERROR: " << TTF_GetError() << '\n';
	}
	else
	{
		// create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL ERROR: " << SDL_GetError() << '\n';
		}
		else
		{
			// get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		// get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	return (mTexture != NULL);
}

void GameTexture::render(int x, int y)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int GameTexture::getWidth()
{
	return mWidth;
}

int GameTexture::getHeight()
{
	return mHeight;
}

// Food functions
Food::Food()
{
	w = OBJECT_DIMENSION;
	h = OBJECT_DIMENSION;
	posX = 0;
	posY = 0;
}

void Food::renderFood()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xAA, 0xFF);
	SDL_Rect foodRect = { posX, posY, w, h };
	SDL_RenderFillRect(gRenderer, &foodRect);
}

void Food::renderFoodOnPos(int x, int y)
{
	posX = x;
	posY = y;
	renderFood();
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
		gWindow = SDL_CreateWindow("SlitherOut", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			std::cout << "Window could not be created! SDL ERROR: " << SDL_GetError() << '\n';
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL ERROR: " << SDL_GetError() << '\n';
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// initialize png loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "SDL_image could not be initialized! SDL_image ERROR: " << IMG_GetError() << '\n';
					success = false;
				}

				// initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					std::cout << "SDL_ttf could not be initialized! SDL_ttf ERROR: " << TTF_GetError() << '\n';
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

	// opens the font
	gFont = TTF_OpenFont("fonts/lazy.ttf", 24);
	if (gFont == NULL)
	{
		std::cout << "Failed to load lazy font! SDL_ttf ERROR: " << TTF_GetError() << '\n';
		success = false;
	}
	else
	{
		// load text
		SDL_Color txtColor = { 0, 0, 0, 255 };
		if (!gGameOverTextPrompt.loadTextTextureFromFile("Collided! Game OVer!", txtColor))
		{
			std::cout << "Failed to render text texture!\n";
			success = false;
		}

		if (!gRestartGuidePrompt.loadTextTextureFromFile("Press Enter to Restart!", txtColor))
		{
			std::cout << "Failed to render restart guide text texture!\n";
			success = false;
		}
	}

	return success;
}

void close()
{
	gGameOverTextPrompt.free();

	// free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	// destroy texture
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;

	// shut down SDL and subsystems
	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char* argv[])
{
	if (!init())
	{
		std::cout << "Failed to iniitalize SDL!\n";
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
			BodyPart currPart;

			Uint32 timeInterval = 200; // 500 miliseconds to update snake
			Uint32 currTime = 0;

			// sname movement
			int x = 1;
			int y = 0;
			bool goodToTakeInput{ true };

			// snake body 
			const int n = 10; // temp

			int initLength = 3;
			for (int i = 0; i < initLength; i++)
			{
				BodyPart part;
				partsArr.push_back(part);
 			}

			int storePosX = 0;
			int storePosY = 0;

			// food states
			Food newFood;
			int spawnPosX = (((std::rand() % (GAREA_UPPER_BOUND_X - GAREA_LOWER_BOUND_X + 1))) + GAREA_LOWER_BOUND_X) / OBJECT_DIMENSION * OBJECT_DIMENSION;
			int spawnPosY = (((std::rand() % (GAREA_UPPER_BOUND_Y - GAREA_LOWER_BOUND_Y + 1))) + GAREA_LOWER_BOUND_Y) / OBJECT_DIMENSION * OBJECT_DIMENSION;

			// game states
			bool collided{ false };
			bool foodConsumed{ false };
			int score = 0; // score of the current game
			std::stringstream scoreText;
			scoreText.str("");
			scoreText << "Score: " << score;
			SDL_Color textColor = { 0, 0, 0, 255 };

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN && goodToTakeInput)
					{
						goodToTakeInput = false;
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							x = 0;
							if (y != 1)
							{
								y = -1;
							}
							break;
						case SDLK_DOWN:
							x = 0;
							if (y != -1)
							{
								y = 1;
							}
							break;
						case SDLK_LEFT:
							if (x != 1)
							{
								x = -1;
							}
							y = 0;
							break;
						case SDLK_RIGHT:
							if (x != -1)
							{
								x = 1;
							}
							y = 0;
							break;
						case SDLK_RETURN:
							if (collided)
							{
								initLength = 3;
								partsArr.clear();
								for (int i = 0; i < initLength; i++)
								{
									BodyPart newPart;
									partsArr.push_back(newPart);
								}
								storePosX = 0;
								storePosY = 0;
								x = 1;
								y = 0;
								collided = false;
								score = 0;
								scoreText.str("");
								scoreText << "Score: " << score;
								std::cout << "reset!\n";
							}
						default:
							break;
						}
					}
				}
				// clear render
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				// render game area
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xCC, 0xAA, 0xFF); // accidental beautiful color! xD
				SDL_Rect gameAreaRect = { (SCREEN_WIDTH - GAME_AREA_WIDTH) / 2, (SCREEN_HEIGHT - GAME_AREA_HEIGHT) / 2, GAME_AREA_WIDTH, GAME_AREA_HEIGHT };
				SDL_RenderFillRect(gRenderer, &gameAreaRect);

				// reset render color to white
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				if (!foodConsumed)
				{
					newFood.renderFoodOnPos(spawnPosX, spawnPosY);
					
					if (partsArr[0].getPosX() == spawnPosX && partsArr[0].getPosY() == spawnPosY)
					{
						foodConsumed = true;
					}
				}
				else
				{
					// increase score
					// increase snake length
					BodyPart newBodyPart;
					int posLastPartX = partsArr[partsArr.size() - 1].getPosX();
					int posLastPartY = partsArr[partsArr.size() - 1].getPosY();
					newBodyPart.moveToPos(posLastPartX, posLastPartY);
					partsArr.push_back(newBodyPart);

					spawnPosX = (((std::rand() % (GAREA_UPPER_BOUND_X - GAREA_LOWER_BOUND_X + 1))) + GAREA_LOWER_BOUND_X) / OBJECT_DIMENSION * OBJECT_DIMENSION;
					spawnPosY = (((std::rand() % (GAREA_UPPER_BOUND_Y - GAREA_LOWER_BOUND_Y + 1))) + GAREA_LOWER_BOUND_Y) / OBJECT_DIMENSION * OBJECT_DIMENSION;

					// increase score
					score++;
					scoreText.str(""); // clear the string content first
					scoreText << "Score: " << score;

					// avoid spawnning food on body
					while (newBodyPart.isPositionOfPart(spawnPosX, spawnPosY))
					{
						std::cout << "a food spawnned on body!\n";
						spawnPosX = (((std::rand() % (GAREA_UPPER_BOUND_X - GAREA_LOWER_BOUND_X + 1))) + GAREA_LOWER_BOUND_X) / OBJECT_DIMENSION * OBJECT_DIMENSION;
						spawnPosY = (((std::rand() % (GAREA_UPPER_BOUND_Y - GAREA_LOWER_BOUND_Y + 1))) + GAREA_LOWER_BOUND_Y) / OBJECT_DIMENSION * OBJECT_DIMENSION;
					}
					foodConsumed = false;
				}

				if (!collided)
				{
					if (SDL_GetTicks() - currTime >= timeInterval)
					{
						currTime = SDL_GetTicks();
						
						// get the new position where head will be moved to
						int headNewPosX{ partsArr[0].getNewPosX(x) };
						int headNewPosY{ partsArr[0].getNewPosY(y) };
						/*
						if (headNewPosX < 0 || headNewPosX >= SCREEN_WIDTH || headNewPosY < 0 || headNewPosY >= SCREEN_HEIGHT)
						{
							collided = true;
						}
						*/
						// portal-like thingy :p
						if (headNewPosX < ((SCREEN_WIDTH - GAME_AREA_WIDTH) / 2))
						{
							headNewPosX = (SCREEN_WIDTH - GAME_AREA_WIDTH) / 2 + GAME_AREA_WIDTH - OBJECT_DIMENSION;
						}
						if (headNewPosX >= ((SCREEN_WIDTH - GAME_AREA_WIDTH) / 2 + GAME_AREA_WIDTH))
						{
							headNewPosX = (SCREEN_WIDTH - GAME_AREA_WIDTH) / 2;
						}
						if (headNewPosY < ((SCREEN_HEIGHT - GAME_AREA_HEIGHT) / 2))
						{
							headNewPosY = (SCREEN_HEIGHT - GAME_AREA_HEIGHT) / 2 + GAME_AREA_HEIGHT - OBJECT_DIMENSION;
						}
						if (headNewPosY >= ((SCREEN_HEIGHT - GAME_AREA_HEIGHT) / 2 + GAME_AREA_HEIGHT))
						{
							headNewPosY = (SCREEN_HEIGHT - GAME_AREA_HEIGHT) / 2;
						}

						// check if head's new position is gonna be it's own body
						if (partsArr[0].isPositionOfPart(headNewPosX, headNewPosY))
						{
							collided = true;
						}

						storePosX = headNewPosX;
						storePosY = headNewPosY;
						
						// update pos of head
						for (auto &part : partsArr)
						{
							int tempPosX, tempPosY;
							tempPosX = part.getPosX();
							tempPosY = part.getPosY();
							part.moveToPos(storePosX, storePosY);
							storePosX = tempPosX;
							storePosY = tempPosY;
						}
						goodToTakeInput = true;
					}

					for (int i = 0; i < partsArr.size(); i++)
					{
						partsArr[i].renderPart();
					}

					// render score display
					if (!gScoreTexture.loadTextTextureFromFile(scoreText.str().c_str(), textColor))
					{
						std::cout << "Unable to load score texture\n";
					}
					gScoreTexture.render((SCREEN_WIDTH - gScoreTexture.getWidth()) / 2, 0);
				}
				else
				{
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);
					gGameOverTextPrompt.render((SCREEN_WIDTH - gGameOverTextPrompt.getWidth()) / 2, 0);
					gRestartGuidePrompt.render((SCREEN_WIDTH - gRestartGuidePrompt.getWidth()) / 2, gGameOverTextPrompt.getHeight());
					goodToTakeInput = true;
				}

				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();
	return 0;
}