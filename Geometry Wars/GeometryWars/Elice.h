#pragma once
#include "ship.h"

class Elice :
    public Ship
{
public:
    Elice(void);
    Elice(float shield_radius, Color color, PhysicsEngine* physX, Visual2D *v, int* scor);
    virtual ~Elice(void);

private:
    void computeCentroid();
    void accelerate();
};

