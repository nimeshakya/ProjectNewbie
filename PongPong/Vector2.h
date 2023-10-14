#pragma once
class Vector2
{
public:
	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	Vector2(float a, float b)
	{
		x = a;
		y = b;
	}

	Vector2 operator+(Vector2 const& rhs)
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	Vector2& operator+=(Vector2 const& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2 operator*(float rhs)
	{
		return Vector2(x * rhs, y * rhs);
	}

	float x, y;
};