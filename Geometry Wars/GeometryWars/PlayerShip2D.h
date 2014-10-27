#pragma once

#include "Framework/Polygon2D.h"

class PlayerShip2D: public Polygon2D
{
public:
	PlayerShip2D(void);
	PlayerShip2D(float shield_radius, Color color);
	~PlayerShip2D(void);

    Point2D centroid;
};

