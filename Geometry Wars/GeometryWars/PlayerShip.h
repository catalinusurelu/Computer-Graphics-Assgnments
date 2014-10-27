#pragma once
#include "Ship.h"
#include "Drill.h"

class PlayerShip: public Ship
{
public:
	PlayerShip(void);
	PlayerShip(float shield_radius, Color color, Color drillColor,  Color torpedoColor, PhysicsEngine* physX, Visual2D *v);
	virtual ~PlayerShip(void);

    void computeCentroid();
    void activateDrill();
    void deactivateDrill();
    void fire();

    static int lives;

protected:
    Drill* drill;
    friend PhysicsEngine;

private:
    float shield_radius;
    Color drillColor;
    Color torpedoColor;
};

