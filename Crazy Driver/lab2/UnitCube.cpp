#include "UnitCube.h"
#include "Framework/Transform3D.h"

UnitCube::UnitCube(Color color, bool fill)
{
    n = 1;

	
	//varfurile de jos
	vertices.push_back(new Point3D(0,0,0));
	vertices.push_back(new Point3D(n,0,0));
	vertices.push_back(new Point3D(n,0,n));
	vertices.push_back(new Point3D(0,0,n));

    transf_vertices.push_back(new Point3D(0,0,0));
    transf_vertices.push_back(new Point3D(n,0,0));
    transf_vertices.push_back(new Point3D(n,0,n));
    transf_vertices.push_back(new Point3D(0,0,n));

	//varfurile de sus
	vertices.push_back(new Point3D(0,n,0));
	vertices.push_back(new Point3D(n,n,0));
	vertices.push_back(new Point3D(n,n,n));
	vertices.push_back(new Point3D(0,n,n));

    transf_vertices.push_back(new Point3D(0,n,0));
    transf_vertices.push_back(new Point3D(n,n,0));
    transf_vertices.push_back(new Point3D(n,n,n));
    transf_vertices.push_back(new Point3D(0,n,n));

	
	//cele 6 fete
	vector <int> contour;
	//fata jos
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));
	//fata sus
	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));
	//fata fata
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));
	//fata dreapta
	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));
	//fata spate
	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));
	//fata stanga
	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	this -> color = color;
	this -> fill = fill;
}


UnitCube::~UnitCube(void)
{
}


float UnitCube::n = 1;