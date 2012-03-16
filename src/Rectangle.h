//File: Rectangle.h
//Author: Eric Kasper
//About: Rectangle class.  Mainly for use with collision detection.  

#pragma once

#include "Vector2.h"

namespace qt
{
	class Rectangle
	{
	public:
		Vector2 mPosition;
		real mWidth;
		real mHeight;

		Rectangle();

		Rectangle(int x, int y, int width, int height);

		Rectangle(real x, real y, real width, real height);

		Rectangle(Vector2 position, real width, real height);
		
		void SetBounds(int x, int y, int width, int height);

		void SetBounds(real x, real y, real width, real height);
		
		void SetBounds(Vector2 position, real width, real height);

		bool ContainsRect(Rectangle subRect);

		static Vector2 CollideDepth(Rectangle rect1, Rectangle rect2);
	};
}