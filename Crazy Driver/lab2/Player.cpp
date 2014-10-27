#include "Player.h"
#include "PhysicsProperties.h"

using namespace PhysicsProperties;


Player::Player(Visual2D* v2d, PhysicsEngine* physX, Color colorUpperBody, Color colorLowerBody, 
               Color colorWheels, bool fillUpperBody, bool fillLowerBody, bool fillWheels, int lives, int benzina)
    : Car(v2d, physX, colorUpperBody, colorLowerBody, colorWheels, fillUpperBody, fillLowerBody, fillWheels)
{
    type = "player";
    speed = 0;
    rotation = 0;
    score = 0;
    distantaParcursa = 0;
    totalRotation = 0;

    this->benzina = benzina;

    addMovable_to_Visual2D(this, v2d);
    physX->addObject(this);
}


Player::~Player(void)
{
}


void Player::rotate_left()
{
    if(totalRotation >= 45)
    {
        totalRotation = 45;
        return;
    }

    if(abs(rotation) > speedLimit / 2)
        return;
    rotation += daa;
}

void Player::rotate_right()
{
    if(totalRotation <= -45)
    {
        totalRotation = -45;
        return;
    }

    if(abs(rotation) > speedLimit / 2)
        return;
    rotation -= daa;
}

void Player::accelerate()
{
   if(speed > speedLimit)
        return;
    speed += da;
    benzina -= 5;
}

void Player::brake()
{
    if(speed > da / 4)
    {
        speed -= da / 4;
    }
    else
    {
        speed = 0;
    }

    benzina -= 1;
}

void Player::redresare()
{
    //
}

int Player::lives = 0;