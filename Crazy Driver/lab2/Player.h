#pragma once

#include "car.h"

class Player :
    public Car
{
public:
    Player(Visual2D* v2d, PhysicsEngine* physX, Color colorUpperBody, Color lowerBody, 
        Color colorWheels, bool fillUpperBody, bool fillLowerBody, bool fillWheels, int lives, int benzina);
    virtual ~Player(void);

    void rotate_left();
    void rotate_right();
    void accelerate();
    void brake();

    void redresare();

    static int lives;
    int score;
    float distantaParcursa;
    int benzina;

    float totalRotation;
};
