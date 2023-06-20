#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

/* GLOBAL CONSTANTS */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BOARD_LENGTH = 385; // sqaure board with length 385
const int SQ_LENGTH = 115; // length of each box
const int LN_LENGTH = 10; // width of partition
const int TOTAL_SQUARES = 9; // no of squares

class LTexture
{
public:
	LTexture(); // initialize members
	~LTexture(); // deallocates memory
	bool loadFromFile(std::string path); // load image at specified path
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor); // load text texture ( fonts ) of specified color
	void free();
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// get image dimension
	int getWidth();
	int getHeight();
private:
	SDL_Texture* mTexture; // actual hardware texture

	// image dimensions
	int mWidth;
	int mHeight;
};

class LButton
{
public:
	LButton();
	void addPiece(int index, SDL_Rect* noughtClip); // spawn a piece at gridRect
	void setButtonRect(int x, int y); // sets the rect of current button
	void placeButton(); // render the button
	bool mouseClickedInside(int x, int y); // checks if mouse position is inside the button when clicked
private:
	// button position
	SDL_Rect buttonRect;
};

class LPiece
{
public:
	LPiece();
	void setPosition(int a, int b); // sets the position of the piece
	void setClip(SDL_Rect* clip); // sets the clip of the piece in the sprite sheet
	void render();
private:
	// position
	int x, y;
	SDL_Rect* pieceClip;
};

class LGameManager
{
public:
	LGameManager();
	void clearRenderer(); // clears the renderer
	void setButtons(); // sets the buttons on board
	void setupBoard(); // sets the board
	void resetGameStates(); // resets the game state variables
	void changeState();
	void changeTurn();
	bool placePieceOnBoard(int buttonIndex);
	void updateGameStateAfterTurn(int positionOnBoard);
	int checkWinLoseDraw(); // returns 0 for draw, 1 for p1 win and 2 for p2 win
	bool actionWinLoseDraw(); // returns true if action happened
	void renderPiecesOnBoard();
	bool getWinLoseDraw();
private:
	// game state variables
	bool winLoseDraw;
	int turnsPlayed;
	int placedPiecePos[9]; // all pieces placed on board and position
	int player1Pos[9]; // positions of player 1 pieces on board
	int player2Pos[9]; // positions of player 2 pieces on board
	int currentTurn;
	SDL_Rect* currentPiece;

};

bool init(); // initialize SDL
bool loadMedia(); // load all media
void close(); // shut down SDL subsystems

/* GLOBAL VARIABLES */
SDL_Window* gWindow{ NULL };
SDL_Surface* gScreenSurface{ NULL };
SDL_Renderer* gRenderer{ NULL };
SDL_Texture* gTexture{ NULL };

// globally used font
TTF_Font* gFont = NULL;
LTexture gTextTexture;

// game images
LTexture gBoard;
enum TicTacToeComponent
{
	NOUGHT,
	CROSS,
	TOTAL_PIECES
};
SDL_Rect gGamePiecesClips[TOTAL_PIECES];
LTexture gGamePiecesSpriteSheet;

LButton buttons[TOTAL_SQUARES]; // buttons on grid
LPiece piecesOnBoard[TOTAL_SQUARES];

LTexture gWinLoseDrawPrompt;
LTexture gResetBoardPrompt;

// enum rep for player 1 and 2
enum PlayerTurn
{
	PLAYER_ONE,
	PLAYER_TWO,
};

LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	// free previous texture
	free();
	SDL_Texture* newTexture = NULL;
	
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load image " << path.c_str() << "! SDL_image ERROR: " << IMG_GetError() << '\n';
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << "Unable to create texture from " << path.c_str() << "! SDL_image ERROR: " << IMG_GetError() << '\n';
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
	}
	mTexture = newTexture;
	return (mTexture != NULL);
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	// get rid of existing texture
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
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
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return (mTexture != NULL);
}

void LTexture::free()
{
	if (mTexture != NULL)
	{
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* point, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, point, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

// LButton functions ...
LButton::LButton()
{
	buttonRect = { 0, 0, SQ_LENGTH, SQ_LENGTH };
}

void LButton::addPiece(int index, SDL_Rect* noughtClip)
{
	// add piece to the pieces on board
	LPiece newPiece;
	newPiece.setPosition(buttonRect.x, buttonRect.y);
	newPiece.setClip(noughtClip);
	piecesOnBoard[index] = newPiece; // subs the piece at index with newPiece

	// here, check the array for not placing pieces at same spot
}

void LButton::setButtonRect(int x, int y)
{
	buttonRect = { x, y, SQ_LENGTH, SQ_LENGTH };
}

void LButton::placeButton()
{
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0x22);
	SDL_RenderFillRect(gRenderer, &buttonRect);
}

bool LButton::mouseClickedInside(int x, int y)
{
	bool isInside{ false };

	// true only if mouse position inside bound of button
	if (x > buttonRect.x && x < (buttonRect.x + buttonRect.w) && y > buttonRect.y && y < (buttonRect.y + buttonRect.h))
	{
		isInside = true;
	}

	return isInside;
}

// LPiece functions
LPiece::LPiece()
{
	x = 0;
	y = 0;
	pieceClip = NULL;
}

void LPiece::setPosition(int a, int b)
{
	x = a;
	y = b;
}

void LPiece::setClip(SDL_Rect* clip)
{
	pieceClip = clip;
}

void LPiece::render()
{
	if (pieceClip != NULL)
	{
		gGamePiecesSpriteSheet.render(x, y, pieceClip);
	}
}

// LGameManager functions
LGameManager::LGameManager()
{
	winLoseDraw = false;
	turnsPlayed = 0;
	for (int i = 0; i < 9; ++i)
	{
		placedPiecePos[i] = 0;
		player1Pos[i] = 0;
		player2Pos[i] = 0;
	}
	currentTurn = PLAYER_ONE;
	currentPiece = &gGamePiecesClips[NOUGHT];
}

void LGameManager::clearRenderer()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
}

void LGameManager::setButtons()
{
	// here value 10 is the thickness of margin between squares and also the padding of board
	buttons[0].setButtonRect((SCREEN_WIDTH - SQ_LENGTH) / 2 - (SQ_LENGTH + 10), (SCREEN_HEIGHT - SQ_LENGTH) / 2 - (SQ_LENGTH + 10));
	buttons[1].setButtonRect((SCREEN_WIDTH - SQ_LENGTH) / 2, (SCREEN_HEIGHT - SQ_LENGTH) / 2 - (SQ_LENGTH + 10));
	buttons[2].setButtonRect((SCREEN_WIDTH - SQ_LENGTH) / 2 + (SQ_LENGTH + 10), (SCREEN_HEIGHT - SQ_LENGTH) / 2 - (SQ_LENGTH + 10));
	buttons[3].setButtonRect((SCREEN_WIDTH - SQ_LENGTH) / 2 - (SQ_LENGTH + 10), (SCREEN_HEIGHT - SQ_LENGTH) / 2);
	buttons[4].setButtonRect((SCREEN_WIDTH - SQ_LENGTH) / 2, (SCREEN_HEIGHT - SQ_LENGTH) / 2);
	buttons[5].setButtonRect((SCREEN_WIDTH - SQ_LENGTH) / 2 + (SQ_LENGTH + 10), (SCREEN_HEIGHT - SQ_LENGTH) / 2);
	buttons[6].setButtonRect((SCREEN_WIDTH - SQ_LENGTH) / 2 - (SQ_LENGTH + 10), (SCREEN_HEIGHT - SQ_LENGTH) / 2 + (SQ_LENGTH + 10));
	buttons[7].setButtonRect((SCREEN_WIDTH - SQ_LENGTH) / 2, (SCREEN_HEIGHT - SQ_LENGTH) / 2 + (SQ_LENGTH + 10));
	buttons[8].setButtonRect((SCREEN_WIDTH - SQ_LENGTH) / 2 + (SQ_LENGTH + 10), (SCREEN_HEIGHT - SQ_LENGTH) / 2 + (SQ_LENGTH + 10));
}

void LGameManager::setupBoard()
{
	gBoard.render((SCREEN_WIDTH - BOARD_LENGTH) / 2, (SCREEN_HEIGHT - BOARD_LENGTH) / 2);
	setButtons(); // sets buttons to it's grid position

	for (int i = 0; i < TOTAL_SQUARES; ++i)
	{
		buttons[i].placeButton();
	}
}

void LGameManager::resetGameStates()
{
	turnsPlayed = 0;
	for (int i = 0; i < 9; ++i)
	{
		placedPiecePos[i] = 0;
		player1Pos[i] = 0;
		player2Pos[i] = 0;
		piecesOnBoard[i] = LPiece();
	}
	winLoseDraw = false;
}

void LGameManager::changeTurn()
{
	// change turn and piece respective to turn
	if (currentTurn == PLAYER_ONE)
	{
		currentTurn = PLAYER_TWO;
		currentPiece = &gGamePiecesClips[CROSS];
	}
	else
	{
		currentTurn = PLAYER_ONE;
		currentPiece = &gGamePiecesClips[NOUGHT];
	}
}

bool LGameManager::placePieceOnBoard(int buttonIndex)
{
	bool isGood{ true };
	if (placedPiecePos[buttonIndex] == 1)
	{
		isGood = false;
	}
	else
	{
		buttons[buttonIndex].addPiece(buttonIndex, currentPiece);
	}
	return isGood;
}

void LGameManager::updateGameStateAfterTurn(int positionOnBoard)
{
	turnsPlayed++;
	placedPiecePos[positionOnBoard] = 1;
	if (currentTurn == PLAYER_ONE)
	{
		player1Pos[positionOnBoard] = 1;
	}
	else if (currentTurn == PLAYER_TWO)
	{
		player2Pos[positionOnBoard] = 1;
	}
}

int LGameManager::checkWinLoseDraw()
{
	int finalCondition = 0; // win or lose or draw
	int lastTurn = currentTurn == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
	if (lastTurn == PLAYER_ONE)
	{
		if (player1Pos[0] == 1 && player1Pos[1] == 1 && player1Pos[2] == 1)
		{
			finalCondition = 1;
		}
		else if (player1Pos[3] == 1 && player1Pos[4] == 1 && player1Pos[5] == 1)
		{
			finalCondition = 1;
		}
		else if (player1Pos[6] == 1 && player1Pos[7] == 1 && player1Pos[8] == 1)
		{
			finalCondition = 1;
		}
		else if (player1Pos[0] == 1 && player1Pos[3] == 1 && player1Pos[6] == 1)
		{
			finalCondition = 1;
		}
		else if (player1Pos[1] == 1 && player1Pos[4] == 1 && player1Pos[7] == 1)
		{
			finalCondition = 1;
		}
		else if (player1Pos[2] == 1 && player1Pos[5] == 1 && player1Pos[8] == 1)
		{
			finalCondition = 1;
		}
		else if (player1Pos[0] == 1 && player1Pos[4] == 1 && player1Pos[8] == 1)
		{
			finalCondition = 1;
		}
		else if (player1Pos[2] == 1 && player1Pos[4] == 1 && player1Pos[6] == 1)
		{
			finalCondition = 1;
		}
	}
	else if (lastTurn == PLAYER_TWO)
	{
		if (player2Pos[0] == 1 && player2Pos[1] == 1 && player2Pos[2] == 1)
		{
			finalCondition = 2;
		}
		else if (player2Pos[3] == 1 && player2Pos[4] == 1 && player2Pos[5] == 1)
		{
			finalCondition = 2;
		}
		else if (player2Pos[6] == 1 && player2Pos[7] == 1 && player2Pos[8] == 1)
		{
			finalCondition = 2;
		}
		else if (player2Pos[0] == 1 && player2Pos[3] == 1 && player2Pos[6] == 1)
		{
			finalCondition = 2;
		}
		else if (player2Pos[1] == 1 && player2Pos[4] == 1 && player2Pos[7] == 1)
		{
			finalCondition = 2;
		}
		else if (player2Pos[2] == 1 && player2Pos[5] == 1 && player2Pos[8] == 1)
		{
			finalCondition = 2;
		}
		else if (player2Pos[0] == 1 && player2Pos[4] == 1 && player2Pos[8] == 1)
		{
			finalCondition = 2;
		}
		else if (player2Pos[2] == 1 && player2Pos[4] == 1 && player2Pos[6] == 1)
		{
			finalCondition = 2;
		}
	}

	return finalCondition;
}

bool LGameManager::actionWinLoseDraw()
{
	SDL_Color txtColor = { 0, 0, 0, 255 }; // color for prompt text

	if (turnsPlayed >= 9 && checkWinLoseDraw() == 0)
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		std::string drawPrompt{ "Draw! " };
		gWinLoseDrawPrompt.loadFromRenderedText(drawPrompt.c_str(), txtColor);
		gWinLoseDrawPrompt.render((SCREEN_WIDTH - gWinLoseDrawPrompt.getWidth()) / 2, (SCREEN_HEIGHT - gWinLoseDrawPrompt.getHeight()) / 2);
		winLoseDraw = true;
	}
	else if (checkWinLoseDraw() == 1)
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		std::string p1WinPrompt{ "Player 1 Win! " };
		gWinLoseDrawPrompt.loadFromRenderedText(p1WinPrompt.c_str(), txtColor);
		gWinLoseDrawPrompt.render((SCREEN_WIDTH - gWinLoseDrawPrompt.getWidth()) / 2, (SCREEN_HEIGHT - gWinLoseDrawPrompt.getHeight()) / 2);
		winLoseDraw = true;
	}
	else if (checkWinLoseDraw() == 2)
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		std::string p2WinPrompt{ "Player 2 Win! " };
		gWinLoseDrawPrompt.loadFromRenderedText(p2WinPrompt.c_str(), txtColor);
		gWinLoseDrawPrompt.render((SCREEN_WIDTH - gWinLoseDrawPrompt.getWidth()) / 2, (SCREEN_HEIGHT - gWinLoseDrawPrompt.getHeight()) / 2);
		winLoseDraw = true;
	}
	return winLoseDraw;
}

void LGameManager::renderPiecesOnBoard()
{
	for (LPiece piece : piecesOnBoard)
	{
		piece.render();
	}
}

bool LGameManager::getWinLoseDraw()
{
	return winLoseDraw;
}

// initialize SDL
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
		// set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			std::cout << "Warning: Linear texture filtering not enabled!\n";
		}

		gWindow = SDL_CreateWindow("cross out", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

				// init png loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "SDL_Image could not be initialized! SDL_image ERROR: " << IMG_GetError() << '\n';
					success = false;
				}

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

// load all images ( media )
bool loadMedia()
{
	bool success{ true };

	if (!gBoard.loadFromFile("images/board.png"))
	{
		std::cout << "Failed to load board image!\n";
		success = false;
	}

	if (!gGamePiecesSpriteSheet.loadFromFile("images/XO.png"))
	{
		std::cout << "Failed to load game pieces sprite!\n";
		success = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_PIECES; ++i)
		{
			gGamePiecesClips[i].x = i * 115;
			gGamePiecesClips[i].y = 0;
			gGamePiecesClips[i].w = SQ_LENGTH;
			gGamePiecesClips[i].h = SQ_LENGTH;
		}
	}

	gFont = TTF_OpenFont("fonts/lazy.ttf", 28);
	if (gFont == NULL)
	{
		std::cout << "Failed to load font!\n";
		success = false;
	}

	return success;
}

// close SDL and shut down all SDL subsystems
void close()
{
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;

	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
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
			LGameManager gameManager;
			bool quit{ false };
			SDL_Event e;

			// main loop ( game loop )
			while (!quit)
			{
				// event loop
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_MOUSEBUTTONDOWN && gameManager.getWinLoseDraw())
					{
						gameManager.resetGameStates();
					}
					else if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						int x, y;
						SDL_GetMouseState(&x, &y);

						// for each button in grid
						for (int i = 0; i < TOTAL_SQUARES; i++)
						{
							// if mouse position inside button
							if (buttons[i].mouseClickedInside(x, y))
							{
								if (!gameManager.placePieceOnBoard(i))
								{
									break;
								}
								gameManager.updateGameStateAfterTurn(i);

								// can get fucked while rendering stuff` but for now it's working
								// if win lose or draw action was taken, break;
								gameManager.changeTurn();
							}
						}
					}
				}
				gameManager.clearRenderer();
				gameManager.setupBoard();
				gameManager.renderPiecesOnBoard();

				if (gameManager.actionWinLoseDraw())
				{
					SDL_Color txtColor = { 0, 0, 0, 255 };
					gResetBoardPrompt.loadFromRenderedText("Click anywhere to reset game!", txtColor);
					gResetBoardPrompt.render((SCREEN_WIDTH - gResetBoardPrompt.getWidth()) / 2, (SCREEN_HEIGHT - gResetBoardPrompt.getHeight()));
				}

				// display rendered stuff
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();
	return 0;
}