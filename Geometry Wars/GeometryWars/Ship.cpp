#include "Ship.h"
#include "Framework/DrawingWindow.h"
#include "Framework\Transform2D.h"
#include "PhysicsProperties.h"

#include <algorithm>

using namespace PhysicsProperties;

Ship::Ship(PhysicsEngine* physX, Visual2D *v2d, int* scor)
    :score(scor)
{
    direction = Direction(1, 0);
    speed = Speed(0, 0);
    omega = 0;

    this->v2d = v2d;
    this->physX = physX;
}

Ship::Ship(PhysicsEngine* physX, Visual2D *v2d)
{
    direction = Direction(1, 0);
    speed = Speed(0, 0);
    omega = 0;

    this->v2d = v2d;
    this->physX = physX;
}

Ship::Ship(void)
{
}

Ship::~Ship()
{
    removeShip_from_Visual2D(this, v2d);
    physX->removeObject(this);
    std::for_each(parts.begin(), parts.end(), [] (Object2D* x) {delete x;});
}

void Ship::computeCentroid()
{
    int nrX = 0;
    int nrY = 0;
    float x = 0;
    float y = 0;

    Components::iterator itParts;
    vector <Point2D*>::iterator itVertices;
    for(itParts = parts.begin(); itParts != parts.end(); itParts++)
        for(itVertices = (*itParts)->transf_points.begin(); itVertices != (*itParts)->transf_points.end(); itVertices++)
        {
            x += (**itVertices).x;
            nrX++;

            y += (**itVertices).y;
            nrY++;
        }

    centroid = Point2D(x / nrX, y / nrY);
}

void Ship::rotate_left()
{
    if(abs(omega) > speedLimit / 2)
        return;
    omega += daa;
}

void Ship::rotate_right()
{
    if(abs(omega) > speedLimit / 2)
        return;
    omega -= daa;
}

void Ship::accelerate()
{
   if(norm(speed) > speedLimit)
        return;
    speed += direction * da;
}

void Ship::brake()
{
    if(norm(speed) > sqrt(da)) // am aproximat putin vecinatatea lui 0 (ca latura unui triunghi dreptunghic)
    {
        speed -= direction * da / 4;
    }
    else
    {
        speed = Speed(0, 0);
    }
}

float produsCartezian(const Point2D& a, const Point2D& b, const Point2D& c)
{
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

bool seIntersecteaza(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d)
{
    if(produsCartezian(a, b, c) * produsCartezian(a, b, d) > 0 ||
       produsCartezian(c, d, a) * produsCartezian(c, d, b) > 0) 
    {
        return false;
    }
    return true;
}

// produs_cartezian = "cat" trebuie sa rotim o dreapta pentru a o suprapune
// peste o a doua. Astfel putm spune cat trebuie sa rotim directia inamicului
// pentru a se indrepta spre centroidul jucatorului (dreapta (0, 0) - centroid)
void Ship::updateAI(Ship* target, bool drill)
{
    // facem produs cartezian din centrodiul curent
    Direction direction = this->direction;
    Transform2D::loadIdentityMatrix();
    Transform2D::translateMatrix(centroid.x, centroid.y);
    Transform2D::applyTransform(&direction, &direction);

    float produs = produsCartezian(centroid, direction, target->centroid);

    if(produs > -45 && produs < 45 && drill)
    {
        rotate_right(); //sau orice directie
        return;
    }
    else if(produs > -20 && produs < 20)
    {
        //accelerate();
    }
    else if(produs < -0.1)
    {
        rotate_right();
    }
    else if(produs > 0.1)
    {
        rotate_left();
    }
    accelerate();
}

void Ship::move(float x, float y)
{
    // just to make sure
    computeCentroid();

    Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(-centroid.x, -centroid.y);
    std::for_each(parts.begin(), parts.end(), [] (Object2D* x) {Transform2D::applyTransform_o(x);});


    Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(x, y);
    std::for_each(parts.begin(), parts.end(), [] (Object2D* x) {Transform2D::applyTransform_o(x);});
    Transform2D::applyTransform(&centroid, &centroid);
}

void Ship::scale(float x, float y)
{
    computeCentroid();

    Transform2D::loadIdentityMatrix();
    Transform2D::translateMatrix(-centroid.x, -centroid.y);
	Transform2D::scaleMatrix(x, y);
    Transform2D::translateMatrix(centroid.x, centroid.y);
    std::for_each(parts.begin(), parts.end(), [] (Object2D* x) {Transform2D::applyTransform_o(x);});
}

void addShip_to_Visual2D(Ship *ship, Visual2D *v)
{
    Components::iterator itParts;
    for(itParts = ship->parts.begin(); itParts != ship->parts.end(); itParts++)
    {
        Ship* ship1 = dynamic_cast<Ship*>(*itParts);
        if(ship1 != NULL)
        {
            addShip_to_Visual2D(ship1, v);
            continue;
        }

        DrawingWindow::addObject2D_to_Visual2D(*itParts, v);
    }
}

void removeShip_from_Visual2D(Ship *ship, Visual2D *v)
{
    Components::iterator itParts;
    for(itParts = ship->parts.begin(); itParts != ship->parts.end(); itParts++)
    {
        Ship* ship1 = dynamic_cast<Ship*>(*itParts);
        if(ship1 != NULL)
        {
            removeShip_from_Visual2D(ship1, v);
            continue;
        }

        DrawingWindow::removeObject2D_from_Visual2D(*itParts, v);
    }
}