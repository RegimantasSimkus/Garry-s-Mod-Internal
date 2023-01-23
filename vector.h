#pragma once

typedef float vec_t;

class Vector
{
public:
	vec_t x, y, z;

	Vector(void)
	{
		x = y = z = 0;
	}

	Vector(vec_t X, vec_t Y = 0.f, vec_t Z = 0.f)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vector& Init()
	{
		x = y = z = 0;
		return *this;
	}
};