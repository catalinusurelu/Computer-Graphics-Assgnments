#pragma once

#include "UnitCube.h"
#include "Framework\Visual2D.h"

class Road: public UnitCube
{
public:
    Road(float width, float length, float height, Color color, bool fill, Visual2D* v2d);
    ~Road(void);

    static float width;
    static float length;
    static float height;

private:
    Visual2D* v2d;
};

