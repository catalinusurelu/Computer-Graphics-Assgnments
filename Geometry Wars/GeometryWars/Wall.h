#pragma once
#include "movable.h"

typedef list<Object2D*> Components;

class Wall :
    public Movable
{
public:
    Wall(Point2D colt, float width, float height, PhysicsEngine* physX);
    ~Wall(void);

    void rotate_left() {};
    void rotate_right() {};
    void accelerate() {};
    void brake() {};
};

