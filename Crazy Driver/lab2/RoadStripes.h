#pragma once
#include "Movable.h"

class RoadStripes : public Movable
{
public:
    RoadStripes(void) {};
    RoadStripes(int nr, float road_width, float road_length, float road_height, Visual2D* v2d, PhysicsEngine* physX, Color color, bool fill);
    virtual ~RoadStripes(void);

    void updateStripes();
    void addFinishLine();

    void rotate_left() {};
    void rotate_right() {};
    void accelerate() {};
    void brake() {};

private:
    float stripe_width;
    float stripe_length;
    float stripe_height;
    float interStripeDistance;

    float road_width;
    float road_length;
    float road_height;

    Color color;
};

