#pragma once

#include "Framework\Object3D.h"

class UnitCube: public Object3D
{
public:
    UnitCube(void);
    UnitCube(Color color, bool fill);
    virtual ~UnitCube(void);

    static float n;

   
};



