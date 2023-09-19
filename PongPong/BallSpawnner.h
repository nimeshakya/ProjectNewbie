#include <vector>
#include <random>
#include <SDL.h>

#ifndef BALL_SPAWNNER
#define BALL_SPAWNNER

class BallSpawnner
{
public:
	BallSpawnner();
	~BallSpawnner();

	// adds to vector array mYpositions the y-coordinate of spawn positions
	void calculateSpawnPosition();

	// renders a sqare to indicate spawn position
	void render();

	// returns a random spawn point along the spawnner position with mYpositions and mPosX
	SDL_Point getSpawnPoint(std::mt19937 generator);
private:
	std::vector<int> mYpositions;
	int mPosX;
};

#endif // !BALL_SPAWNNER
