#include "Wall.h"
#include "Framework/Rectangle2d.h"


Wall::Wall(Point2D colt, float width, float height, PhysicsEngine* physX)
{
    type = "wall";

    this->physX = physX;
    physX->addObject(this);
    parts.push_back(new Rectangle2D(colt, width, height));
}

Wall::~Wall()
{
    delete static_cast<Rectangle2D*>(parts.back());
}