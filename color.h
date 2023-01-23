#pragma once
#include "vector.h"

typedef unsigned char color_t;
class Color
{
public:
	color_t r, g, b, a;

	Color(color_t R = 255, color_t G = 255, color_t B = 255, color_t A = 255)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}

	unsigned int D3DInt()
	{
		return (
				(((a) & 0xFF) << 24) | 
				(((r) & 0xFF) << 16) | 
				(((g) & 0xFF) << 8)  | 
				((b) & 0xFF)
			);
	}

	Vector ToVector()
	{
		return { r / 255.f, g / 255.f, b / 255.f };
	}
};