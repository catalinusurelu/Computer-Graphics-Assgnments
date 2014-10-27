#pragma once
#include "car.h"
class Enemy :
    public Car
{
public:
    Enemy(Visual2D* v2d, PhysicsEngine* physX, Color colorUpperBody, Color colorLowerBody, 
          Color colorWheels, bool fillUpperBody, bool fillLowerBody, bool fillWheels);
    virtual ~Enemy(void);
};

