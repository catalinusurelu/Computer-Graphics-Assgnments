#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform3D.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <thread>
#include <algorithm>

#include "Colors.h"
#include "Road.h"
#include "RoadStripes.h"
#include "Player.h"
#include "Obstacle.h"
#include "PhysicsEngine.h"
#include "PhysicsProperties.h"
#include "Movable.h"
#include "Enemy.h"
#include "PowerUp.h"
#include "Wheel.h"
#include "Movable.h"



#define PI 3.14159265358979323846
#define inf 1000000

using namespace std;
using namespace PhysicsProperties;

Visual2D *v2d;
PhysicsEngine* physX;

Player* player;
Road* road;
RoadStripes* stripes;

Text *scoreTitle, *scoreValue, *livesTitle, *livesValue, *distanceTitle, *distanceValue;
Text *timeTitle, *timeValue, *carburantTitle, *carburantValue;

int& lives = Player::lives;
int delay;
bool upArrow, downArrow, leftArrow, rightArrow;

float road_height;
float road_length;
float road_width;

float n=1;

bool finishLineAdded = false;

int &height = DrawingWindow::height;
int &width = DrawingWindow::width;

float left_x;
float right_x;
float upper_y;
float lower_y;
float log_width;
float log_height;

time_t startTime;
float totalDistance;

int benzina;

Point3D generatingPoints[3]; // puncte unde se plaseaza obstacole
vector<Movable*> obstacole;

int obstacleDelay;
int obstacleDelayStart;

inline int minutes()
{
    return ((int)(time(NULL) - startTime)) / 60;
}
inline int seconds()
{
    return ((int)(time(NULL) - startTime)) % 60;
}

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{

    width = 640;
    height = 480;

    road_height = 0.01f;
    road_length = 300.f;
    road_width = 20.f;

    // colturi ecran
    left_x = -30;
    right_x = 30;
    lower_y = -60;
    upper_y = 60;

    log_width = 60;
    log_height = 120;

    startTime = time(NULL);

    // setam proprietatile fizice
    da = road_length / 1500; // acceleratie liniaraa
    daa = road_width / 30; // acceleratie unghiulara
    speedDragConstant = 0.95;
    angularDragConstant = 0.85;
    speedLimit = road_length / 100;

    totalDistance = road_length * 10;
    lives = 10;
    benzina = totalDistance / speedLimit * 9;

    obstacleDelay = totalDistance / 30;
	
	v2d = new Visual2D(left_x,lower_y,right_x,upper_y,0,0,width,height); 
	//v2d1->tipTran(true);
	addVisual2D(v2d);

    physX = new PhysicsEngine();
    player = new Player(v2d, physX, BLUE, BLUE, BLACK, false, false, false, lives, benzina);
    stripes = new RoadStripes(20, road_width, road_length, road_height, v2d, physX, WHITE, true);
    road = new Road(road_width, road_length, road_height, DARK_GREY, true, v2d);

    physX->addPlayer(player);
    physX->addRoad(road);

    player->move(0, 0, road_length / 2.5);

    float distText = log_width / 5;

    // puncte care spun locul in care e plasat un obstacol pe banda (cate un punct per banda)
    generatingPoints[0] = Point3D(-road_width / 3, 0, -road_length / 2 - player->length / 2);
    generatingPoints[1] = Point3D(0, 0, -road_length / 2 - player->length / 2);
    generatingPoints[2] = Point3D(road_width / 3, 0, -road_length / 2 - player->length / 2);

    scoreTitle = new Text("SCORE", Point2D(left_x + distText / 3, upper_y - log_height / 10), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(scoreTitle,  v2d);

    scoreValue = new Text(to_string(player->score), Point2D(left_x + distText / 3 , upper_y - log_height / 7), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(scoreValue,  v2d);

    livesTitle = new Text("LIVES", Point2D(left_x + distText * 1  + distText / 3, upper_y - log_height / 10), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(livesTitle,  v2d);

    livesValue = new Text(to_string(lives) + "X ", Point2D(left_x + distText * 1  + distText / 3, upper_y - log_height / 7), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(livesValue,  v2d);

    carburantTitle = new Text("CARBURANT", Point2D(left_x + distText * 2  + distText / 3, upper_y - log_height / 10), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(carburantTitle,  v2d);

    carburantValue = new Text(to_string(player->benzina), Point2D(left_x + distText * 2  + distText / 3, upper_y - log_height / 7), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(carburantValue,  v2d);

    timeTitle = new Text("TIME", Point2D(left_x + distText * 3  + distText / 3, upper_y - log_height / 10), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(timeTitle,  v2d);

    timeValue = new Text(to_string(minutes()) + ":" + to_string(seconds()), Point2D(left_x + distText * 3  + distText / 3, upper_y - log_height / 7), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(timeValue,  v2d);

    distanceTitle = new Text("DISTANCE", Point2D(left_x + distText * 4  + distText / 3, upper_y - log_height / 10), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(distanceTitle,  v2d);

    distanceValue = new Text(to_string((int)(100. * player->distantaParcursa / totalDistance)) + "%", Point2D(left_x + distText * 4  + distText / 3, upper_y - log_height / 7), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
    addText_to_Visual2D(distanceValue,  v2d);
}


void updatePlayerInput()
{
    if(lives == 0) return;
    if(upArrow) player->accelerate();
    if(downArrow) player->brake();
    if(rightArrow) player->rotate_left(); // se misca invers, pentru ca noi ne miscam pe axa negativa
    if(leftArrow) player->rotate_right(); // daca era pe cea pozitiva se misca normal
}

void applyProjection()
{
    Transform3D::loadIdentityModelMatrix();
    Transform3D::loadIdentityProjectionMatrix();
	Transform3D::perspectiveProjectionMatrix(0.7, 20, 175);
    std::for_each(physX->objects.begin(), physX->objects.end(), 
            [] (Movable* x) {Transform3D::applyTransformProjection(x);});
}

void addObstacles()
{
    srand(time(NULL));
    int banda = rand() % 3;
    int objType = rand() % 100;
    float x = generatingPoints[banda].x;
    float y = generatingPoints[banda].y;
    float z = generatingPoints[banda].z;


    if(objType < 20)
    {
        obstacole.push_back(new PowerUp(v2d, physX, true));
    }
    else if(objType < 50)
    {
        obstacole.push_back(new Enemy(v2d, physX, RED, RED, BLACK, false, false, false));
    }
    else
    {
        obstacole.push_back(new Obstacle(v2d, physX, YELLOW, false));
    }

    obstacole.back()->move(x, y, z);
}

//functia care permite animatia
void DrawingWindow::onIdle()
{
    if(lives == -1 || totalDistance == -1)
    {
        std::chrono::milliseconds timp(2000);
        std::this_thread::sleep_for(timp);
        glutLeaveMainLoop();
        return;
    }

    if(lives == 0 || player->benzina <= 0)
    {
        Text* lost = new Text("YOU LOST!!! ", Point2D(0, upper_y - log_height / 4), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
        DrawingWindow::addText_to_Visual2D(lost,  v2d);

        lives--;
        totalDistance = -1;
        return;
    }

    if(totalDistance < player->distantaParcursa)
    {
        Text* won = new Text("YOU WON!!! ", Point2D(0, upper_y - log_height / 4), GREEN, GLUT_BITMAP_TIMES_ROMAN_24);
        DrawingWindow::addText_to_Visual2D(won,  v2d);

        totalDistance = -1;
        return;
    }

    removeObject3D_from_Visual2D(road, v2d); // stergem strada si o adaugam la loc dupa ce adaugam obiectele noi (daca e cazul)
                                             // altfel, se pune peste ele, si nu se vad

    if(totalDistance - player->distantaParcursa - player->length <= road_length && !finishLineAdded)
    {
        stripes->addFinishLine();
        finishLineAdded = true;
    }

    updatePlayerInput();
    physX->update();

    if(player->distantaParcursa - obstacleDelayStart > obstacleDelay)
    {
        obstacleDelayStart = player->distantaParcursa;
        addObstacles();
    }

    addObject3D_to_Visual2D(road, v2d);

    applyProjection();

    timeValue->text = to_string(minutes()) + ":" + to_string(seconds());
    distanceValue->text = to_string((int)(100. * player->distantaParcursa / totalDistance)) + "%";
    scoreValue->text = to_string(player->score);
    livesValue->text = to_string(max(lives, 0));
    carburantValue->text = to_string(player->benzina);
    
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{
	v2d->poarta(0,0,width,height); 
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


void DrawingWindow::onKeyDown(unsigned char key, int x, int y)
{
    switch(key)
    {
        case KEY_ESC: glutLeaveMainLoop();
    }
	
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{
	
}


int main(int argc, char** argv)
{
	//creare fereastra
    DrawingWindow dw(argc, argv, 640, 480, 0, 0, "Cars");
    glutFullScreen();
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;

}