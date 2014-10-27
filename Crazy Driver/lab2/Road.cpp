#include "Road.h"
#include "Framework\Transform3D.h"
#include "Framework\Visual2D.h"
#include "Framework\DrawingWindow.h"



Road::Road(float width, float length, float height, Color color, bool fill, Visual2D* v2d)
    : UnitCube(color, fill)
{
    this->width = width;
    this->length = length;
    this->height = height;

    Transform3D::loadIdentityModelMatrix();
	Transform3D::translateMatrix(-n/2,-n/2,-n/2);
	Transform3D::scaleMatrix(width, height, length);
    Transform3D::translateMatrix(0, -height/2, 0);
    Transform3D::applyTransform(this);
	

    // Am experimentat pentru a obtine valorile cat mai bune
    Transform3D::loadIdentityModelMatrix();
	Transform3D::loadIdentityProjectionMatrix();
	Transform3D::perspectiveProjectionMatrix(0.7, 20, 175);
	Transform3D::applyTransformProjection(this);
    Transform3D::loadIdentityProjectionMatrix(); // mai departe lucram doar cu matricea model
    Transform3D::loadIdentityModelMatrix();

    this->color = color;
    this->fill = fill;

    DrawingWindow::addObject3D_to_Visual2D(this,v2d);
}


Road::~Road(void)
{
    DrawingWindow::removeObject3D_from_Visual2D(this, v2d);
}

// Initial e ca UnitCube; nu ne intereseaza; e doar de initializare
float Road::width = 1;
float Road::length = 1;
float Road::height = 1;
