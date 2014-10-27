#include "Chain.h"
#include "Framework\Polygon2D.h"
#include "Framework\Transform2D.h"
#include "PhysicsProperties.h"

using namespace PhysicsProperties;

Chain::Chain(float side, Color color, PhysicsEngine* physX, Visual2D *v, int* scor)
    :Ship(physX, v, scor)
{
    type = "enemy";
    side *= 1.5;
    speedLimit = speedOfLight / 1.3;
    radius = side;

    // Patrat 1
    Polygon2D* rect = new Polygon2D(color, false);
    rect->addPoint(Point2D(side * (float)1/3, 0));
    rect->addPoint(Point2D(-side * (float)2/3, 0));
    rect->addPoint(Point2D(-side * (float)2/3, side));
    rect->addPoint(Point2D(side * (float)1/3, side));

    parts.push_back(rect);

    // Patrat 2
    rect = new Polygon2D(color, false);
    rect->addPoint(Point2D(0, side * (float)1/3));
    rect->addPoint(Point2D(side, side * (float)1/3));
    rect->addPoint(Point2D(side , -side * (float)2/3));
    rect->addPoint(Point2D(0, -side * (float)2/3));

    parts.push_back(rect);

    computeCentroid();
    addShip_to_Visual2D(this, v);
    physX->addObject(this);
}

Chain::Chain(void)
{
}


Chain::~Chain(void)
{
    *score += 200;
}
