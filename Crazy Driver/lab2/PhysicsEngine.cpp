#include "PhysicsEngine.h"
#include "Framework/Transform3d.h"
#include "PhysicsProperties.h"
#include "Player.h"
#include "Wheel.h"
#include "RoadStripes.h"
#include "PowerUp.h"
#include "Road.h"

using namespace PhysicsProperties;
#define PI 3.14159265358979323846

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

void PhysicsEngine::addRoad(Road* road)
{
    this->road = road;
}

void PhysicsEngine::addPlayer(Player* player)
{
    this->player = player;
}

// aplicam frecare -> obiectele incetinesc la oprirea acceleratiei
// aici, da doar iluzia, miscarea facandu-se relativ la viteza
// jucatorului; Asadar  aplicam drag doar jucatorului, totul
// fiind calculat in functie de viteza lui
void PhysicsEngine::applyDrag()
{
    Objects::iterator it;
    for(it = objects.begin(); it != objects.end(); it++)
    {
        if((*it)->type == "player")
        {
            (*it)->speed *= speedDragConstant;
            (*it)->rotation = (*it)->rotation * angularDragConstant;
        }
    }
}

void PhysicsEngine::updateObjectsLocation()
{
    Objects::iterator it;
    for(it = objects.begin(); it != objects.end(); it++)
    {
        (*it)->updateBoundingBox();

        // Cand iese din aria vizuala (josul strazii, sau, pentru
        // masini, partea de sus a strazii) le eliminam
        // De asemenea, scorul creste cand trecem de un obstacol
        if((*it)->boundingBox.upper > road->length / 2.1)
        {
            if((*it)->type == "obstacle")
            {
                delete *it;
                player->score += 100;
                it = objects.begin();
                continue;
            }
            else if((*it)->type == "enemy")
            {
                delete *it;;
                player->score += 200;
                it = objects.begin();
                continue;
            }
            else if((*it)->type == "powerup")
            {
                delete *it;
                it = objects.begin();
                continue;
            }
        }

        if((*it)->type == "enemy")
        {
            if((*it)->boundingBox.upper < - road->length / 2)
            {
                delete *it;
                it = objects.begin();
                continue;
            }
        }

        if((*it)->type == "player")
        {
            Player* player = static_cast<Player*>(*it);

            Transform3D::loadIdentityMatrix();
            Transform3D::translateMatrix(-player->center.x, -player->center.y, -player->center.z);
            Transform3D::rotateMatrixOy(player->rotation);

            // testam invers rotatia, ca ne miscam pe axa OZ in sens negativ
            if(player->totalRotation > 0 && + road->width / 2 > player->boundingBox.right)
            {
                // viteza laterala - la rotatie - o caclulam in functie de viteza curenta si de cat de "rotite"
                // sunt rotile - 45 grade = viteza maxima, ca in realitate
                Transform3D::translateMatrix((player->speed / 8.f) * (player->totalRotation / 45.f), 0, 0);
            }
            // Daca iesim din strada, facem o translatie in sens opus - da senzatia de impact cu marginea strazii
            else if(player->totalRotation > 0 && + road->width / 2 < player->boundingBox.right)
            {
                Transform3D::translateMatrix(-(player->speed / 8.f) * (player->totalRotation / 45.f), 0, 0);
            }

            if(player->totalRotation < 0 && - road->width / 2 < player->boundingBox.left)
            {
                Transform3D::translateMatrix((player->speed / 8.f) * (player->totalRotation / 45.f), 0, 0);
            }
            else if(player->totalRotation > 0 && - road->width / 2 > player->boundingBox.left)
            {
                Transform3D::translateMatrix(2 * (player->speed / 8.f) * (player->totalRotation / 45.f), 0, 0);
            }
            

            Transform3D::translateMatrix(player->center.x, player->center.y, player->center.z);
            Transform3D::applyTransform(player);
            Transform3D::applyTransform(&player->center, &(player)->center);

            // Updatam aici viteza si rotatie, ca daca updatam in accelerate, rotate_left etc,
            // atunci nu tinea cont de drag
            player->distantaParcursa += player->speed;
            player->totalRotation += player->rotation;

             // Rotim rotile
            for(int i = 2; i < player->parts.size(); i++)
            {
                Wheel* wheel = static_cast<Wheel*>(player->parts[i]);
                wheel->updateCenter();
                Transform3D::loadIdentityMatrix();
                Transform3D::translateMatrix(-wheel->center.x, -wheel->center.y, -wheel->center.z);
                Transform3D::rotateMatrixOy(-player->totalRotation);  // intai rotim rotile sa fie pe directia inainte, nu rotite

                // nz = raza; am aplicat regula de 3 simpla, adica, am calculat cat de mult se roteste roata cand parcurge o anumita
                // distanta
                Transform3D::rotateMatrixOx(360. * player->speed / (2 * Wheel::nz * PI)); 
                Transform3D::rotateMatrixOy(player->totalRotation);
                Transform3D::translateMatrix(wheel->center.x, wheel->center.y, wheel->center.z);
                Transform3D::applyTransform(wheel);
            }
            
            
            player->benzina -= 1; // daca sta pe loc, tot consuma

            continue;
        }

        if((*it)->type == "stripes")
        {
            RoadStripes* stripes = static_cast<RoadStripes*>(*it);
            stripes->updateStripes();
        }

        if((*it)->type == "powerup")
        {
            PowerUp* power = static_cast<PowerUp*>(*it);
            power->updateColor();
        }

        Transform3D::loadIdentityModelMatrix();
        Transform3D::loadIdentityProjectionMatrix();
        Transform3D::translateMatrix(0, 0, (*it)->speed + player->speed);

        Transform3D::applyTransform(*it);
        Transform3D::applyTransform(&(*it)->center, &(*it)->center);
    }

}

void PhysicsEngine::resolveCollisions()
{
    int i, j;
    for(i = 0; i < objects.size(); ++i)
    {
        objects[i]->updateBoundingBox();

        if(objects[i]->type == "stripes")
        {
             continue;
        }
        for(j = i + 1; j < objects.size(); ++j)
        {
            if(objects[j]->type == "stripes")
            {
                continue;
            }

            if(collides(objects[i], objects[j]))
            {
                collide(objects[i], objects[j]);
            }
        }
    }
}

void PhysicsEngine::collide(Movable* a, Movable* b)
{
    // cand ciocnim masina, reducem viteza jucatorului - da senzatia de ciocnire
    if(a->type == "player" && b->type == "enemy")
    {
        Player* player = static_cast<Player*>(a);
        player->speed /= 8;
        player->score -= 200;
        player->lives--;

        Transform3D::loadIdentityModelMatrix();
        Transform3D::loadIdentityProjectionMatrix();
        Transform3D::translateMatrix(0, 0, -a->speed / 2);
        Transform3D::applyTransform(b);


        return;
    }
         
    if(a->type == "enemy" && b->type == "player")
    {
        Player* player = static_cast<Player*>(b);
        player->speed /= 2;
        player->score -= 200;
        player->lives--;


        Transform3D::loadIdentityModelMatrix();
        Transform3D::loadIdentityProjectionMatrix();
        Transform3D::translateMatrix(0, 0, -b->speed / 2);
        Transform3D::applyTransform(a);

        return;
    }

    if(a->type == "player" && b->type == "obstacle")
    {
        Player* player = static_cast<Player*>(a);
        player->speed /= 2;
        player->score -= 100;
        player->lives--;
        delete b;
        return;
    }

    if(a->type == "obstacle" && b->type == "player")
    {
        Player* player = static_cast<Player*>(b);
        player->speed /= 2;
        player->score -= 100;
        player->lives--;
        delete a;
        return;
    }

    if(a->type == "obstacle" && b->type == "enemy")
    {
        delete a;
        return;
    }

    if(a->type == "enemy" && b->type == "obstacle")
    {
        delete b;
        return;
    }

    if(a->type == "powerup" && b->type == "player")
    {
        PowerUp* power = static_cast<PowerUp*>(a);
        Player* player = static_cast<Player*>(b);
        player->score += power->score;
        delete power;
        return;
    }

    if(b->type == "powerup" && a->type == "player")
    {
        PowerUp* power = static_cast<PowerUp*>(b);
        Player* player = static_cast<Player*>(a);
        player->score += power->score;
        delete power;
        return;
    }

}

// intersectie de 2 dreptunghiuri cu axele paralele cu XOY
bool PhysicsEngine::collides(Movable *a, Movable *b)
{
    Movable::BoundingBox b1 = a->boundingBox;
    Movable::BoundingBox b2 = b->boundingBox;

    return !(b1.right < b2.left || b1.left > b2.right || b1.upper < b2.lower || b1.lower > b2.upper);
}