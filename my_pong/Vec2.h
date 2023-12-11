#ifndef VEC2_H
#define VEC2_H

class Vec2
{
public:
	Vec2(double a = 0, double b = 0) : x{ a }, y{ b } {};

	Vec2 operator+=(Vec2 u)
	{
		return Vec2{ x += u.x, y += u.y };
	}
	Vec2 operator*(double k)
	{
		return Vec2{ x * k, y * k };
	}
public:
	double x, y;
};

#endif // !VEC2_H
