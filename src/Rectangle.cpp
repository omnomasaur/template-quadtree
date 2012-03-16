//File: Rectangle.cpp
//Author: Eric Kasper
//About: Function definitions for rectangle.h

#include "Rectangle.h"
#include <cstdlib>

namespace qt
{
	Rectangle::Rectangle(){}

	Rectangle::Rectangle(int x, int y, int width, int height)
	{
		mPosition.x = (real)x;
		mPosition.y = (real)y;
		mWidth = (real)width;
		mHeight = (real)height;
	}

	Rectangle::Rectangle(real x, real y, real width, real height)
	{
		mPosition.x = x;
		mPosition.y = y;
		mWidth = width;
		mHeight = height;
	}

	Rectangle::Rectangle(Vector2 position, real width, real height)
	{
		mPosition = position;
		mWidth = width;
		mHeight = height;
	}
	
	void Rectangle::SetBounds(int x, int y, int width, int height)
	{
		mPosition.x = (real)x;
		mPosition.y = (real)y;
		mWidth = (real)width;
		mHeight = (real)height;
	}

	void Rectangle::SetBounds(real x, real y, real width, real height)
	{
		mPosition.x = x;
		mPosition.y = y;
		mWidth = width;
		mHeight = height;
	}

	void Rectangle::SetBounds(Vector2 position, real width, real height)
	{
		mPosition = position;
		mWidth = width;
		mHeight = height;
	}

	bool Rectangle::ContainsRect(Rectangle subRect)
	{
		return (subRect.mPosition.x >= mPosition.x &&
			subRect.mPosition.y >= mPosition.y &&
			subRect.mPosition.x + subRect.mWidth <= mPosition.x + mWidth &&
			subRect.mPosition.y + subRect.mHeight <= mPosition.y + mHeight);
	}

	Vector2 Rectangle::CollideDepth(Rectangle rectA, Rectangle rectB)
	{
		Vector2 returnVector;
		// Calculate half sizes.
		real halfWidthA = rectA.mWidth / 2.0f;
		real halfHeightA = rectA.mHeight / 2.0f;
		real halfWidthB = rectB.mWidth / 2.0f;
		real halfHeightB = rectB.mHeight / 2.0f;

		// Calculate centers.  
		Vector2 centerA;
		centerA.x = rectA.mPosition.x + halfWidthA;
		centerA.y = rectA.mPosition.y + halfHeightA;
		Vector2 centerB;
		centerB.x = rectB.mPosition.x + halfWidthB;
		centerB.y = rectB.mPosition.y + halfHeightB;

		// Calculate current and minimum-non-intersection distances between centers.
		real distanceX = centerA.x - centerB.x;
		real distanceY = centerA.y - centerB.y;
		real minDistanceX = halfWidthA + halfWidthB;
		real minDistanceY = halfHeightA + halfHeightB;

		// If not colliding return (0, 0).  WHAT WAS GOING ON HERE?!?!?!?!
		if(abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
		{
			returnVector.x = 0.0f;
			returnVector.y = 0.0f;
			return returnVector;
		}

		// Calculate and return collision depth.
		real depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
		real depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
		returnVector.x = depthX;
		returnVector.y = depthY;
		return returnVector;
	}
}