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

#include <vector>
#include <string>

class Sky
{
public:
	Sky(glm::vec3 material_kd,  glm::vec3 material_ks,
        const float& height, const float& width,
        unsigned int gl_program_shader)
    {
        gl_program_shader_curent = gl_program_shader;

        this->material_kd = material_kd;
        this->material_ks = material_ks;

        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        glm::vec3 stSus = glm::vec3(- width / 2, height, - width / 2);
        glm::vec3 drSus = glm::vec3(width / 2, height, - width / 2);
        glm::vec3 stJos = glm::vec3(- width / 2, height, width / 2);
        glm::vec3 drJos = glm::vec3(width / 2, height, width / 2);


        vertices.push_back(VertexFormat(stSus.x, stSus.y, stSus.z, 0, -1, 0));
        vertices.push_back(VertexFormat(drSus.x, drSus.y, drSus.z, 0, -1, 0));
        vertices.push_back(VertexFormat(stJos.x, stJos.y, stJos.z, 0, -1, 0));
        vertices.push_back(VertexFormat(drJos.x, drJos.y, drJos.z, 0, -1, 0));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);

        mesh_num_indices = 6;

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
	}

	~Sky()
    {
        glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
	}


public:
	//deseneaza stadion
	void draw()
    {
        glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_kd"), material_kd.x, material_kd.y, material_kd.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_ks"), material_ks.x, material_ks.y, material_ks.z);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, mesh_num_indices, GL_UNSIGNED_INT, 0);
    }

public:
	unsigned int vao, vbo, ibo, mesh_num_indices;

    glm::vec3 material_kd;
	glm::vec3 material_ks;

    unsigned int gl_program_shader_curent;
};
