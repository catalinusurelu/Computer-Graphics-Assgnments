#include "Elice.h"
#include "Framework\Circle2D.h"
#include "Framework\Polygon2D.h"
#include "Framework\Transform2D.h"
#include "PhysicsProperties.h"

using namespace PhysicsProperties;

Elice::Elice(void)
{
}

Elice::Elice(float radius, Color color, PhysicsEngine* physX, Visual2D *v, int* scor)
    :Ship(physX, v, scor)
{
    type = "enemy";

	parts.push_back(new Circle2D(Point2D(0, 0), radius / 5, color, false));

    Polygon2D *blade1, *blade2, *blade3, *blade4;
    radius *= 1.5;
    this->radius = radius;


    // Blade 1
    blade1 = new Polygon2D(color, false);
    blade1->addPoint(Point2D(0, 0));

    // triunghi dreptunghic cerc - a^2 + a^2 = r^2
    // r = distanta de la cercul interior la raza externa
    blade1->addPoint(Point2D(sqrt((pow(radius * 4 / 5, 2) / 2)), -sqrt((pow(radius * 4 / 5, 2) / 2)))); 
    blade1->addPoint(Point2D(radius / 2, 0));

    Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(radius / 5, 0);
    Transform2D::applyTransform(blade1);
    
    parts.push_back(blade1);


    // Blade 2
    blade2 = new Polygon2D(color, false);
    blade2->addPoint(Point2D(0, 0));

    // triunghi dreptunghic cerc - a^2 + a^2 = r^2
    // r = distanta de la cercul interior la raza externa
    blade2->addPoint(Point2D(sqrt((pow(radius * 4 / 5, 2) / 2)), -sqrt((pow(radius * 4 / 5, 2) / 2)))); 
    blade2->addPoint(Point2D(radius / 2, 0));

    Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(radius / 5, 0);
    Transform2D::rotateMatrix(90);
    Transform2D::applyTransform(blade2);

    parts.push_back(blade2);


    // Blade 3
    blade3 = new Polygon2D(color, false);
    blade3->addPoint(Point2D(0, 0));

    // triunghi dreptunghic cerc - a^2 + a^2 = r^2
    // r = distanta de la cercul interior la raza externa
    blade3->addPoint(Point2D(sqrt((pow(radius * 4 / 5, 2) / 2)), -sqrt((pow(radius * 4 / 5, 2) / 2)))); 
    blade3->addPoint(Point2D(radius / 2, 0));

    Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(radius / 5, 0);
    Transform2D::rotateMatrix(180);
    Transform2D::applyTransform(blade3);

    parts.push_back(blade3);

    // Blade 4
    blade4 = new Polygon2D(color, false);
    blade4->addPoint(Point2D(0, 0));

    // triunghi dreptunghic cerc - a^2 + a^2 = r^2
    // r = distanta de la cercul interior la raza externa
    blade4->addPoint(Point2D(sqrt((pow(radius * 4 / 5, 2) / 2)), -sqrt((pow(radius * 4 / 5, 2) / 2)))); 
    blade4->addPoint(Point2D(radius / 2, 0));

    Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(radius / 5, 0);
    Transform2D::rotateMatrix(270);
    Transform2D::applyTransform(blade4);

    parts.push_back(blade4);

    computeCentroid();

    speedLimit = speedOfLight / 1.3;

    addShip_to_Visual2D(this, v);
    physX->addObject(this);
}


Elice::~Elice(void)
{
    *score += 700;
}

void Elice::computeCentroid()
{
    centroid = Point2D(0, 0);
}

void Elice::accelerate()
{
    Ship::accelerate();
    
    Transform2D::loadIdentityMatrix();
    Transform2D::translateMatrix(-centroid.x, -centroid.y);
    Transform2D::rotateMatrix(daa);
    Transform2D::translateMatrix(centroid.x, centroid.y);
    Transform2D::applyTransform_o(this);
}