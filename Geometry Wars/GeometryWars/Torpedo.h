#pragma once
#include "Framework/Visual2D.h"
#include "Movable.h"

typedef Point2D Direction;

class Torpedo :
    public Movable
{
public:
    Torpedo(void);
    Torpedo(float shield_radius, Direction dir, Point2D center, Color color, Speed speed, PhysicsEngine* physX, Visual2D *v2d);
    virtual ~Torpedo(void);

    virtual void rotate_left();
	virtual void rotate_right();
	virtual void accelerate();
	virtual void brake();

    bool update();

private:
    int ttl;
    Direction dir;
    Visual2D *v2d;

    friend class Transform2D;
    friend class PhysicsEngine;
};

