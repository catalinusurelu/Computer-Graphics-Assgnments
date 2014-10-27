#pragma once
#include "Color.h"


class Point2D
{
public:
	float x,y;
	//Color color;

public:
	Point2D()
	{
		x = 0;
		y = 0;
		//color.r = color.g = color.b = 0;
	}

	Point2D(float _x, float _y)
	{
		x = _x;
		y = _y;
		//color.r = color.g = color.b = 0;
	}

	/* Point2D(float _x, float _y, Color _color)
	{
		x = _x;
		y = _y;
		color.r = _color.r;
		color.g = _color.g;
		color.b = _color.b;
	}*/

	~Point2D()
	{}

    Point2D& operator+=(Point2D rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    Point2D& operator-=(Point2D rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    Point2D operator-()
    {
        return Point2D(-this->x, -this->y);
    }

    Point2D& operator*=(float scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        return *this;
    }

    Point2D operator*(float scalar)
    {
        return Point2D(this->x * scalar, this->y * scalar);
    }

    Point2D operator/(float scalar)
    {
        return Point2D(this->x / scalar, this->y / scalar);
    }
};
