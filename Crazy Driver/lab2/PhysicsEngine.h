#pragma once
#include <vector>

class Player;
class Movable;
class Road;
typedef std::vector<Movable*> Objects;

class PhysicsEngine
{
public:
    PhysicsEngine(void);
    virtual ~PhysicsEngine(void);

    void update();
    void addObject(Movable* movable);
    void addPlayer(Player* player);
    void addRoad(Road* road);
    void removeObject(Movable* movable);

private:
    void applyDrag();
    void updateObjectsLocation();
    void collide(Movable* a, Movable* b);
    bool collides(Movable *a, Movable *b);
    void resolveCollisions();

    Objects objects;
    Player* player;
    Road* road;

    friend class DrawingWindow;
    friend class Transform3D;
    friend class Player;
    friend void applyProjection();
};