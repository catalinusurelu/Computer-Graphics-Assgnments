#include "PlayerShip.h"
#include "PlayerShip2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Transform2D.h"
#include "PhysicsProperties.h"
#include "Drill.h"
#include "Torpedo.h"

using namespace PhysicsProperties;

PlayerShip::PlayerShip(void)
{
}


PlayerShip::PlayerShip(float shield_radius, Color color, Color drillColor,  Color torpedoColor, PhysicsEngine* physX, Visual2D *v2d)
    :Ship(physX, v2d)
{
    type = "player";

    this->shield_radius = shield_radius;
    this->drill = NULL;
    this->drillColor = drillColor;
    this->torpedoColor = torpedoColor;

	parts.push_back(new PlayerShip2D(shield_radius, color));
	parts.push_back(new Circle2D(Point2D(0, 0), shield_radius, color, false));
    computeCentroid();

    speedLimit = speedOfLight;
    addShip_to_Visual2D(this, v2d);
    physX->addObject(this);
}


void PlayerShip::computeCentroid()
{
    centroid = Point2D(0, 0);
}

void PlayerShip::activateDrill()
{
    if(Drill::active == true)
    {
        return;
    }

    drill = new Drill(shield_radius, direction, centroid, drillColor);
    parts.push_back(drill);
    DrawingWindow::addObject2D_to_Visual2D(drill, v2d);

    speedLimit = speedOfLight / 2;
}

void PlayerShip::deactivateDrill()
{
    // are un bug glut cand apesi mai multe taste intr-o combinatie(gen fire, drill si inainte)
    // apeleaza de 2 ori onkeup
    if(parts.size() != 3)
    {
        return;
    }

    DrawingWindow::removeObject2D_from_Visual2D(drill, v2d);
    parts.pop_back();
    delete drill;
    drill = NULL;

    speedLimit = speedOfLight;
}

void PlayerShip::fire()
{
    new Torpedo(shield_radius, direction, centroid, torpedoColor, direction * speedLimit, physX, v2d);
}

PlayerShip::~PlayerShip(void)
{
}

int PlayerShip::lives = 3;