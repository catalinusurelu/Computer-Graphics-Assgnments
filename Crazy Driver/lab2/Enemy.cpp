#include "Enemy.h"
#include "PhysicsProperties.h"

using namespace PhysicsProperties;


Enemy::Enemy(Visual2D* v2d, PhysicsEngine* physX, Color colorUpperBody, Color colorLowerBody, 
             Color colorWheels, bool fillUpperBody, bool fillLowerBody, bool fillWheels)
    : Car(v2d, physX, colorUpperBody, colorLowerBody, colorWheels, fillUpperBody, fillLowerBody, fillWheels)
{
    type = "enemy";
    speed = - 10 * da; // speed constant
    rotation = 0;

    addMovable_to_Visual2D(this, v2d);
    physX->addObject(this);
}


Enemy::~Enemy(void)
{
}
