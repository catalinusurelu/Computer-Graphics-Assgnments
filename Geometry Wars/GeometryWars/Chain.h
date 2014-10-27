#pragma once
#include "ship.h"

class Chain :
    public Ship
{
public:
    Chain(void);
    Chain(float side, Color color, PhysicsEngine* physX, Visual2D *v, int* scor);
    virtual ~Chain(void);
};

