#include "Car.h"
#include "UnitCube.h"
#include "Wheel.h"
#include "Framework\Transform3D.h"


Car::Car(void)
{
}

Car::Car(Visual2D* v2d, PhysicsEngine* physX, Color colorUpperBody, Color colorLowerBody, 
        Color colorWheels, bool fillUpperBody, bool fillLowerBody, bool fillWheels)
    : Movable(v2d, physX, fill)
{
    type = "car";
    UnitCube *upperBody, *lowerBody;

    float n = UnitCube::n;
    length = 5;
    width = 3;
    height = 1.5;

    speed = 0;
    rotation = 0;

    // raza rotii - am scris asa ca sa fie cat lower body
    Wheel::nz = height / 2;

    // partea de jos a masinii
    lowerBody = new UnitCube(colorLowerBody, fillLowerBody);
    Transform3D::loadIdentityModelMatrix();
    Transform3D::loadIdentityProjectionMatrix();
    Transform3D::translateMatrix(-n/2, -n/2,-n/2);
	Transform3D::scaleMatrix(width, height, length);
    Transform3D::applyTransform(lowerBody);

    parts.push_back(lowerBody);

    // centrul masinii (in plan 2D ne intereseaza mai mult)
    center = Point3D(0, 0, 0);

    // partea de sus a masinii
    upperBody = new UnitCube(colorUpperBody, fillUpperBody);
    Transform3D::loadIdentityModelMatrix();
    Transform3D::loadIdentityProjectionMatrix();
    Transform3D::translateMatrix(-n/2,-n/2,-n/2);
	Transform3D::scaleMatrix(width * 0.8, height * 0.7, length * 0.8);
	Transform3D::translateMatrix(0, height / 2 + height * 0.7 / 2 , 0);
    Transform3D::applyTransform(upperBody);

    parts.push_back(upperBody);
    
    Wheel *wheel1, *wheel2, *wheel3, *wheel4;

    wheel1= new Wheel(colorWheels, fillWheels);
    Transform3D::loadIdentityMatrix();
    Transform3D::translateMatrix(-width / 2, 0,  -length / 2);
    Transform3D::applyTransform(wheel1);

    parts.push_back(wheel1);
    
    wheel2 = new Wheel(colorWheels, fillWheels);
    Transform3D::loadIdentityModelMatrix();
    Transform3D::translateMatrix(width / 2, 0,  -length / 2);
    Transform3D::applyTransform(wheel2);

    parts.push_back(wheel2);


    wheel3= new Wheel(colorWheels, fillWheels);
    Transform3D::loadIdentityModelMatrix();
    Transform3D::translateMatrix(-width / 2, 0,  length / 2);
    Transform3D::applyTransform(wheel3);

    parts.push_back(wheel3);

    wheel4= new Wheel(colorWheels, fillWheels);
    Transform3D::loadIdentityModelMatrix();
    Transform3D::translateMatrix(width / 2, 0,  length / 2);
    Transform3D::applyTransform(wheel4);

    parts.push_back(wheel4);
}


Car::~Car(void)
{
}
