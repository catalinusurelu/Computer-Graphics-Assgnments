//-------------------------------------------------------------------------------------------------
// Descriere: fisier main
//
// Autor: student
// Data: today
//-------------------------------------------------------------------------------------------------

//incarcator de meshe
#include "lab_mesh_loader.hpp"

//geometrie: drawSolidCube, drawWireTeapot...
#include "lab_geometry.hpp"

//incarcator de shadere
#include "lab_shader_loader.hpp"

//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"

//camera
#include "lab_camera.hpp"

#include "VertexFormat.h"

#include "dependente\glm\gtx\rotate_vector.hpp"

#include "Colors.h"

//time
#include <ctime>
#include <iostream>

#include "Stadium.hpp"
#include "Runner.hpp"
#include "Sky.hpp"
#include "TPSCam.hpp"
#include "Flag.hpp"

class Laborator : public lab::glut::WindowListener{

//variabile
private:
	glm::mat4 model_matrix, view_matrix, projection_matrix;								//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int gl_program_shader_phong;												//id obiect shader phong
	unsigned int gl_program_shader_curent;												//id obiect shader curent
    unsigned int gl_program_shader_wave;

    glm::vec3 winner_material_kd;
	glm::vec3 winner_material_ks;


    lab::Camera* cam;
    lab::Camera skyCamera;
    Stadium* stadion;
    Runner *runner1, *runner2, *runner3;
    Sky* sky;
    TPSCam* tpscam;
    Flag* flag;

	glm::vec3 light_position;
	glm::vec3 eye_position;
	unsigned int material_shininess;

    bool accelerate, decelerate, rotateLeft, rotateRight;
    bool finished, flagNotDrawn;


//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
        gl_program_shader_wave = lab::loadShader("shadere\\shader_wave_phong_vertex.glsl", "shadere\\shader_wave_phong_fragment.glsl");
		gl_program_shader_curent = gl_program_shader_phong;
		
        float speed_runner2, speed_runner3;
        std::ifstream in("resurse/viteze.txt");
        in >> speed_runner2 >> speed_runner3;

        stadion = new Stadium(10, 1, 4, 0.1, 10, gl_program_shader_curent);
        sky = new Sky(SKY_BLUE, SKY_BLUE, 100, 5000, gl_program_shader_curent);

        runner1 = new Runner(BLUE, BLUE, glm::vec3(stadion->midTrack[0], 0, 0), 1, stadion, gl_program_shader_curent);
        Runner::loadModel("resurse\\runner\\", 14);
        runner1->setAttributes(0, 0.005, 0.01, 0.0001, 0.00005, 0.5, 0);
        cam =  &(runner1->cam);
        
        runner2 = new Runner(YELLOW, YELLOW, glm::vec3(stadion->midTrack[1], 0, 0), 1, stadion, gl_program_shader_curent);
        runner2->setAutopilot(speed_runner2);

        runner3 = new Runner(RED, RED, glm::vec3(stadion->midTrack[2], 0, 0), 1, stadion, gl_program_shader_curent);
        runner3->setAutopilot(speed_runner3);

        accelerate = false;
        decelerate = false;
        rotateLeft = false;
        rotateRight = false;

        finished = false;
        flagNotDrawn = true;

        tpscam = new TPSCam(BLACK, BLACK, runner1, stadion, "resurse\\camera.obj", gl_program_shader_curent);

        skyCamera.set(glm::vec3(0,40,0), glm::vec3(0,0,0), glm::vec3(0,0,-1));

		//lumina & material
		eye_position = glm::vec3(0,10,50);
		light_position = glm::vec3(0,20,-20);
		material_shininess = 100;

		//matrici de modelare si vizualizare
		model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shadere
		glDeleteProgram(gl_program_shader_phong);
    }

	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame(){
	}

	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		//bufferele din framebuffer sunt aduse la valorile initiale (setate de clear color si cleardepth)
		//adica se sterge ecranul si se pune culoare (si alte propietati) initiala
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//foloseste shaderul
		glUseProgram(gl_program_shader_curent);
				
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "model_matrix"),1,false,glm::value_ptr(model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent,"view_matrix"),1,false,glm::value_ptr(cam->getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "light_position"),light_position.x, light_position.y, light_position.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "eye_position"),eye_position.x, eye_position.y, eye_position.z);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "material_shininess"),material_shininess);
		
        if(finished)
        {
            //trimite variabile uniforme la shader wave (pentru panza)
            glUseProgram(gl_program_shader_wave);
            glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_wave, "model_matrix"),1,false,glm::value_ptr(model_matrix));
		    glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_wave,"view_matrix"),1,false,glm::value_ptr(cam->getViewMatrix()));
		    glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_wave, "projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		    glUniform3f(glGetUniformLocation(gl_program_shader_wave, "light_position"),light_position.x, light_position.y, light_position.z);
		    glUniform3f(glGetUniformLocation(gl_program_shader_wave, "eye_position"),eye_position.x, eye_position.y, eye_position.z);
		    glUniform1i(glGetUniformLocation(gl_program_shader_wave, "material_shininess"),material_shininess);

            flag->draw();
            return;
        }

        stadion->draw();
        sky->draw();
        tpscam->draw();
        runner1->draw();
        runner2->draw();
        runner3->draw();
	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame()
    {
        if(finished)
        {
            return;
        }

        if(accelerate) runner1->accelerate();
        if(decelerate) runner1->decelerate();
        if(rotateLeft) runner1->rotate_left();
        if(rotateRight) runner1->rotate_right();
        runner1->decelerate();
        runner2->autopilot();
        runner3->autopilot();
        tpscam->update();

        // setam checkpoint
        if(runner1->position.x < 0)
        {
            runner1->checkpoint = true;
        }
        if(runner2->position.x < 0)
        {
            runner2->checkpoint = true;
        }
        if(runner2->position.x < 0)
        {
            runner2->checkpoint = true;
        }

        // verificam daca am trecut de linia de final
        if(runner1->checkpoint && runner1->position.x > 0 && runner1->position.z < 0)
        {
            finished = true;
            runner1->winner = true;
        }
        if(runner2->checkpoint && runner2->position.x > 0 && runner2->position.z < 0)
        {
            finished = true;
            runner2->winner = true;
        }
        if(runner3->checkpoint && runner3->position.x > 0 && runner3->position.z < 0)
        {
            finished = true;
            runner3->winner = true;
        }

        // verificam daca a castigat cineva
        if(runner1->isWinner())
        {
            finished = true;
            winner_material_kd = runner1->material_kd;
            winner_material_ks = runner1->material_ks;
        } else if(runner2->isWinner())
        {
            finished = true;
            winner_material_kd = runner2->material_kd;
            winner_material_ks = runner2->material_ks;
        }
        else if(runner3->isWinner())
        {
            finished = true;
            winner_material_kd = runner3->material_kd;
            winner_material_ks = runner3->material_ks;
        }

        eye_position = cam->position;

        // daca a castigat cineva desenam steagul cu culoare lui
        if(finished && flagNotDrawn)
        {
            cam = &skyCamera;
            cam->set(glm::vec3(0,15,0), glm::vec3(0,0,0), glm::vec3(0,0,-1));
            cam->translateRight(0);
            cam->translateUpword(-12);
            cam->rotateFPSoX(10.0f);
            cam->rotateFPSoY(10.0f);
            cam->rotateFPSoZ(6.0f);
            eye_position = glm::vec3(0,10,50); // arata mai bine asa
            flag = new Flag(winner_material_kd, winner_material_ks, 20.f, 1.f, 10.f, 20.f, gl_program_shader_phong, gl_program_shader_wave);
            flagNotDrawn = false;
        }
    }
	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height==0) height=1;
		glViewport(0,0,width,height);
		projection_matrix = glm::perspective(90.0f, (float)width/(float)height,0.1f, 10000.0f);
	}


	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader_phong);
			gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
			gl_program_shader_curent = gl_program_shader_phong;
		}

        if(key_pressed == 'i'){
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}

        if(key_pressed == '1') { cam = &(runner1->cam); } 
        if(key_pressed == '2') { cam = &(runner2->cam); } 
        if(key_pressed == '3') { cam = &(runner3->cam); } 
        if(key_pressed == '4') { cam = &(tpscam->cam); } 
        if(key_pressed == '5') { cam = &skyCamera; } 
         if(key_pressed == '6') { skyCamera.set(glm::vec3(0,40,0), glm::vec3(0,0,0), glm::vec3(0,0,-1)); }

		if(key_pressed == 'w') { cam->translateForward(1.0f); }
		if(key_pressed == 'a') { cam->translateRight(-1.0f); }
		if(key_pressed == 's') { cam->translateForward(-1.0f); }
		if(key_pressed == 'd') { cam->translateRight(1.0f); }
		if(key_pressed == 'r') { cam->translateUpword(1.0f); }
		if(key_pressed == 'f') { cam->translateUpword(-1.0f); }
		if(key_pressed == 'q') { cam->rotateFPSoY(1.0f); }
		if(key_pressed == 'e') { cam->rotateFPSoY(-1.0f); }
		if(key_pressed == 'z') { cam->rotateFPSoZ(-1.0f); }
		if(key_pressed == 'c') { cam->rotateFPSoZ(1.0f); }
		if(key_pressed == 't') { cam->rotateFPSoX(1.0f); }
		if(key_pressed == 'g') { cam->rotateFPSoX(-1.0f); }


	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();

        if(key_pressed == GLUT_KEY_UP) accelerate = true;
        if(key_pressed == GLUT_KEY_DOWN) decelerate = true;
        if(key_pressed == GLUT_KEY_LEFT) rotateLeft = true;
        if(key_pressed == GLUT_KEY_RIGHT) rotateRight = true;
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y)
    {
        if(key_released == GLUT_KEY_UP) accelerate = false;
        if(key_released == GLUT_KEY_DOWN) decelerate = false;
        if(key_released == GLUT_KEY_LEFT) rotateLeft = false;
        if(key_released == GLUT_KEY_RIGHT) rotateRight = false;
    }
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ std::cout<<"Mouse scroll"<<std::endl;}

};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("Atletism"),800,600,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);
    //lab::glut::enterFullscreen();

	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\tw ... toggle wireframe"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}