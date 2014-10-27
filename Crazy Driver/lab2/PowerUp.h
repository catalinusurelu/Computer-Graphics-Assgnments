#pragma once
#include "movable.h"
class PowerUp :
    public Movable
{
public:
    PowerUp(Visual2D* v2d, PhysicsEngine* physX, bool fill);
    virtual ~PowerUp(void);

    void rotate_left() {};
    void rotate_right() {};
    void accelerate() {};
    void brake() {};

    void updateColor();
    int score;

private:
    int frame;
};

