#pragma once
#include "Framework\Object2D.h"
#include "PhysicsEngine.h"
#include <list>

typedef Point2D Direction;
typedef Point2D Speed;
typedef std::list<Object2D*> Components;

class Movable: public Object2D
{
public:
    Movable(void);
    virtual ~Movable(void) = 0; //abstracta

    virtual void rotate_left() = 0;
	virtual void rotate_right() = 0;
	virtual void accelerate() = 0;
	virtual void brake() = 0;

protected:

    float omega;
    float speedLimit;
    Point2D centroid;
    Direction direction;
    Speed speed;
	Components parts;
    string type;

    PhysicsEngine *physX;

    friend class PhysicsEngine;
    friend class Transfrom2D;
    friend class Ship;
    friend void updateEnemies();
    friend inline float norm(Point2D a, Point2D b); 
    friend inline float norm(Point2D a);
};


inline float norm(Point2D a, Point2D b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)) ;
}

inline float norm(Point2D a)
{
    return norm(a, Point2D(0, 0));
}