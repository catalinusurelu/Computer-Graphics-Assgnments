#ifndef PARTICLE_H
#define PARTICLE_H

#include "lab_glut.hpp"

#include <ctime>
#include <chrono>
#include <cstdlib>
#include <vector>


class Particle
{
public:
    Particle() {};
    ~Particle() {}; 
    
    // Daca implementez in destructor e mai rau ca se 
    // sterg mereu cand copiez variabila sau o returnez etc.
    // Ar trebuisa implementez reference counting si nu prea
    // are rost.
    void clear()
    {
        glDeleteBuffers(1,&vbo);	
        glDeleteBuffers(1,&ibo);	
        glDeleteVertexArrays(1,&vao);	
    }

    // Instantiem mesh-ul ca un singleton.
    // Astfel, folosim mai putin resurse, deoarece, putem refolosi acelasi
    // mesh pentru toate particulele
    static void instantiate(float particle_width, float particle_height)
    {
        if(!instantiated)
        {
            width = particle_width;
            height = particle_height;
            // generez QUAD

            std::vector<VertexFormat> vertices;
            std::vector<unsigned int> indices;

            glm::vec3 stSus = glm::vec3(- particle_width / 2, + particle_height / 2, 0);
            glm::vec3 drSus = glm::vec3(+ particle_width / 2, + particle_height / 2, 0);
            glm::vec3 stJos = glm::vec3(- particle_width / 2, - particle_height / 2, 0);
            glm::vec3 drJos = glm::vec3(+ particle_width / 2, - particle_height / 2, 0);

            // Punem si coordonate UV
            vertices.push_back(VertexFormat(stSus.x, stSus.y, stSus.z, 0, 0, 0, 0, 1));
            vertices.push_back(VertexFormat(drSus.x, drSus.y, drSus.z, 0 ,0, 0, 1, 1));
            vertices.push_back(VertexFormat(stJos.x, stJos.y, stJos.z, 0 ,0, 0, 0, 0));
            vertices.push_back(VertexFormat(drJos.x, drJos.y, drJos.z, 0 ,0, 0, 1, 0));

            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);

            indices.push_back(1);
            indices.push_back(3);
            indices.push_back(2);
            num_indices = 6;

            glGenVertexArrays(1, &vao);
	        glBindVertexArray(vao);
		
	        //vertex buffer object -> un obiect in care tinem vertecsii
	        glGenBuffers(1, &vbo);
	        glBindBuffer(GL_ARRAY_BUFFER, vbo);
	        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

	        //index buffer object -> un obiect in care tinem indecsii
	        glGenBuffers(1,&ibo);
	        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	        glEnableVertexAttribArray(0);
	        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
	        glEnableVertexAttribArray(1);
	        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
            glEnableVertexAttribArray(2);
	        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(2*sizeof(float)*3));	//trimite coordonate UV pe pipe 2
            
            instantiated = true;
        }
    }

    static unsigned int vao, vbo, ibo, num_indices;
    static float width;
    static float height;
    static bool instantiated;
    std::chrono::high_resolution_clock::time_point start_time;
    glm::vec3 color;
    glm::vec3 velocity;
    glm::vec3 center;
   
};

// genereaza particule pornind dintr-un paralelipiped definit de un coltz,
// lungime, latime is adancime. Adancimea e pe axa OX, adica ce cu directia
// proiectilului. Particulele pornesc din puncte random
Particle generate_particles(glm::vec3 velocity, glm::vec3 colt,
                           float width, float height, float depth,
                           int grid_points_width, int grid_points_height, int grid_points_depth)
{
    // calculam distanta dintre 2 puncte in plan (puncte din care ies particule) => granularitate
    float grid_point_dist_width = abs(colt.z - width) / (grid_points_width - 1);
    float grid_point_dist_height = abs(colt.y - height) / (grid_points_height - 1);
    float grid_point_dist_depth = abs(colt.x - depth) / (grid_points_depth - 1);

    // generez coordonate random pentru punctul din care iese particula
    // cordonata e doar coordonata din grid, pozitia in coordonate reale o
    // calculam mai jos
    int poz_y = rand() % grid_points_height;
    int poz_z = rand() % grid_points_width;
    int poz_x = rand() % grid_points_depth;

    Particle x;
    x.center = glm::vec3(colt.x - poz_x * grid_point_dist_depth, colt.y + poz_y * grid_point_dist_height, colt.z - poz_z * grid_point_dist_width);
    x.color = glm::vec3((rand() % 255) / 255.f, (rand() % 255) / 255.f, (rand() % 255) / 255.f); // culoare random
    x.velocity = velocity;
    x.start_time = std::chrono::high_resolution_clock::now(); // timpul e folosit pentru a calcula pozitia acutala tinand cont de viteza, acceleratie etc..


    return x;
}

unsigned int Particle::vao = -1;
unsigned int Particle::vbo = -1;
unsigned int Particle::ibo = -1;
unsigned int Particle::num_indices = -1;
bool Particle::instantiated = false;
float Particle::width = 1;
float Particle::height = 1;

#endif