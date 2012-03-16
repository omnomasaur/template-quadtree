//File: Vector2.h
//Author: Eric Kasper
//About: A point in 2D space with accompanying functions.

#pragma once

#include "Precision.h"
#include <math.h>

namespace qt
{
	class Vector2
	{
	public:
		real x;
		real y;

		Vector2() : x(0), y(0) {}
		~Vector2();

		Vector2(const real x, const real y)
			: x(x), y(y) {}

		bool operator==(const Vector2& vector)
		{
			if( x == vector.x && y == vector.y )
				return true;

			return false;
		}

		bool operator!=(const Vector2& vector)
		{
			if ( x != vector.x || y != vector.y)
				return true;

			else return false;
		}

		void operator=(const Vector2& vector)
		{
			x = vector.x;
			y = vector.y;
		}

		void operator*=(const real value)
		{
			x *= value;
			y *= value;
		}

		Vector2 operator*(const real value) const
		{
			return Vector2(x*value, y*value);
		}

		real DotProduct(const Vector2& vector) const
		{
			return x*vector.x + y*vector.y;
		}

		real operator*(const Vector2& vector) const
		{
			return x*vector.x + y*vector.y;
		}

		void operator+=(const Vector2& v)
		{
			x += v.x;
			y += v.y;
		}

		Vector2 operator+(const Vector2& v) const
		{
			return Vector2(x+v.x, y+v.y);
		}

		void operator-=(const Vector2& v)
		{
			x -= v.x;
			y -= v.y;
		}

		Vector2 operator-(const Vector2& v) const
		{
			return Vector2(x-v.x, y-v.y);
		}

		void AddScaledVector(const Vector2& vector, real scale)
		{
			x += vector.x * scale;
			y += vector.y * scale;
		}

		void Invert();

		real Distance() const { return sqrt( x*x + y*y ); }

		real DistanceSquared() const { return x*x + y*y; }

		void Normalize();
	};
}