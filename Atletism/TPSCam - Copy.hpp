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
#include "Runner.hpp"
#include "Stadium.hpp"

#include <vector>
#include <string>
#include <iostream>

class TPSCam
{
public:
	TPSCam(glm::vec3 material_kd,  glm::vec3 material_ks,
           Runner* runner, Stadium* stadium,
           std::string path,
           unsigned int gl_program_shader)
    {
        gl_program_shader_curent = gl_program_shader;

        this->material_kd = material_kd;
        this->material_ks = material_ks;
        this->runner = runner;
        this->stadium = stadium;

        cameraBack = 1;

        // punem camera la marginea stadionului - initial e in linie cu linia de start
        model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
        model_matrix = glm::translate(model_matrix, glm::vec3(stadium->razaMargineCuloar + cameraBack, 1, 0));
        cam.set(glm::vec3(stadium->razaMargineCuloar, 1, 0), runner->position, glm::vec3(0, 1, 0));

        // incarcam modelul de camera
        loadModel(path);
	}

	~TPSCam()
    {
        glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
	}

    void loadModel(std::string path)
    {
        lab::loadObj(path, vao, vbo, ibo, mesh_num_indices);
    }

public:
	//deseneaza stadion
	void draw()
    {
        glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "model_matrix"),1,false,glm::value_ptr(model_matrix));
        glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_kd"), material_kd.x, material_kd.y, material_kd.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_ks"), material_ks.x, material_ks.y, material_ks.z);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, mesh_num_indices, GL_UNSIGNED_INT, 0);
    }

    void  update()
    {
        std::cout << "Camera  poz " << cam.position.x << " " << cam.position.y << " " << (int)cam.position.z << std::endl; 

        if(runner->position.z >= stadium->centruSemicercSus.z &&
           runner->position.z <= stadium->centruSemicercJos.z)
        {
            cam.translateRight(cam.position.z - runner->position.z);
            model_matrix = glm::translate(model_matrix, glm::vec3(0, 0, cam.position.z - runner->position.z));
        }
        else if(runner->position.z <= stadium->centruSemicercSus.z)
        {
            glm::vec3 camPositionXoZ = cam.position - glm::vec3(0, 1, 0);
            float angleRunnerCamera = glm::acos(glm::dot(runner->position, camPositionXoZ) / 
                                                         (glm::length(runner->position) * glm::length(camPositionXoZ)));
            cam.rotateTPSoY(angleRunnerCamera, glm::distance(camPositionXoZ, stadium->centruSemicercSus));


            model_matrix = glm::rotate(model_matrix, angleRunnerCamera, glm::vec3(0, 1, 0));
        }
        else if(runner->position.z >= stadium->centruSemicercJos.z)
        {
            glm::vec3 camPositionXoZ = cam.position - glm::vec3(0, 1, 0);
            float angleRunnerCamera = glm::acos(glm::dot(runner->position, camPositionXoZ) / 
                                                         (glm::length(runner->position) * glm::length(camPositionXoZ)));
            cam.rotateTPSoY(angleRunnerCamera, glm::distance(camPositionXoZ, stadium->centruSemicercJos));


            model_matrix = glm::rotate(model_matrix, angleRunnerCamera, glm::vec3(0, 1, 0));
        }
    }

public:

    lab::Camera cam;
    float cameraBack;

    unsigned int vao, vbo, ibo, mesh_num_indices;
    unsigned int gl_program_shader_curent;

    glm::vec3 material_kd;
	glm::vec3 material_ks;

    Runner* runner;
    Stadium* stadium;

    glm::mat4 model_matrix;
};
