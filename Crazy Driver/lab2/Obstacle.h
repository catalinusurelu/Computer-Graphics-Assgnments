#pragma once
#include "Movable.h"
class Obstacle :
    public Movable
{
public:
    Obstacle(Visual2D* v2d, PhysicsEngine* physX, Color color, bool fill);
    virtual ~Obstacle(void);

    void rotate_left();
	void rotate_right();
	void accelerate();
	void brake();

    static float n;
};

