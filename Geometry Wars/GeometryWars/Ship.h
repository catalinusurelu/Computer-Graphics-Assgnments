#pragma once
#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Movable.h"

class Ship: public Movable
{
public:
	Ship(void);
    Ship(PhysicsEngine* physX, Visual2D *v, int* scor);
    Ship(PhysicsEngine* physX, Visual2D *v);
	virtual ~Ship(void);

    virtual void rotate_left();
	virtual void rotate_right();
	virtual void accelerate();
	virtual void brake();

    virtual void updateAI(Ship* target, bool drill);

	virtual void computeCentroid();
    virtual void move(float x, float y);
    virtual void scale(float x, float y);

    friend void addShip_to_Visual2D(Ship *ship, Visual2D *v);
    friend void removeShip_from_Visual2D(Ship *ship, Visual2D *v);

protected:

    Visual2D *v2d;
    float radius;
    int* score;

    friend class Transform2D;
    friend class PhysicsEngine;
};

void addShip_to_Visual2D(Ship *ship, Visual2D *v);
void removeShip_from_Visual2D(Ship *ship, Visual2D *v);

float produsCartezian(const Point2D& a, const Point2D& b, const Point2D& c);
bool seIntersecteaza(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d);