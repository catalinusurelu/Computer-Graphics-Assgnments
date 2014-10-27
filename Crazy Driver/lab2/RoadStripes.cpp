#include "RoadStripes.h"
#include "UnitCube.h"
#include "Framework\Transform3D.h"
#include "Framework\DrawingWindow.h"
#include "Colors.h"


// nr = numarul de stripe-uri vizibile la un moment dat pe o "linie" despartitoare
RoadStripes::RoadStripes(int nr, float road_width, float road_length, float road_height, 
                         Visual2D* v2d, PhysicsEngine* physX, Color color, bool fill)
    : Movable(v2d, physX, fill)
{
    UnitCube* stripe;

    type = "stripes";

    speed = 0;
    rotation = 0;
    this->color = color;
    this->fill = fill;

    float n = UnitCube::n;

    stripe_width = road_width / 25;
    stripe_length = road_length / nr;
    stripe_height = road_height;
    interStripeDistance = stripe_length / 2;

    this->road_height = road_height;
    this->road_length = road_length;
    this->road_width = road_width;

    // face mai multe decat trebuie, dar e mai usor asa, se autocorecteaza in physics engine
    for(int i = 0; i < nr; i++)
    {
        stripe = new UnitCube(color, fill);
        Transform3D::loadIdentityModelMatrix();
        Transform3D::loadIdentityProjectionMatrix();
	    Transform3D::translateMatrix(-n/2,-n/2,-n/2);
	    Transform3D::scaleMatrix(stripe_width, stripe_height, stripe_length);
	    Transform3D::translateMatrix(-(road_width / 2 - road_width / 3), -stripe_height/2, 
                                        -road_length / 2 + stripe_length / 2 + i * stripe_length + (i == 0 ? 0 : i) * interStripeDistance);
        Transform3D::applyTransform(stripe);
        parts.push_back(stripe);


        stripe = new UnitCube(color, fill);
        Transform3D::loadIdentityModelMatrix();
        Transform3D::loadIdentityProjectionMatrix();
	    Transform3D::translateMatrix(-n/2,-n/2,-n/2);
	    Transform3D::scaleMatrix(stripe_width, stripe_height, stripe_length);
	    Transform3D::translateMatrix(road_width / 2 - road_width / 3, -stripe_height/2,
                                        -road_length / 2 + stripe_length / 2 + i * stripe_length + (i == 0 ? 0 : i) * interStripeDistance);
        Transform3D::applyTransform(stripe);
        parts.push_back(stripe);
    }

    addMovable_to_Visual2D(this, v2d);
    physX->addObject(this);
}


RoadStripes::~RoadStripes(void)
{
}

void RoadStripes::addFinishLine()
{
    UnitCube* stripe = new UnitCube(color, fill);
    float n = UnitCube::n;

    Transform3D::loadIdentityModelMatrix();
    Transform3D::loadIdentityProjectionMatrix();
	Transform3D::translateMatrix(-n/2,-n/2,-n/2);
	Transform3D::scaleMatrix(road_width, road_height, stripe_length / 3);
    Transform3D::translateMatrix(0, 0, -road_length / 2);
    Transform3D::applyTransform(stripe);
    DrawingWindow::addObject3D_to_Visual2D(stripe, v2d);
    parts.push_front(stripe);
}

void RoadStripes::updateStripes()
{
    // stergem ce nu se mai vede in josul ecranului
    // while - pentru ca la inceput am pus mai multe decat trebuie
    while(boundingBox.upper > road_length / 2.1 + (stripe_length + interStripeDistance))
    {
        DrawingWindow::removeObject3D_from_Visual2D(parts.back(), v2d);
        //delete parts.back();
        parts.pop_back();

        DrawingWindow::removeObject3D_from_Visual2D(parts.back(), v2d);
        //delete parts.back();
        parts.pop_back();

        updateBoundingBox();
    }

    // adaugam stripe nou
    // bounding box, include si stripe-urile, deci cand ajunge in josul strazii, adauga un stripe nou sus
    while(boundingBox.lower > -road_length / 2 + (stripe_length + interStripeDistance))
    {
        float n = UnitCube::n;

        UnitCube* stripe = new UnitCube(color, fill);
        Transform3D::loadIdentityModelMatrix();
        Transform3D::loadIdentityProjectionMatrix();
	    Transform3D::translateMatrix(-n/2,-n/2,-n/2);
	    Transform3D::scaleMatrix(stripe_width, stripe_height, stripe_length);
	    Transform3D::translateMatrix(-(road_width / 2 - road_width / 3), -stripe_height/2, 
                                        -road_length / 2 + stripe_length / 2);
        Transform3D::applyTransform(stripe);
        DrawingWindow::addObject3D_to_Visual2D(stripe, v2d);
        parts.push_front(stripe);


        stripe = new UnitCube(color, fill);
        Transform3D::loadIdentityModelMatrix();
        Transform3D::loadIdentityProjectionMatrix();
	    Transform3D::translateMatrix(-n/2,-n/2,-n/2);
	    Transform3D::scaleMatrix(stripe_width, stripe_height, stripe_length);
	    Transform3D::translateMatrix((road_width / 2 - road_width / 3), -stripe_height/2, 
                                        -road_length / 2 + stripe_length / 2);
        Transform3D::applyTransform(stripe);
        DrawingWindow::addObject3D_to_Visual2D(stripe, v2d);
        parts.push_front(stripe);

        updateBoundingBox();
    }
}
