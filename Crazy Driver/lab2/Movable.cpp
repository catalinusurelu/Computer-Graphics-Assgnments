#include "Movable.h"
#include "Framework\Transform3D.h"
#include<algorithm>
#include "Framework\DrawingWindow.h"


Movable::Movable(void)
{
}

Movable::Movable(Visual2D* v2d, PhysicsEngine* physX, bool fill)
{
    this->physX = physX;
    this->v2d = v2d;
    this->fill = fill;
}

Movable::~Movable(void)
{
    removeMovable_from_Visual2D(this, v2d);
    physX->removeObject(this);
    std::for_each(parts.begin(), parts.end(), [] (Object3D* x) {delete x;});
}

void Movable::addMovable_to_Visual2D(Movable *movable, Visual2D *v)
{
    Components::iterator itParts;
    for(itParts = movable->parts.begin(); itParts != movable->parts.end(); itParts++)
    {
        DrawingWindow::addObject3D_to_Visual2D(*itParts, v);
    }
}

void Movable::removeMovable_from_Visual2D(Movable *movable, Visual2D *v)
{
    Components::iterator itParts;
    for(itParts = movable->parts.begin(); itParts != movable->parts.end(); itParts++)
    {

        DrawingWindow::removeObject3D_from_Visual2D(*itParts, v);
    }
}

void Movable::updateBoundingBox()
{

    float upper = parts[0]->vertices[0]->z;
    float lower = parts[0]->vertices[0]->z;
    float left = parts[0]->vertices[0]->x;
    float right = parts[0]->vertices[0]->x;

    for(int i = 0; i < parts.size(); i++)
    {
        for(int j = 0; j < parts[i]->vertices.size(); j++)
        {
            if(parts[i]->vertices[j]->z < lower)
            {
                lower = parts[i]->vertices[j]->z;
            }
            
            if(parts[i]->vertices[j]->z > upper)
            {
                upper = parts[i]->vertices[j]->z;
            }

            if(parts[i]->vertices[j]->x < left)
            {
                left = parts[i]->vertices[j]->x;
            }
            
            if(parts[i]->vertices[j]->x > right)
            {
                right = parts[i]->vertices[j]->x;
            }
        }
    }

    Movable::BoundingBox b = {upper, lower, left, right};
    boundingBox = b;
}

void Movable::move(float x, float y, float z)
{
    Transform3D::loadIdentityModelMatrix();
    Transform3D::loadIdentityProjectionMatrix();
    Transform3D::translateMatrix(-center.x, -center.y, -center.z);
    Transform3D::translateMatrix(x, y, z);
    std::for_each(parts.begin(), parts.end(), [] (Object3D* x) {Transform3D::applyTransform(x);});
    Transform3D::applyTransform(&center, &center);
}