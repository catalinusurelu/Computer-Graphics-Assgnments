#pragma once
#include "Framework\Object3D.h"
#include "Framework\Visual2D.h"
#include "PhysicsEngine.h"
#include <deque>

typedef std::deque<Object3D*> Components;

class Movable
{
public:
    Movable(void);
    Movable(Visual2D* v2d, PhysicsEngine* physX, bool fill);
    virtual ~Movable(void); //abstracta

    virtual void rotate_left() = 0;
	virtual void rotate_right() = 0;
	virtual void accelerate() = 0;
	virtual void brake() = 0;

    void updateBoundingBox();
    void move(float x, float y, float z);

    struct BoundingBox
    {
        float upper;
        float lower;
        float left;
        float right;
    } boundingBox;

    void addMovable_to_Visual2D(Movable *ship, Visual2D *v);
    void removeMovable_from_Visual2D(Movable *ship, Visual2D *v);

    Components parts;

protected:

    string type;
    float speed;
    float rotation;
    bool fill;

    Point3D center;

    PhysicsEngine *physX;
    Visual2D* v2d;

    friend class PhysicsEngine;
    friend class Transfrom3D;
    friend class DrawingWindow;
};