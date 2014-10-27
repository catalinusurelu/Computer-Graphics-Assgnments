#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Transform3D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include "dependente/freeglut.h"
#include <iostream>
#include <windows.h>
#include <time.h>
#include <thread>
#include <algorithm>

#include "PhysicsEngine.h"
#include "PlayerShip2D.h"
#include "PlayerShip.h"
#include "Elice.h"
#include "Plane.h"
#include "Chain.h"
#include "Wall.h"
#include "Colors.h"
#include "PhysicsProperties.h"


#define PI 3.14159265358979323846
#define inf 1000000
#define DELAY 10

using namespace std;
using namespace PhysicsProperties;

Visual2D *v2d_battle_field, *v2d_HUD;
Text *scoreTitle, *scoreValue, *livesTitle, *livesValue, *swarmTitle, *swarmValue;

int score = 0;
int& lives = PlayerShip::lives;
int delay;
bool upArrow, downArrow, leftArrow, rightArrow;
bool drillActivat = false;
bool torpedo; // tasta space
bool levelFinished = false;
int generatedEnemies = 0; 

enum ternaryBool { adevarat, nedeterminat, fals} drill; // tasta A

PlayerShip *ship;
PhysicsEngine* physicsEngine;

int &height = DrawingWindow::height;
int &width = DrawingWindow::width;

// variabila ce da marimea navelor (nu inseamna efectiv raza, ci  
// aproximeaza niste dimensiuni ca sa para ca se afla intr-un cerc
int shipRadius;

int swarmLevel = 1;

time_t lastTime = time(NULL);

// colturi/pozitii din care ies inamicii
Point2D corners[4];



//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
    width = 1366;
    height = 768;

    physicsEngine = new PhysicsEngine();

    // initializam constante de fizica si raza navelor
    da = height / (float)750;
    daa = height / (float)100;
    speedDragConstant = 0.95;
    angularDragConstant = 0.85;
    speedOfLight = 30 * da / 4;
    shipRadius = height / 40;

    corners[0] = Point2D(width / 9, height / 9);
    corners[1] = Point2D(width / 9, height * 8 / 10);
    corners[2] = Point2D(width * 8 / 10, height / 9);
    corners[3] = Point2D(width * 8 / 10, height * 8 / 10);
	
    //Spatiu de joc - lupta
	v2d_battle_field = new Visual2D(0, 0, width, height, 0, height * 9 / 10, width, height); 
    v2d_battle_field->cadruPoarta(BLUE);
	addVisual2D(v2d_battle_field);

    // Chenarul spatiului de joc
    Wall* wall = new Wall(Point2D(0, 0), width, height, physicsEngine);

    // HUD
	v2d_HUD = new Visual2D(0, 0, width, height, height, 0, width, height * 9 / 10); 
    v2d_HUD->cadruPoarta(ORANGE);
	addVisual2D(v2d_HUD);

	scoreTitle = new Text("SCORE", Point2D(width / 2 , height / 2), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(scoreTitle,  v2d_HUD);

    scoreValue = new Text(to_string(score), Point2D(width / 2 , height / 5), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(scoreValue,  v2d_HUD);

    livesTitle = new Text("LIVES", Point2D(width * 8 / 10, height / 2), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(livesTitle,  v2d_HUD);

    livesValue = new Text(to_string(lives) + "X ", Point2D(width * 8 / 10, height / 5), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(livesValue,  v2d_HUD);

    swarmTitle = new Text("SWARM", Point2D(width / 9, height / 2), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(swarmTitle,  v2d_HUD);

    swarmValue = new Text(to_string(swarmLevel), Point2D(width / 9, height / 5), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(swarmValue,  v2d_HUD);


    // Nava Player
    ship = new PlayerShip(shipRadius, ORANGE, ORANGE, WHITE, physicsEngine, v2d_battle_field);
    ship->move(width / 2, height / 2);
    drill = nedeterminat;
    torpedo = false;

}

void generateSwarm()
{
    srand (time(NULL));
    time_t currentTime = time(NULL);

    // cat timp nu am omorat inamicii (2 = eu + perete)
    if(swarmLevel == 1 && currentTime - lastTime > 2)
    {
        lastTime = currentTime;
        // genereaza timp de 3 secunde
        if(generatedEnemies < 5)
        {
           generatedEnemies++;
           int i = generatedEnemies % 4;
           int enemy = rand() % 100;

           // densitati de probabilitate in functie de tipul inamicului (cei mai puternici
           // apar mai rar)
           if(enemy < 30)
           {
                Chain* chain = new Chain(shipRadius, ORANGE, physicsEngine, v2d_battle_field, &score);
                chain->move(corners[i].x, corners[i].y);
           }
           else if(enemy < 50)
           {
                 Plane* plane = new Plane(shipRadius, YELLOW, physicsEngine, v2d_battle_field, &score);
                 plane->move(corners[i].x, corners[i].y);
           }
           else
           {
               Elice* elice = new Elice(shipRadius, RED, physicsEngine, v2d_battle_field, &score);
               elice->move(corners[i].x, corners[i].y);
           }
        }
        if(physicsEngine->objects.size() <= 2 && generatedEnemies == 5)
        {
             swarmLevel++;
             generatedEnemies = 0;
        }
    }

    if(swarmLevel == 2 && currentTime - lastTime > 2)
    {
        lastTime = currentTime;
        if(generatedEnemies < 10)
        {
           for(int j = 1; j <= 2; j++) {
               generatedEnemies++;
               lastTime = currentTime;
               int i = generatedEnemies % 4;
               int enemy = rand() % 100;

               if(enemy < 30)
               {
                    Chain* chain = new Chain(shipRadius, ORANGE, physicsEngine, v2d_battle_field, &score);
                    chain->move(corners[i].x, corners[i].y);
               }
               else if(enemy < 50)
               {
                     Plane* plane = new Plane(shipRadius, YELLOW, physicsEngine, v2d_battle_field, &score);
                     plane->move(corners[i].x, corners[i].y);
               }
               else
               {
                   Elice* elice = new Elice(shipRadius, RED, physicsEngine, v2d_battle_field, &score);
                   elice->move(corners[i].x, corners[i].y);
               }
            }
        }
        if(physicsEngine->objects.size() <= 2 && generatedEnemies == 10)
        {
           swarmLevel++;
           generatedEnemies = 0;
        }
    }

    if(swarmLevel == 3 && currentTime - lastTime > 2)
    {
        lastTime = currentTime;
        if(generatedEnemies < 16)
        {
           for(int j = 1; j <= 4; j++) {
               generatedEnemies++;
               lastTime = currentTime;
               int i = generatedEnemies % 4;
               int enemy = rand() % 100;

               if(enemy < 30)
               {
                    Chain* chain = new Chain(shipRadius, ORANGE, physicsEngine, v2d_battle_field, &score);
                    chain->move(corners[i].x, corners[i].y);
               }
               else if(enemy < 50)
               {
                     Plane* plane = new Plane(shipRadius, YELLOW, physicsEngine, v2d_battle_field, &score);
                     plane->move(corners[i].x, corners[i].y);
               }
               else
               {
                   Elice* elice = new Elice(shipRadius, RED, physicsEngine, v2d_battle_field, &score);
                   elice->move(corners[i].x, corners[i].y);
               }
            }
        }
        if(physicsEngine->objects.size() <= 2 && generatedEnemies == 15)
        {
            swarmLevel++;
            Text* swarmText = new Text("YOU WON!!! ", Point2D(width / 2, height / 2), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
            DrawingWindow::addText_to_Visual2D(swarmText,  v2d_battle_field);
            std::for_each(physicsEngine->objects.begin(), physicsEngine->objects.end(), 
                            [] (Movable* x) {removeShip_from_Visual2D(static_cast<Ship*>(x), v2d_battle_field);});

        }
    }

    if(swarmLevel == 4) 
    {
        std::chrono::milliseconds timp(2000);
        std::this_thread::sleep_for(timp);
        glutLeaveMainLoop();
    }
}


void updatePlayerInput()
{
    if(lives == 0) return;
    if(upArrow) ship->accelerate();
    if(downArrow) ship->brake();
    if(rightArrow) ship->rotate_right();
    if(leftArrow) ship->rotate_left();
    if(drill == adevarat && !torpedo)
    {
        ship->activateDrill();
        drill = nedeterminat; // nici nu se tine apasat nici off key, altfel ar face new/delete intruna
        drillActivat = true;
    }
    if(drill == fals)
    {
        ship->deactivateDrill();
        drill = nedeterminat;
        drillActivat = false;
    }

    if(torpedo && !Drill::active && delay == 0)
    {
        ship->fire();
        delay = DELAY;
    }
    delay = max(delay - 1, 0); 
}

void updateEnemies()
{
    Objects::iterator it;
    for(it = physicsEngine->objects.begin(); it != physicsEngine->objects.end(); ++it)
    {
        if((*it)->type == "enemy")
        {
            Ship* enemy = dynamic_cast<Ship*>(*it);
            enemy->updateAI(ship, Drill::active);
        }
    }
}

//functia care permite animatia
void DrawingWindow::onIdle()
{
    if(lives == -1)
    {
        std::chrono::milliseconds timp(2000);
        std::this_thread::sleep_for(timp);
        glutLeaveMainLoop();
    }

    if(lives == 0)
    {
        Text* swarmText = new Text("YOU LOST!!! ", Point2D(width / 2, height / 2), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
        DrawingWindow::addText_to_Visual2D(swarmText,  v2d_battle_field);
        std::for_each(physicsEngine->objects.begin(), physicsEngine->objects.end(), 
            [] (Movable* x) {removeShip_from_Visual2D(static_cast<Ship*>(x), v2d_battle_field);});
        lives--;
    }

    updatePlayerInput();
    updateEnemies();
    generateSwarm();
    physicsEngine->update();

    swarmValue->text = to_string(swarmLevel);
    scoreValue->text = to_string(score);
    livesValue->text = to_string(max(lives, 0));
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{
	v2d_battle_field->poarta(0,height / (float)10,width,height);
	v2d_HUD->poarta(0,0,width,height / (float)10);
}


void DrawingWindow::onKeyUp(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'A':
    case 'a':
        drill = fals;
        break;
    case KEY_SPACE: torpedo = false;
    }
}

void DrawingWindow::onKeyDown(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'A':
    case 'a':
        drill = adevarat;
        break;
    case KEY_SPACE: torpedo = true; break;
    case KEY_ESC: glutLeaveMainLoop();
    }
	
}


void DrawingWindow::onKeySpecialUp(int key, int x, int y)
{
    switch(key)
    {
    case KEY_LEFT: leftArrow = false; break;
    case KEY_RIGHT: rightArrow = false; break;
    case KEY_UP: upArrow = false; break;
    case KEY_DOWN: downArrow = false; break;
    }
}

void DrawingWindow::onKeySpecialDown(int key, int x, int y)
{
    switch(key)
    {
    case KEY_LEFT: leftArrow = true; break;
    case KEY_RIGHT: rightArrow = true; break;
    case KEY_UP: upArrow = true; break;
    case KEY_DOWN: downArrow = true; break;
    }
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{
	
}


int main(int argc, char** argv)
{
	//creare fereastra
	DrawingWindow dw(argc, argv, 1366, 768, 0, 0, "Geometry Wars");
    glutFullScreen();
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;

}