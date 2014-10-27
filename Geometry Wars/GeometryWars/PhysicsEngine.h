#pragma once
#include <vector>

class Movable;
class PlayerShip;
class Ship;
typedef std::vector<Movable*> Objects;

class PhysicsEngine
{
public:
    PhysicsEngine(void);
    virtual ~PhysicsEngine(void);

    void update();
    void addObject(Movable* movable);
    void removeObject(Movable* movable);

private:
    void applyDrag();
    void updateObjectsLocation();
    void collide(Movable* a, Movable* b);
    bool collides(Movable *a, Movable *b);
    bool collideDrillEnemy(PlayerShip* a, Ship* b);
    void resolveCollisions();

    Objects objects;

    friend class Movable;
    friend class Ship;
    friend class DrawingWindow;
    friend void updateEnemies();
    friend void generateSwarm();
};