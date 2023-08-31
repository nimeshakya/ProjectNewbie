#include <vector>

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
private:
	std::vector<int> mYpositions;
	int mPosX;
};

#endif // !BALL_SPAWNNER
