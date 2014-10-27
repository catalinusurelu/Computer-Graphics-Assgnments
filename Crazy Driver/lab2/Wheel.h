#pragma once
#include "framework\object3D.h"

class Wheel :
    public Object3D
{
public:
    Wheel(void);
    Wheel(Color color, bool fill);

    virtual ~Wheel(void);

    void updateCenter();

    static float nx;
    static float nz;

    Point3D center;
};

