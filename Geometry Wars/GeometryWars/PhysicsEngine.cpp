#include "PhysicsEngine.h"
#include "Framework/Transform2d.h"
#include "Ship.h"
#include "Torpedo.h"
#include "Wall.h"
#include "PhysicsProperties.h"
#include "Enemy.h"
#include "PlayerShip.h"

#include <list>

using namespace PhysicsProperties;

PhysicsEngine::PhysicsEngine(void)
{
}

PhysicsEngine::~PhysicsEngine(void)
{
}

void PhysicsEngine::update()
{
    applyDrag();
    resolveCollisions();
    updateObjectsLocation();
}

void PhysicsEngine::addObject(Movable* movable)
{
    objects.push_back(movable);
}

void PhysicsEngine::removeObject(Movable* movable)
{
    Objects::iterator it;
    for(it = objects.begin(); it != objects.end(); it++)
    {
        if(dynamic_cast<Movable*>(*it) == movable) /// putem compara doar pointeri catre acelasi tip
        {
            objects.erase(it);
            return;
        }
    }
}

void PhysicsEngine::applyDrag()
{
    Objects::iterator it;
    for(it = objects.begin(); it != objects.end(); it++)
    {
         if(dynamic_cast<Torpedo*>(*it) != NULL)
         {
            continue;
         }

        (*it)->speed *= speedDragConstant;
        (*it)->omega = (*it)->omega * angularDragConstant;

    }
}

void PhysicsEngine::updateObjectsLocation()
{

    Objects::iterator it;
    for(it = objects.begin(); it != objects.end(); it++)
    {
        // peretele sta pe loc
         if((*it)->type == "wall")
         {
             continue;
         }

         if((*it)->type == "torpedo")
         {
             // Verificam daca ttl > 0
             Torpedo* torpedo = dynamic_cast<Torpedo*>(*it);
             if(!torpedo->update())
             {
                 it = objects.erase(it);
                 delete torpedo;
                 if(it == objects.end() && objects.size() != 0)
                 {
                     it = objects.begin();
                     continue;
                 }
                 else
                 {
                     return;
                 }
             }
         }

        Transform2D::loadIdentityMatrix();
        Transform2D::translateMatrix((*it) -> speed.x, (*it) -> speed.y);

        Transform2D::applyTransform_o(*it);
        Transform2D::applyTransform(&(*it)->centroid, &(*it)->centroid);

        Transform2D::loadIdentityMatrix();
        Transform2D::translateMatrix(-(*it)->centroid.x, -(*it)->centroid.y);
        Transform2D::rotateMatrix((*it) -> omega);
        Transform2D::translateMatrix((*it)->centroid.x, (*it)->centroid.y);

        Transform2D::applyTransform_o(*it);

        Transform2D::loadIdentityMatrix();
        Transform2D::rotateMatrix((*it) -> omega);
        Transform2D::applyTransform(&(*it)->direction, &(*it)->direction);
    }
}

void PhysicsEngine::resolveCollisions()
{
    int i, j;
    for(i = 0; i < objects.size(); ++i)
        for(j = i + 1; j < objects.size(); ++j)
        {
           if(collides(objects[i], objects[j]))
            {
                collide(objects[i], objects[j]);
            }
        }
}

void PhysicsEngine::collide(Movable* a, Movable* b)
{
    if(a->type == "wall")
    {
        b->speed = -b->speed;
        return;
    }
    else if (b->type == "wall")
    {
        a->speed = -a->speed;
        return;
    }

    if(a->type == "torpedo" && b->type == "enemy" ||
        a->type == "enemy" && b->type == "torpedo")
    {
       delete a;
       delete b;
       return;
    }

    if(a->type == "enemy" && b->type == "enemy")
    {
        Speed aux;
        aux = b->speed;
        b->speed = a->speed;
        a->speed = aux;
        return;
    }

    // Acum tratam special coliziunea burghiu inamic
    PlayerShip* player;
    Ship* enemy;

    if(a->type == "player" && b->type == "enemy" && Drill::active)
    {
        player = static_cast<PlayerShip*>(a);
        enemy = static_cast<PlayerShip*>(b);
        
        // a atins nava, nu burghiul
        if(!collideDrillEnemy(player, enemy))
        {
            Speed aux;
            aux = enemy->speed;
            enemy->speed = player->speed;
            player->speed = aux;

            enemy->speed = enemy->speed * 1.2;
            player->speed = player->speed * 1.2;

            PlayerShip::lives--;
        }            
    }
    else if(b->type == "player" && a->type == "enemy" && Drill::active)
    {
        player = static_cast<PlayerShip*>(b);
        enemy = static_cast<PlayerShip*>(a);
        
        // a atins nava, nu burghiul
        if(!collideDrillEnemy(player, enemy))
        {
            Speed aux;
            aux = player->speed;
            player->speed = enemy->speed;
            enemy->speed = aux;

            enemy->speed = enemy->speed * 1.2;
            player->speed = player->speed * 1.2;

            PlayerShip::lives--;
        }
        
    }
    else 
    {
        Speed aux;
        aux = b->speed;
        b->speed = a->speed;
        a->speed = aux;

        a->speed = a->speed * 1.2;
        b->speed = b->speed * 1.2;

        PlayerShip::lives--;
    }

    // In caz ca se ciocneste de prea multe ori aiurea
     PlayerShip::lives = std::max( PlayerShip::lives, 0);
}

bool PhysicsEngine::collides(Movable *a, Movable *b)
{

    if(a->type == "torpedo" && b->type == "player" ||
       a->type == "player" && b->type == "torpedo")
    {
        return false;
    }

    // fast reject
    if(a->type == "enemy" && b->type == "enemy")
    {
        if((((Ship*)a)->radius + ((Ship*)b)->radius) * 1.1 < norm(((Ship*)a)->centroid, ((Ship*)b)->centroid))
            return false;
    }

    int i, j;
    Components::iterator it1, it2;
    for(it1 = a->parts.begin(); it1 !=  a->parts.end(); it1++)
        for(it2 = b->parts.begin(); it2 !=  b->parts.end(); it2++)
        {
            vector<Point2D*> &pa = (*it1)->transf_points;  //aliasuri
            vector<Point2D*> &pb = (*it2)->transf_points;
            for(i = 0; (i + 1) <= pa.size(); ++i)
                for(j = 0; (j + 1) <= pb.size(); ++j)
                // facem % ca sa verificam si ultimul si primul punct
                if(seIntersecteaza(*pa[i], *pa[(i + 1) % pa.size()], *pb[j], *pb[(j + 1) % pb.size()]))
                {
                    return true;
                }
        }
    return false;
}

// E cam copy paste, dar nu am avut alta alternativa pentru
// cazul asta special (drill nu e movable ca Torpedo)
bool PhysicsEngine::collideDrillEnemy(PlayerShip* a, Ship* b)
{
    int i, j;
    Components::iterator it1;
    for(it1 = b->parts.begin(); it1 !=  b->parts.end(); it1++)
    {
        vector<Point2D*> &pa = (*it1)->transf_points;
        vector<Point2D*> &pb = a->drill->transf_points;
        for(i = 0; (i + 1) <= pa.size(); ++i)
            for(j = 0; (j + 1) <= pb.size(); ++j)
                if(seIntersecteaza(*pa[i], *pa[(i + 1) % pa.size()], *pb[j], *pb[(j + 1) % pb.size()]))
                {
                    delete b;
                    return true;
                }
    }
    return false;
}