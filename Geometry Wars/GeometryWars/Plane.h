#pragma once
#include "ship.h"

class Plane :
    public Ship
{
public:
    Plane(void);
    Plane(float shield_radius, Color color, PhysicsEngine* physX, Visual2D *v, int* scor);
    virtual ~Plane(void);
};

