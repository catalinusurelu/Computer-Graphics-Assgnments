#include "Torpedo.h"
#include "Framework/Transform2D.h"
#include "Framework/Circle2D.h"
#include "PhysicsProperties.h"

using namespace PhysicsProperties;

Torpedo::Torpedo(float shield_radius, Direction dir, Point2D center, Color color, 
                 Speed speed, PhysicsEngine* physX, Visual2D *v2d)
{
    type = "torpedo";

    this->speed = speed;
    ttl = 60;
    this->v2d = v2d;
    this->physX = physX;
    this->direction = dir;

    // generam torpedo in colt - (0, 0)
    parts.push_back(new Circle2D(Point2D(0, 0), shield_radius / 3, color, true));

    // si il punem la baza navei (unde incepe si drill-ul) si ii dam drumul (are viteza data mai sus)
    Point2D normalToBaseOfDrill(Point2D(dir.x * shield_radius * 1.3, dir.y * shield_radius * 1.3));
    Transform2D::loadIdentityMatrix();
    Transform2D::translateMatrix(center.x + normalToBaseOfDrill.x, center.y + normalToBaseOfDrill.y);
    Transform2D::applyTransform_o(parts.back());

    DrawingWindow::addObject2D_to_Visual2D(dynamic_cast<Circle2D*>(parts.back()), v2d);
    physX->addObject(this);
}

// returneaza true daca obiectul inca mai exista dupa update
bool Torpedo::update()
{
    ttl--;
    return ttl != 0;
}

void Torpedo::rotate_left()
{
}

void Torpedo::rotate_right()
{
}

void Torpedo::accelerate()
{
}

void Torpedo::brake()
{
}

Torpedo::Torpedo(void)
{
}

Torpedo::~Torpedo(void)
{
    DrawingWindow::removeObject2D_from_Visual2D(this->parts.back(), v2d); 
    physX->removeObject(this);
    delete this->parts.back();
}
