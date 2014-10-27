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
#include "Runner.hpp"

#include <time.h>
#include <chrono>

class Flag
{
public:
	Flag(glm::vec3 material_kd,  glm::vec3 material_ks,
         float pole_length, float pole_width,
         float fabric_length, float fabric_width,
         unsigned int gl_program_shader_phong,
         unsigned int gl_program_shader_wave)
    {
        this->gl_program_shader_phong = gl_program_shader_phong;
        this->gl_program_shader_wave = gl_program_shader_wave;

        this->material_kd = material_kd;
        this->material_ks = material_ks;

        this->pole_length = pole_length;
        this->pole_width = pole_width;
        this->fabric_length = fabric_length;
        this->fabric_width = fabric_width;
        
        start = std::chrono::high_resolution_clock::now();

        drawPole();
        drawFabric();
	}

	~Flag()
    {
        glDeleteVertexArrays(1, &pole_vao);
		glDeleteBuffers(1, &pole_vbo);
		glDeleteBuffers(1, &pole_ibo);

        glDeleteVertexArrays(1, &fabric_vao);
		glDeleteBuffers(1, &fabric_vbo);
		glDeleteBuffers(1, &fabric_ibo);
	}

    void drawPole()
    {
        unsigned int indices[] = {
			// spate
			0, 1, 2,
			2, 3, 0,
			// sus
			3, 2, 6,
			6, 7, 3,
			// fata
			7, 6, 5,
			5, 4, 7,
			// jos
			4, 5, 1,
			1, 0, 4,
			// stanga
			4, 0, 3,
			3, 7, 4,
			// dreapta
			1, 5, 6,
			6, 2, 1,
		};

		std::vector<VertexFormat> vertices;
		vertices.push_back(VertexFormat(0, 0, 0));
		vertices.push_back(VertexFormat(pole_width, 0, 0));
		vertices.push_back(VertexFormat(pole_width, pole_width, 0));
		vertices.push_back(VertexFormat(0, pole_width, 0));

		vertices.push_back(VertexFormat(0, 0, pole_length));
		vertices.push_back(VertexFormat(pole_width, 0, pole_length));
		vertices.push_back(VertexFormat(pole_width, pole_width, pole_length));
		vertices.push_back(VertexFormat(0, pole_width, pole_length));

        pole_mesh_num_indices = sizeof(indices);

        glGenVertexArrays(1, &pole_vao);
		glBindVertexArray(pole_vao);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1,&pole_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, pole_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&pole_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pole_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
    }

    void drawFabric()
    {
		std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        fabric_mesh_num_indices = 0;

        float granularity1 = 50;
        float granularity2 = granularity1 * fabric_length / fabric_width;
        for(float i = 0; i < granularity1; i++)
            for(float j = 0; j < granularity2; j++)
            {
                vertices.push_back(VertexFormat(j * fabric_width / granularity2, 0, i * fabric_length / granularity1, 0, 1, 0));
            }

        for(float i = 0; i < granularity1 - 1; i++)
            for(float j = 0; j < granularity2 - 1; j++)
            {
                indices.push_back(i * granularity2 + j);
                indices.push_back((i + 1) * granularity2 + j);
                indices.push_back((i + 1) * granularity2 + j + 1);

                indices.push_back(i * granularity2 + j);
                indices.push_back((i + 1) * granularity2 + j + 1);
                indices.push_back(i * granularity2 + j + 1);

                fabric_mesh_num_indices += 6;
            }

        glGenVertexArrays(1, &fabric_vao);
		glBindVertexArray(fabric_vao);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1,&fabric_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, fabric_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&fabric_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fabric_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
    }

public:

	void draw()
    {
        // desenez pole-ul steagului
        glUseProgram(gl_program_shader_phong);
        
        glm::mat4 model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
        model_matrix = glm::translate(model_matrix, glm::vec3(-pole_width, -pole_width / 2, -pole_width / 2)); // translatez pole-ul in stanga panzei
        glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_phong, "model_matrix"),1,false,glm::value_ptr(model_matrix));
        glUniform3f(glGetUniformLocation(gl_program_shader_phong, "material_kd"), RED.x, RED.y, RED.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_phong, "material_ks"), RED.x, RED.y, RED.z);

        glBindVertexArray(pole_vao);
		glDrawElements(GL_TRIANGLES, pole_mesh_num_indices, GL_UNSIGNED_INT, 0);
       
        // desenez panza steagului
        glUseProgram(gl_program_shader_wave);
        glUniform3f(glGetUniformLocation(gl_program_shader_wave, "material_kd"), material_kd.x, material_kd.y, material_kd.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_wave, "material_ks"), material_ks.x, material_ks.y, material_ks.z);
        glUniform1f(glGetUniformLocation(gl_program_shader_wave, "amplitude"), 2.f);
        glUniform1f(glGetUniformLocation(gl_program_shader_wave, "flag_width"), fabric_width);

        auto ellapsed_time = std::chrono::high_resolution_clock::now() - start;
        int time = std::chrono::duration_cast<std::chrono::milliseconds>(ellapsed_time).count() / 100;
        glUniform1i(glGetUniformLocation(gl_program_shader_wave, "time"), time);

		glBindVertexArray(fabric_vao);
		glDrawElements(GL_TRIANGLES, fabric_mesh_num_indices, GL_UNSIGNED_INT, 0);
    }

public:
    std::chrono::high_resolution_clock::time_point start;

	unsigned int fabric_vao, fabric_vbo, fabric_ibo, fabric_mesh_num_indices;
    unsigned int pole_vao, pole_vbo, pole_ibo, pole_mesh_num_indices;

    glm::vec3 material_kd;
	glm::vec3 material_ks;

    float pole_length,  pole_width;
    float fabric_length,  fabric_width;

    unsigned int gl_program_shader_phong;
    unsigned int gl_program_shader_wave;
};
