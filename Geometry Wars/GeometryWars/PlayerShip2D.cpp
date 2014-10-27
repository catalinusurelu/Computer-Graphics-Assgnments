#include "PlayerShip2D.h"
#include "Framework/Point2D.h"
#include "Framework/Line2D.h"
#include "Framework/Transform2D.h"
#include <math.h>

#define PI 3.14159265358979323846
#define cos(x) ((float)cos(((x) * (2 * PI)) / 360))
#define sin(x) ((float)sin(((x) * (2 * PI)) / 360))

PlayerShip2D::PlayerShip2D(float shield_radius, Color color)
	: Polygon2D(color, false)
{

	Point2D p1, p2, p3;
	float x_centroid;
	float y_centroid;
	
	addPoint(Point2D(0, 0));

	p1 = Point2D(shield_radius * cos(45), shield_radius * sin(45));
	addPoint(p1);

	p2 = Point2D(p1.x + shield_radius * (2 / (float)3) * cos(-30),
			     p1.y + shield_radius * (2 / (float)3) * sin(-30));
	addPoint(p2);

	p3 = Point2D(p2.x + shield_radius * (1.5 / (float)3) * cos(160),
				 p2.y + shield_radius * (1.5 / (float)3) * sin(160));
	addPoint(p3);

	addPoint(Point2D(shield_radius / 2.5, 0));
	addPoint(Point2D(p3.x, -p3.y));
	addPoint(Point2D(p2.x, -p2.y));
	addPoint(Point2D(p1.x, -p1.y));

	x_centroid = (0 + 2 * p1.x + 2 * p2.x + 2 * p3.x) / 7;
	y_centroid = 0; // e simetric

	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(-x_centroid, -y_centroid);
	Transform2D::applyTransform_o(this);

    centroid = Point2D(x_centroid, y_centroid);
}


PlayerShip2D::~PlayerShip2D(void)
{
}
