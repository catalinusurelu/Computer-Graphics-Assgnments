#include "Drill.h"
#include "Framework/Transform2D.h"


Drill::Drill(float shield_radius, Direction dir, Point2D center, Color color)
    : Polygon2D(color, true)
{
    Point2D normalToBaseOfDrill = Point2D(dir.x * shield_radius * 1.3, dir.y * shield_radius * 1.3);

    Transform2D::loadIdentityMatrix();
    Transform2D::translateMatrix(center.x, center.y);
    Transform2D::applyTransform(&normalToBaseOfDrill, &normalToBaseOfDrill);

    Point2D p1, p2, p3;

    p1 = Point2D(dir.x * shield_radius * 2 / 3, dir.y * shield_radius * 2 / 3);

    Transform2D::loadIdentityMatrix();
    Transform2D::rotateMatrix(90);
    Transform2D::translateMatrix(normalToBaseOfDrill.x, normalToBaseOfDrill.y);
    Transform2D::applyTransform(&p1, &p1);

     
    p2 = Point2D(dir.x * shield_radius * 2 / 3, dir.y * shield_radius * 2 / 3);

    Transform2D::loadIdentityMatrix();
    Transform2D::rotateMatrix(-90);
    Transform2D::translateMatrix(normalToBaseOfDrill.x, normalToBaseOfDrill.y);
    Transform2D::applyTransform(&p2, &p2);


    p3 = Point2D(dir.x * shield_radius * 2.5, dir.y * shield_radius * 2.5);
    Transform2D::loadIdentityMatrix();
    Transform2D::translateMatrix(normalToBaseOfDrill.x, normalToBaseOfDrill.y);
    Transform2D::applyTransform(&p3, &p3);

    addPoint(p1);
    addPoint(p2);
    addPoint(p3);
    addPoint(p1);

    active = true;
}

Drill::Drill(void)
{
}


Drill::~Drill(void)
{
    active = false;
}

bool Drill::active = false;