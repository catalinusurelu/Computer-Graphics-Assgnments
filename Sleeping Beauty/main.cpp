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

//texturi
#include "lab_texture_loader.hpp"

//camera
#include "lab_camera.hpp"

#include "VertexFormat.h"

#include "Colors.h"

#include "Finite_state_automaton.h"
#include "Particle.h"

//time
#include <ctime>
#include <cstdlib>
#include <chrono>


class Laborator : public lab::glut::WindowListener
{

//variabile
private:
	glm::mat4 model_matrix, view_matrix, projection_matrix;								//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int gl_program_shader_beauty;												//id obiect shader phong - frumoasa adormita
    unsigned int gl_program_shader_particle;                                            //id object shader particula

    unsigned int vao_sleeping_surpised, vbo_sleeping_surpised, ibo_sleeping_surpised, num_indices_sleeping_surpised;
    unsigned int vao_surpised_angry, vbo_surpised_angry, ibo_surpised_angry, num_indices_surpised_angry;
    unsigned int vao_angry_sleeping, vbo_angry_sleeping, ibo_angry_sleeping, num_indices_angry_sleeping;
    unsigned int num_indices_girl; // numarul de indici e acelasi pentru toate starile, o sa il setam la unul din cele de sus

	unsigned int mesh_vbo_particle, mesh_ibo_particle, mesh_vao_particle, mesh_num_indices_particle;

    unsigned int girl_texture, particle_texture_alpha;

    std::vector<VertexFormat> vertices_sleeping, vertices_surprised, vertices_angry;
    std::vector<unsigned int> indices_sleeping, indices_surprised, indices_angry;

    lab::Camera* cam;

    Finite_state_automaton fsm;

	glm::vec3 light_position;
	glm::vec3 eye_position;
	unsigned int material_shininess;

    bool fire;

    float gravity;

    glm::vec3 head_position;
    std::vector<Particle> particles;

//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader_beauty = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
        gl_program_shader_particle = lab::loadShader("shadere\\shader_particle_vertex.glsl", "shadere\\shader_particle_fragment.glsl");
		
        // incarca modele pentru stari
		lab::_loadObjFile("resurse\\girl_sleep.obj", vertices_sleeping, indices_sleeping);	
		lab::_loadObjFile("resurse\\girl_surprise.obj", vertices_surprised, indices_surprised);
		lab::_loadObjFile("resurse\\girl_annoyed.obj", vertices_angry, indices_angry);

        // incarc vao, vbo, ibo cu date pentru cate o tranzitie (adica vbo contine 2 VertexFormat, pentru 2 stari)
        load_models_transitions();

        // incarc texturi
        girl_texture = lab::loadTextureBMP("resurse\\girl_texture.bmp");
        particle_texture_alpha = lab::loadTextureBMP("resurse\\music.bmp");

        // adaug stari
        fsm.add_state(0);	
		fsm.add_state(1);
		fsm.add_state(2);

        // Tranzitiile sunt defapt (vao, timpi, numar_cadre)
        fsm.add_transition(vao_sleeping_surpised);
        fsm.add_transition(vao_surpised_angry);
        fsm.add_transition(vao_angry_sleeping);

        // Adaug timpul in secunde la trecere dintre 2 stari
        fsm.add_transition_times(0, 6);  // stare 1 -> stare 2
        fsm.add_transition_times(6, 12);  // starea 2 -> starea 3
        fsm.add_transition_times(12, 18);  // starea 3 -> starea 1

        head_position = center_of_model(vertices_sleeping);
        cam = new lab::Camera();
        cam->set(glm::vec3(head_position.x - 0.4, head_position.y, head_position.z + 2), head_position, glm::vec3(0,1,0));

        Particle::instantiate(0.4, 0.4); // instantiem mesha pentru particula
        
        // dau drumul la muzica de la inceput :)
        fire = true;

        gravity = 0.00000004;

        // facem seed pentru generarea particulelor
        srand(time(NULL));
    
		//lumina & material
		light_position = head_position + glm::vec3(-20, 0, 20);
		material_shininess = 1000;

		//matrice de modelare
		model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shadere
        glDeleteProgram(gl_program_shader_beauty);
		glDeleteProgram(gl_program_shader_particle);
    }

	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame()
    {
        static std::chrono::high_resolution_clock::time_point fire_start;
        if(fire)
        {
            fsm.start();

            // la fiecare frame generam cate n particule
            int n = 5;
            for(int i = 0 ; i < n; i++)
            {
                particles.push_back(generate_particles(glm::vec3(0.001, 0, -0.000005),
                                                       head_position + glm::vec3(-4, 0.1, -1), 
                                                       0.1, 9, 7, 10, 200, 400));
            }
        }

        auto ellapsed_time = std::chrono::high_resolution_clock::now() - fsm.get_start_time();
        float time_sec = std::chrono::duration_cast<std::chrono::seconds>(ellapsed_time).count();
        bool finished_firing = time_sec > 12? true : false;
        
        // Dupa terminarea tranzitiei 2, dispar notele
        if(fire && finished_firing)
        {
            fire = false;
            particles.clear();

            // data viitoare vrem alte particule
            srand(time(NULL));
        }

        // Luam totul de la capat
        // 24 = 18 (pt tranzitii) + 6 (somn - in timpul asta automatul nu se schimba, returneaza ultima stare)
        if(time_sec >= 24 && fire == false)
        {
            fsm.finish();
            fire = true;
        }

        fsm.update();

        eye_position = cam->position;
	}

	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){

        float blend_factor = fsm.get_blend_factor();
        unsigned int vao = fsm.get_current_transition();

		//bufferele din framebuffer sunt aduse la valorile initiale (setate de clear color si cleardepth)
		//adica se sterge ecranul si se pune culoare (si alte propietati) initiala
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//foloseste shaderul
		glUseProgram(gl_program_shader_beauty);
		
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_beauty, "model_matrix"),1,false,glm::value_ptr(model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_beauty,"view_matrix"),1,false,glm::value_ptr(cam->getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_beauty, "projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		glUniform3f(glGetUniformLocation(gl_program_shader_beauty, "light_position"),light_position.x, light_position.y, light_position.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_beauty, "eye_position"),eye_position.x, eye_position.y, eye_position.z);
		glUniform1i(glGetUniformLocation(gl_program_shader_beauty, "material_shininess"),material_shininess);
        glUniform1f(glGetUniformLocation(gl_program_shader_beauty, "blend_factor"),blend_factor);

        // texturam
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, girl_texture);
		glUniform1i(glGetUniformLocation(gl_program_shader_beauty, "textura"), 1);
		
        // desenam modelul curent
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, num_indices_girl, GL_UNSIGNED_INT, 0);

        // desenam particule
		glUseProgram(gl_program_shader_particle);

        auto frame_time = std::chrono::high_resolution_clock::now();
        for(auto it: particles)
        {
            //texturam 
		    glActiveTexture(GL_TEXTURE0 + 1);
		    glBindTexture(GL_TEXTURE_2D, particle_texture_alpha);
		    glUniform1i(glGetUniformLocation(gl_program_shader_particle, "textura"), 1);
		    glUniform1i(glGetUniformLocation(gl_program_shader_particle, "has_alpha"), 1);

            // desenam mesha

            auto ellapsed_time = frame_time - it.start_time;
            int time_milli = (std::chrono::duration_cast<std::chrono::milliseconds>(ellapsed_time).count());

		    //trimite variabile uniforme la shader
		    glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_particle, "model_matrix"),1,false,glm::value_ptr(model_matrix));
		    glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_particle, "view_matrix"),1,false,glm::value_ptr(cam->getViewMatrix()));
		    glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_particle, "projection_matrix"),1,false,glm::value_ptr(projection_matrix));
            glUniform3f(glGetUniformLocation(gl_program_shader_particle, "center"),it.center.x, it.center.y, it.center.z);
            glUniform3f(glGetUniformLocation(gl_program_shader_particle, "color"),it.color.x, it.color.y, it.color.z);
            glUniform3f(glGetUniformLocation(gl_program_shader_particle, "velocity"),it.velocity.x, it.velocity.y, it.velocity.z);
            glUniform1i(glGetUniformLocation(gl_program_shader_particle, "time"),time_milli);
            glUniform1f(glGetUniformLocation(gl_program_shader_particle, "width"),it.width);
            glUniform1f(glGetUniformLocation(gl_program_shader_particle, "height"),it.height);
            glUniform1f(glGetUniformLocation(gl_program_shader_particle, "gravity"),gravity);

            glBindVertexArray(it.vao);
            glDrawElements(GL_TRIANGLES, it.num_indices, GL_UNSIGNED_INT, 0);
        }
        
	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame()
    {
      
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
        if(key_pressed == GLUT_KEY_CTRL_L)
{
			//CTRL_L reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader_beauty);
            glDeleteProgram(gl_program_shader_particle);
			gl_program_shader_beauty = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
            gl_program_shader_particle = lab::loadShader("shadere\\shader_particle_vertex.glsl", "shadere\\shader_particle_fragment.glsl");
		}

        if(key_pressed == 'i')
        {
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}

		if(key_pressed == 'w') { cam->translateForward(0.2f); }
		if(key_pressed == 'a') { cam->translateRight(-0.2f); }
		if(key_pressed == 's') { cam->translateForward(-0.2f); }
		if(key_pressed == 'd') { cam->translateRight(0.2f); }
		if(key_pressed == 'r') { cam->translateUpword(0.2f); }
		if(key_pressed == 'f') { cam->translateUpword(-0.2f); }
		if(key_pressed == 'q') { cam->rotateFPSoY(1.f); }
		if(key_pressed == 'e') { cam->rotateFPSoY(-1.f); }
		if(key_pressed == 'z') { cam->rotateFPSoZ(-1.f); }
		if(key_pressed == 'c') { cam->rotateFPSoZ(1.f); }
		if(key_pressed == 't') { cam->rotateFPSoX(1.f); }
		if(key_pressed == 'g') { cam->rotateFPSoX(-1.f); }
	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y)
    {
    }
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y)
    {
    }
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ std::cout<<"Mouse scroll"<<std::endl;}

    // la shadere trimitem perechi de date pentru stari (pozitii starea 1, pozitii starea 2, normale 1, normale 2 ...)
    void load_models_transitions()
    {
        // tranzitia [sleeping, surprised)

        num_indices_sleeping_surpised = indices_sleeping.size();
        std::vector<VertexFormat> vertices;
        for(int i = 0; i < vertices_sleeping.size(); i++)
        {
            vertices.push_back(vertices_sleeping[i]);
            vertices.push_back(vertices_surprised[i]);
        }

        glGenVertexArrays(1, &vao_sleeping_surpised);
        glBindVertexArray(vao_sleeping_surpised);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
        glGenBuffers(1, &vbo_sleeping_surpised);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_sleeping_surpised);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&ibo_sleeping_surpised);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_sleeping_surpised);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_sleeping.size() * sizeof(unsigned int), &indices_sleeping[0], GL_STATIC_DRAW);

        // trimit setul de date pentru starea A
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
        glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(2*sizeof(float)*3));	//trimite texcoords pe pipe 2

        // trimit setul de date pentru starea B
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(VertexFormat) + 0));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(VertexFormat) + sizeof(float)*3));
        glEnableVertexAttribArray(5);
		glVertexAttribPointer(5,2,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(VertexFormat) + 2*sizeof(float)*3));


        // tranzitia [surprised, angry)
              
        num_indices_sleeping_surpised = indices_surprised.size();
        vertices.clear();
        for(int i = 0; i < vertices_surprised.size(); i++)
        {
            vertices.push_back(vertices_surprised[i]);
            vertices.push_back(vertices_angry[i]);
        }

        glGenVertexArrays(1, &vao_surpised_angry);
        glBindVertexArray(vao_surpised_angry);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
        glGenBuffers(1, &vbo_surpised_angry);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_surpised_angry);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&ibo_surpised_angry);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_surpised_angry);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_surprised.size() * sizeof(unsigned int), &indices_surprised[0], GL_STATIC_DRAW);

        // trimit setul de date pentru starea A
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
        glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(2*sizeof(float)*3));	//trimite texcoords pe pipe 2

        // trimit setul de date pentru starea B
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(VertexFormat) + 0));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(VertexFormat) + sizeof(float)*3));
        glEnableVertexAttribArray(5);
		glVertexAttribPointer(5,2,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(VertexFormat) + 2*sizeof(float)*3));

        // tranzitia [angry, sleeping)

        num_indices_sleeping_surpised = indices_angry.size();
        vertices.clear();
        for(int i = 0; i < vertices_angry.size(); i++)
        {
            vertices.push_back(vertices_angry[i]);
            vertices.push_back(vertices_sleeping[i]);
        }

        glGenVertexArrays(1, &vao_angry_sleeping);
        glBindVertexArray(vao_angry_sleeping);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
        glGenBuffers(1, &vbo_angry_sleeping);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_angry_sleeping);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&ibo_angry_sleeping);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_angry_sleeping);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_angry.size() * sizeof(unsigned int), &indices_angry[0], GL_STATIC_DRAW);

        // trimit setul de date pentru starea A
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
        glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(2*sizeof(float)*3));	//trimite texcoords pe pipe 2

        // trimit setul de date pentru starea B
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(VertexFormat) + 0));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(VertexFormat) + sizeof(float)*3));
        glEnableVertexAttribArray(5);
		glVertexAttribPointer(5,2,GL_FLOAT,GL_FALSE,2 * sizeof(VertexFormat),(void*)(sizeof(VertexFormat) + 2*sizeof(float)*3));

        num_indices_girl = num_indices_sleeping_surpised; // oricum toate sunt la fel
    }

    // Consideram centrul modelului ca fiind centrul sau de greutate 
    // Ne va fi folositor sa calculam distante fata de el
    glm::vec3 center_of_model(std::vector<VertexFormat> vertices)
    {
        glm::vec3 result = glm::vec3(0.f, 0.f, 0.f);
        for(auto it: vertices)
        {
            result += glm::vec3(it.position_x, it.position_y, it.position_z);
        }
        result /= vertices.size();
        return result;
    }
};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("Sleeping Beauty"),800,600,100,100,true);
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
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tCTRL_L... reincarca shadere"<<std::endl<<"\ti ... toggle wireframe"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}