#pragma once

#include "Movable.h"

class Car: public Movable
{
public:
    Car(void);
    Car(Visual2D* v2d, PhysicsEngine* physX, Color colorUpperBody, Color colorLowerBody, 
        Color colorWheels, bool fillUpperBody, bool fillLowerBody, bool fillWheels);
    virtual ~Car(void);

    void rotate_left() {};
    void rotate_right() {};
    void accelerate() {};
    void brake() {};

    float length;
    float width;
    float height;
};

