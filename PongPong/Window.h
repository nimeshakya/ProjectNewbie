#include <SDL.h>

#ifndef WINDOW
#define WINDOW

enum class PlayerType
{
	PLAYER_ONE,
	PLAYER_TWO
};

inline constexpr int UNIT_SIZE{ 20 };

// screen dimension
inline constexpr int SCREEN_WIDTH{ 640 };
inline constexpr int SCREEN_HEIGHT{ 480 };

// rendering variables
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

// board colors
inline constexpr SDL_Color gBoardColor{ 0xCC, 0xCC, 0xCC, 0xFF };
inline constexpr SDL_Color gObjColor{ 0x22, 0x22, 0x22, 0xFF };


#endif // !WINDOW