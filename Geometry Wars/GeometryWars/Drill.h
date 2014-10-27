#pragma once
#include "Framework/Polygon2D.h"

typedef Point2D Direction;

class Drill :
    public Polygon2D
{
public:
    Drill(void);
    Drill(float shield_radius, Direction dir, Point2D center, Color color);
    virtual ~Drill(void);

    static bool active;
};

