#include "Obstacle.h"
#include "framework/Transform3D.h"

// Obstacol - con
Obstacle::Obstacle(Visual2D* v2d, PhysicsEngine* physX, Color color, bool fill)
    :Movable(v2d, physX, fill)
{
    type = "obstacle";

    speed = 0;
    rotation = 0;

    vector <Point3D*> vertices;
	vector <Face*> faces;

    // baza
	vertices.push_back(new Point3D(0,0,0));
	vertices.push_back(new Point3D(n,0,0));
	vertices.push_back(new Point3D(n,0,n));
	vertices.push_back(new Point3D(0,0,n));

    // varf
    vertices.push_back(new Point3D(n / 2, 4 * n, n / 2));

    vector <int> contour;
	//fata jos
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
    faces.push_back(new Face(contour));

    // fete laterale
    contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(4);
    faces.push_back(new Face(contour));

    contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
    faces.push_back(new Face(contour));

    contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
    faces.push_back(new Face(contour));

    contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(4);
    faces.push_back(new Face(contour));

    Object3D* piramida = new Object3D(vertices, faces, color, fill);
    Transform3D::loadIdentityMatrix();
    Transform3D::translateMatrix(-n/2,-n/2,-n/2);
    Transform3D::rotateMatrixOy(45);
    Transform3D::applyTransform(piramida);

    center = Point3D(0, 0, 0);

    parts.push_back(piramida);

    addMovable_to_Visual2D(this, v2d);
    physX->addObject(this);
}


Obstacle::~Obstacle(void)
{
}

void Obstacle::rotate_left() {};
void Obstacle::rotate_right() {};
void Obstacle::accelerate() {};
void Obstacle::brake() {};


float Obstacle::n = 2;