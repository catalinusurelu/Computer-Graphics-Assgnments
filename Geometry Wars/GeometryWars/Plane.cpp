#include "Plane.h"
#include "Framework\Polygon2D.h"
#include "Framework\Transform2D.h"
#include "PhysicsProperties.h"

using namespace PhysicsProperties;

Plane::Plane(float shield_radius, Color color, PhysicsEngine* physX, Visual2D *v, int* scor)
    :Ship(physX, v, scor)
{
    type = "enemy";
    speedLimit = speedOfLight / 1.3;
    this->radius = shield_radius;

    Polygon2D *wing1, *wing2;
    Point2D p1, p2;

    // Wing 1
    wing1 = new Polygon2D(color, false);
    p1 = Point2D(shield_radius * 1.5, 0);
    Transform2D::loadIdentityMatrix();
    Transform2D::rotateMatrix(45);
    Transform2D::applyTransform(&p1, &p1);

    p2 = Point2D(shield_radius * 4 / 3, 0);
    Transform2D::loadIdentityMatrix();
    Transform2D::rotateMatrix(-60);
    Transform2D::applyTransform(&p2, &p2);

    wing1->addPoint(Point2D(0, 0));
    wing1->addPoint(p1);
    wing1->addPoint(p2);

    parts.push_back(wing1);

    // Wing 2 oglindita si rotita putin
    wing2 = new Polygon2D(color, false);
    wing2->addPoint(Point2D(0, 0));
    wing2->addPoint(Point2D(p1.y, p1.x));
    wing2->addPoint(Point2D(p2.y, p2.x));

    Transform2D::loadIdentityMatrix();
    Transform2D::translateMatrix(-p1.x, -p1.y);
    Transform2D::rotateMatrix(-10);
    Transform2D::translateMatrix( p1.x, p1.y);
    Transform2D::applyTransform_o(wing2);

    parts.push_back(wing2);

    computeCentroid();
    addShip_to_Visual2D(this, v);
    physX->addObject(this);
}

Plane::Plane(void)
{
}



Plane::~Plane(void)
{
    *score += 400;
}
