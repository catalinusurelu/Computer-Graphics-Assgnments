#include "PowerUp.h"
#include "UnitCube.h"
#include "Colors.h"
#include "Framework\Transform3D.h"


PowerUp::PowerUp(Visual2D* v2d, PhysicsEngine* physX, bool fill)
    :Movable(v2d, physX, fill)
{
    type = "powerup";

    
    frame = 0;
    score = 1000;

    speed = 0;
    rotation = 0;

    UnitCube* cube = new UnitCube(BLUE, fill);
    float n = UnitCube::n;
    Transform3D::loadIdentityMatrix();
    Transform3D::translateMatrix(-n/2, -n/2,-n/2);
    Transform3D::scaleMatrix(2, 2, 2);
    Transform3D::applyTransform(cube);

    parts.push_back(cube);
    center = Point3D(0, 0, 0);
    
    addMovable_to_Visual2D(this, v2d);
    physX->addObject(this);
}

void PowerUp::updateColor()
{
    frame = (frame + 1) % 60;

    if(frame < 20)
    {
        parts[0]->color = BLUE;
    }
    else if(frame < 40)
    {
        parts[0]->color = GREEN;
    }
    else if(frame < 60)
    {
        parts[0]->color = RED;
    }
}

PowerUp::~PowerUp(void)
{
}
