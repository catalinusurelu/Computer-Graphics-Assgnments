#pragma once
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtx\rotate_vector.hpp"
#include "dependente\freeglut\freeglut.h"
#include "dependente\glew\glew.h"
#include "dependente\glm\glm.hpp"
#include "lab_mesh_loader.hpp"

#include "VertexFormat.h"
#include "Colors.h"
#include "lab_camera.hpp"
#include "Stadium.hpp"

#include <vector>
#include <string>

class Runner
{
public:
	Runner(glm::vec3 material_kd,  glm::vec3 material_ks,
            glm::vec3 position,
            float radius, Stadium* stadium,
            unsigned int gl_program_shader)
    {
        gl_program_shader_curent = gl_program_shader;

        this->material_kd = material_kd;
        this->material_ks = material_ks;
        
        checkpoint = false;
        winner = false;
        
        currentFrame = 0;
        rotationDegrees = 0;
        speed = 0;
        direction = glm::vec3(0, 0, -1);
        this->position = position;
        this->radius = radius;

        this->stadium = stadium;

        cameraBack = -1;
        cam.set(position + glm::vec3(0, 1, 0) + cameraBack * direction, position + glm::vec3(0, 1, 0) + direction, glm::vec3(0, 1, 0));
	}

	~Runner()
    {
        for(auto it: vao)
        {
            glDeleteVertexArrays(1, &it);

        }

        for(auto it: vbo)
        {
		    glDeleteBuffers(1, &it);
        }

        for(auto it: ibo)
        {
		    glDeleteBuffers(1, &it);
        }
	}

    static void loadModel(std::string path, int count)
    {
        vao.resize(count);
        vbo.resize(count);
        ibo.resize(count);
        mesh_num_indices.resize(count);
        for(int i = 0; i < count; i++)
        {
            lab::loadObj(path + std::to_string(i) + ".obj", vao[i], vbo[i], ibo[i], mesh_num_indices[i]);
        }
    }

    void setAttributes(float speed, float maxSpeed, float referenceSpeed,
                       float acceleration, float deceleration, float rotationSpeed,
                       float rotationDegrees)
    {
        this->speed = speed;
        this->maxSpeed = maxSpeed;
        this->referenceSpeed = referenceSpeed;
        this->acceleration = acceleration;
        this->deceleration = deceleration;
        this->rotationSpeed = rotationSpeed;
        this->rotationDegrees = rotationDegrees;
    }

public:
	//deseneaza stadion
	void draw()
    {
         glm::mat4 model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
         
         model_matrix = glm::translate(model_matrix, position);
         model_matrix = glm::rotate(model_matrix, 180.f, glm::vec3(0, 1, 0));
         model_matrix = glm::rotate(model_matrix, rotationDegrees, glm::vec3(0, 1, 0));
         glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "model_matrix"),1,false,glm::value_ptr(model_matrix));

        // deseneaza semicercuri
        glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_kd"), material_kd.x, material_kd.y, material_kd.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_ks"), material_ks.x, material_ks.y, material_ks.z);


		glBindVertexArray(vao[currentFrame/10]);
		glDrawElements(GL_TRIANGLES, mesh_num_indices[currentFrame/10], GL_UNSIGNED_INT, 0);
    }


    void setAutopilot(float speed)
    {
        this->speed = speed;
    }
    
    void autopilot()
    { 
        if(position.z >= stadium->centruSemicercSus.z &&
           position.z <= stadium->centruSemicercJos.z)
        {
            if(position.x > 0)
            {
                direction = glm::vec3(0, 0, -1);
            }
            else
            {
                direction = glm::vec3(0, 0, 1);
            }
        }
        else if(position.z <= stadium->centruSemicercSus.z)
        {
            glm::vec3 tangent = glm::normalize(position - stadium->centruSemicercSus);
            direction = glm::rotateY(tangent, 90.f);
            rotationDegrees =  glm::degrees(glm::acos(glm::dot(tangent, glm::vec3(1, 0, 0)) / 
                                          (glm::length(tangent) * glm::length(glm::vec3(1, 0, 0)))));
        }
        else if(position.z >= stadium->centruSemicercJos.z)
        {
            glm::vec3 tangent = glm::normalize(position - stadium->centruSemicercJos);
            direction = glm::rotateY(tangent, 90.f);
            rotationDegrees = 180 + glm::degrees(glm::acos(glm::dot(glm::vec3(-1, 0, 0), tangent) / 
                                         (glm::length(tangent) * glm::length(glm::vec3(-1, 0, 0)))));
        }

        position = position + direction * speed;
        cam.set(position + glm::vec3(0, 1, 0) + cameraBack * direction, position + glm::vec3(0, 1, 0) + direction, glm::vec3(0, 1, 0));
        
        currentFrame++;
        currentFrame %= 10*vao.size(); // fac asta ca se misca prea repede (si impart la 10 cand afisez, ca sa nu depaseasca size-ul)
    }
    

    void accelerate()
    {
        glm::vec3 old_position = position;
        if(speed < maxSpeed)
        {
            speed += acceleration;
        }
        position = position + direction * speed;
        if(!isValidMove())
        {
            position = old_position;
        }
        else
        {
            cam.translateForward(speed);
        }
        currentFrame++;
        currentFrame %= 10*vao.size();
    }

    void decelerate()
    {
        glm::vec3 old_position = position;
        speed -= deceleration;
        if(speed <= 0)
        {
            speed = 0;
            currentFrame = 0;
        }
        else
        {
            position = position + direction * speed;
            currentFrame++;
            currentFrame %= 10*vao.size();
            
            if(!isValidMove())
            {
                position = old_position;
            }
            else
            {
                 cam.set(position + glm::vec3(0, 1, 0) + cameraBack * direction, position + glm::vec3(0, 1, 0) + direction, glm::vec3(0, 1, 0));
            }
        }
    }

    void rotate_left()
    {
        rotationDegrees += rotationSpeed;
        direction = glm::rotateY(glm::vec3(0, 0, -1), rotationDegrees);
        
        cam.set(position + glm::vec3(0, 1, 0) + cameraBack * direction, position + glm::vec3(0, 1, 0) + direction, glm::vec3(0, 1, 0));
    }

    void rotate_right()
    {
        rotationDegrees -= rotationSpeed;
        direction = glm::rotateY(glm::vec3(0, 0, -1), rotationDegrees);
        
        cam.set(position + glm::vec3(0, 1, 0) + cameraBack * direction, position + glm::vec3(0, 1, 0) + direction, glm::vec3(0, 1, 0));
    }

    // verific sa nu ies in afara (daca ies o sa anulez ultima mutare)
    bool isValidMove()
    {
        if(position.z >= stadium->centruSemicercSus.z &&
           position.z <= stadium->centruSemicercJos.z)
        {
            if(position.x > 0 && position.x < stadium->razaSemicerc)
            {
                return false;
            }
            else if(position.x < 0 && position.x > -stadium->razaSemicerc)
            {
                return false;
            }
            else if(position.x > 0 && position.x > stadium->razaUltimCuloar)
            {
                return false;
            }
            else if(position.x < 0 && position.x < -stadium->razaUltimCuloar)
            {
                return false;
            }
  
        }
        else if(position.z <= stadium->centruSemicercSus.z)
        {
            if(glm::distance(stadium->centruSemicercSus, position) < stadium->razaSemicerc)
            {
                return false;
            }
            else if(glm::distance(stadium->centruSemicercSus, position) > stadium->razaUltimCuloar)
            {
                return false;
            }
        }
        else if(position.z >= stadium->centruSemicercJos.z)
        {
            if(glm::distance(stadium->centruSemicercJos, position) < stadium->razaSemicerc)
            {
                return false;
            }
            else if(glm::distance(stadium->centruSemicercJos, position) > stadium->razaUltimCuloar)
            {
                return false;
            }
        }

        return true;
    }

    bool isWinner()
    {
        return winner;
    }

public:

    lab::Camera cam;
    float cameraBack;

	static std::vector<unsigned int> vao, vbo, ibo, mesh_num_indices;

    glm::vec3 material_kd;
	glm::vec3 material_ks;

    glm::vec3 direction;
    glm::vec3 position;
    float speed;
    float maxSpeed;
    float referenceSpeed; // calculez frameRateul in functie de ea
    float acceleration;
    float deceleration;
    float rotationSpeed;
    float rotationDegrees;
    float radius; // pentru coliziuni - il dam manual
    float offset; // distanta ca sa stea pe sol

    int currentFrame;

    bool checkpoint; // hack pentru a impiedica jucatorul sa se intoarca imediat inapoi si sa castige
    bool winner;

    Stadium* stadium;

    unsigned int gl_program_shader_curent;
};

std::vector<unsigned int> Runner::vao = std::vector<unsigned int>();
std::vector<unsigned int> Runner::vbo = std::vector<unsigned int>();
std::vector<unsigned int> Runner::ibo = std::vector<unsigned int>();
std::vector<unsigned int> Runner::mesh_num_indices = std::vector<unsigned int>();