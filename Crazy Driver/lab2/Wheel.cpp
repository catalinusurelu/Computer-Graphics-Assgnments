#include "Wheel.h"
#include "framework/Transform3D.h"


Wheel::Wheel(void)
{
}

// Wheel e pozitional fix cu centrul in (0, 0, 0)
Wheel::Wheel(Color color, bool fill)
{
    // granularity = cat de multe punct sa aibe poligonul care simuleaza cerc-ul rotii
    int granularity = 10;

    //
    for(int i = 0; i < granularity; i++)
    {
        Point3D* point = new Point3D(-nx / 2, 0, nz);

        Transform3D::loadIdentityMatrix();
        Transform3D::rotateMatrixOx(i * 360. / granularity);
        Transform3D::computeMVP();  // calculam MVP, ca altfel applyTransform nu are matrice MVP calculatara 
        Transform3D::applyTransform(point, point);

        vertices.push_back(point);
        transf_vertices.push_back(new Point3D(point->x, point->y, point->z)); // altfel, daca era tot point, le modifica pe 
                                                                              // amandoua deodata in aplicarea transformarii perspecrtiva

        point = new Point3D(+nx / 2, 0, nz);

        Transform3D::applyTransform(point, point);

        vertices.push_back(point);
        transf_vertices.push_back(new Point3D(point->x, point->y, point->z));
    }

    // fac fete pe roata (exterior)
    vector <int> contour;
    for(int i = 0; i < vertices.size(); i += 2)
    {
        contour.clear();
        contour.push_back(i);
        contour.push_back(i + 1);
        contour.push_back((i + 3) % vertices.size());
        contour.push_back((i + 2) % vertices.size());
        faces.push_back(new Face(contour));
    }

    // centru roata, stanga
    Point3D* point = new Point3D(0, -nx / 2, 0);
    vertices.push_back(point);
    transf_vertices.push_back(new Point3D(point->x, point->y, point->z));

    // centru roata, dreapta
    point = new Point3D(0, +nx / 2, 0);
    vertices.push_back(point);
    transf_vertices.push_back(new Point3D(point->x, point->y, point->z));

    // fac fete roata - gen spitze
    for(int i = 0; i < vertices.size(); i += 2)
    {
        contour.clear();
        contour.push_back(i);
        contour.push_back((i + 2) % vertices.size());
        contour.push_back(vertices.size() - 1);
        faces.push_back(new Face(contour));
    }

    for(int i = 1; i < vertices.size(); i += 2)
    {
        contour.clear();
        contour.push_back(i);
        contour.push_back((i + 2) % vertices.size());
        contour.push_back(vertices.size() - 2);
        faces.push_back(new Face(contour));
    }

    this -> color = color;
	this -> fill = fill;

}

void Wheel::updateCenter()
{
    Point3D* centruStanga = vertices[vertices.size() - 2 ];
    Point3D* centruDreapta = vertices[vertices.size() - 1 ];
    center = Point3D((centruStanga->x + centruDreapta->x) / 2,
                     (centruStanga->y + centruDreapta->y) / 2,
                     (centruStanga->z + centruDreapta->z) / 2);
}

Wheel::~Wheel(void)
{
}

float Wheel::nz = 1;
float Wheel::nx = 0.5;