#include "Paddle.h"

void Paddle::Update(double deltaTime)
{
	m_position += Vec2{ 0, m_velY } * PADDLE_SPEED * deltaTime;
}

// handles movement through input (isPlayerOne decides which keys to take as input)
void Paddle::HandleMovement(bool isPlayerOne)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (isPlayerOne)
	{
		if (currentKeyStates[SDL_SCANCODE_W] && !(m_position.y < UNIT_LENGTH))
		{
			m_velY = -1.0;
		}
		else if (currentKeyStates[SDL_SCANCODE_S] && !(m_position.y > SCREEN_HEIGHT - UNIT_LENGTH - m_height))
		{
			m_velY = 1.0;
		}
		else
		{
			m_velY = 0.0;
		}
	}
	else
	{
		if (currentKeyStates[SDL_SCANCODE_UP] && !(m_position.y < UNIT_LENGTH))
		{
			m_velY = -1.0;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN] && !(m_position.y > SCREEN_HEIGHT - UNIT_LENGTH - m_height))
		{
			m_velY = 1.0;
		}
		else
		{
			m_velY = 0.0;
		}
	}
}