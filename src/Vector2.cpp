//File: Vector2.h
//Author: Eric Kasper
//About: A point in 2D space with accompanying functions.

#include "Vector2.h"

namespace qt
{
	Vector2::~Vector2()
	{
	}

	void Vector2::Invert()
	{
		x = -x;
		y = -y;
	}

	void Vector2::Normalize()
	{
		real l = Distance();

		if( l > 0 )
		{
			(*this) *= ((real)1)/l;
		}
	}
}