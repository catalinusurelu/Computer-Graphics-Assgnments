#pragma once
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtx\rotate_vector.hpp"
#include "dependente\freeglut\freeglut.h"
#include "dependente\glew\glew.h"
#include "dependente\glm\glm.hpp"

#include "VertexFormat.h"
#include "Colors.h"

#include <vector>

class Stadium
{
public:
    Stadium()
    {
    }

	Stadium(float razaSemicerc, float trackWidth, float outeTrackWidth,
            float stripeWidth, float distCentruSemicerc,
            unsigned int gl_program_shader)
    {
        gl_program_shader_curent = gl_program_shader;

        semicerc_material_kd = BROWN;
        semicerc_material_ks = BROWN;

        field_material_kd = GREEN;
        field_material_ks = GREEN;

        stripe_material_kd= WHITE;
	    stripe_material_ks= WHITE;

        track_material_kd = BROWN;
        track_material_ks = BROWN;

        stage_material_kd = GREY;
        stage_material_ks = GREY;

        this-> razaSemicerc = razaSemicerc;
        this-> trackWidth = trackWidth;
        this-> stripeWidth = stripeWidth;

		createSemicercSus(razaSemicerc, distCentruSemicerc);
        createSemicercJos(razaSemicerc, distCentruSemicerc);
        createField(razaSemicerc);

        // creez stripe-uri
        for(int i = 1; i <= 4; i++)
        {
            createCircularMesh(vao_stripes[i - 1], vbo_stripes[i - 1], ibo_stripes[i - 1], mesh_num_indices_stripe[i - 1],
                               razaSemicerc + i * stripeWidth + (i - 1) * trackWidth,
                               razaSemicerc + (i - 1) * stripeWidth + (i - 1) * trackWidth,
                               distCentruSemicerc, 0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
        }

        razaUltimCuloar = razaSemicerc + 4 * stripeWidth + 3 * trackWidth;

        // creez culoare
        for(int i = 1; i <= 3; i++)
        {
            createCircularMesh(vao_track[i - 1], vbo_track[i - 1], ibo_track[i - 1], mesh_num_indices_track[i - 1],
                               razaSemicerc + i * stripeWidth + i * trackWidth, 
                               razaSemicerc + i * stripeWidth + (i - 1) * trackWidth,
                               distCentruSemicerc, 0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
        }

        createCircularMesh(vao_track[3], vbo_track[3], ibo_track[3], mesh_num_indices_track[3],
                    razaSemicerc + 4 * stripeWidth + 3 * trackWidth + outeTrackWidth, 
                    razaSemicerc + 4 * stripeWidth + 3 * trackWidth,
                    distCentruSemicerc, 0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

        razaMargineCuloar = razaSemicerc + 4 * stripeWidth + 3 * trackWidth + outeTrackWidth;

        // gasesc mijlocul culoarului pentru fiecare jucator (ne intereseaza doar coordonata x)
        midTrack[0] = razaSemicerc + stripeWidth + trackWidth / 2;
        midTrack[1] = razaSemicerc + 2 * stripeWidth + trackWidth + trackWidth / 2;
        midTrack[2] = razaSemicerc + 3 * stripeWidth + 2 * trackWidth + trackWidth / 2;

        vao_stage.resize(4);
        vbo_stage.resize(4);
        ibo_stage.resize(4);
        mesh_num_indices_stage.resize(4);

        for(int i = 0; i < 4; i++)
        {
            createCircularMesh(vao_stage[i], vbo_stage[i], ibo_stage[i], mesh_num_indices_stage[i],
                    razaSemicerc + 4 * stripeWidth + 3 * trackWidth + outeTrackWidth + (i + 1) * 2, 
                    razaSemicerc + 4 * stripeWidth + 3 * trackWidth + outeTrackWidth + i * 2,
                    distCentruSemicerc, i * 1, glm::vec3(1, 1, 1), glm::vec3(-1, 1, -1));
        }

        // deseneaza linie start
        createStartLine();

	}

	~Stadium()
    {
        // sterge semcierc sus
        glDeleteVertexArrays(1,&vao_semicerc_sus);
		glDeleteBuffers(1,&vbo_semicerc_sus);
		glDeleteBuffers(1,&ibo_semicerc_sus);

        // sterge semcierc jos
        glDeleteVertexArrays(1,&vao_semicerc_jos);
		glDeleteBuffers(1,&vbo_semicerc_jos);
		glDeleteBuffers(1,&ibo_semicerc_jos);

        // sterge camp
        glDeleteVertexArrays(1,&vao_field);
		glDeleteBuffers(1,&vbo_field);
		glDeleteBuffers(1,&ibo_field);

        // sterge culoare
        for(int i = 0; i < 4; i++)
        {
            glDeleteVertexArrays(1,&vao_track[i]);
		    glDeleteBuffers(1,&vbo_track[i]);
		    glDeleteBuffers(1,&ibo_track[i]);
        }

        // sterge linii albe
        for(int i = 0; i < 4; i++)
        {
            glDeleteVertexArrays(1,&vao_stripes[i]);
		    glDeleteBuffers(1,&vbo_stripes[i]);
		    glDeleteBuffers(1,&ibo_stripes[i]);
        }

        // sterge linie de start
        glDeleteVertexArrays(1,&vao_start);
		glDeleteBuffers(1,&vbo_start);
		glDeleteBuffers(1,&ibo_start);

        // sterge tribune
        for(auto it: vao_stage)
        {
            glDeleteVertexArrays(1, &it);
        }

        for(auto it: vbo_stage)
        {
		    glDeleteBuffers(1, &it);
        }

        for(auto it: ibo_stage)
        {
		    glDeleteBuffers(1, &it);
        }
	}

private:

    void createSemicercSus(const int& r, const int& offset)
    {
        mesh_num_indices_semicerc_sus = 0;
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        VertexFormat centruSemicercSusVertex = VertexFormat(0, 0, -offset, 0, 1, 0);
        centruSemicercSus = glm::vec3(0, 0, -offset);

        float angleDegrees = 1;
      
        vertices.push_back(centruSemicercSusVertex);
        glm::vec3 semicercPoint = glm::vec3(r, 0, 0);
        glm::vec3 p = (semicercPoint + centruSemicercSus);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, 0, 1, 0));
       
        for(float i = 0; ceil(i) < 180; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercSus);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, 0, 1, 0));
        }

        for(int i = 1; i + 1 < vertices.size(); i++)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
            mesh_num_indices_semicerc_sus += 3;
        }

        glGenVertexArrays(1, &vao_semicerc_sus);
		glBindVertexArray(vao_semicerc_sus);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1,&vbo_semicerc_sus);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_semicerc_sus);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&ibo_semicerc_sus);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_semicerc_sus);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
    }

    void createSemicercJos(const int& r, const int& offset)
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        VertexFormat centruSemicercJosVertex = VertexFormat(0, 0, offset, 0, 1, 0);
        centruSemicercJos = glm::vec3(0, 0, offset);

        mesh_num_indices_semicerc_jos = 0;
 
        glm::vec3 semicercPoint = glm::vec3(-r, 0, 0);
        vertices.push_back(centruSemicercJosVertex);
        glm::vec3 p = (semicercPoint + centruSemicercJos);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, 0, 1, 0));

        float angleDegrees = 1;
       
        for(float i = 180; i < 360; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercJos);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, 0, 1, 0));
        }

        for(int i = 1; i + 1 < vertices.size(); i++)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
            mesh_num_indices_semicerc_jos += 3;
        }


        glGenVertexArrays(1, &vao_semicerc_jos);
		glBindVertexArray(vao_semicerc_jos);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1,&vbo_semicerc_jos);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_semicerc_jos);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&ibo_semicerc_jos);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_semicerc_jos);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
    }

    void createField(const int& r)
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        glm::vec3 stSus = centruSemicercSus + glm::vec3(-r, 0, 0);
        glm::vec3 drSus = centruSemicercSus + glm::vec3(r, 0, 0);
        glm::vec3 stJos = centruSemicercJos + glm::vec3(-r, 0, 0);
        glm::vec3 drJos = centruSemicercJos + glm::vec3(r, 0, 0);


        vertices.push_back(VertexFormat(stSus.x, stSus.y, stSus.z, 0, 1, 0));
        vertices.push_back(VertexFormat(drSus.x, drSus.y, drSus.z, 0, 1, 0));
        vertices.push_back(VertexFormat(stJos.x, stJos.y, stJos.z, 0, 1, 0));
        vertices.push_back(VertexFormat(drJos.x, drJos.y, drJos.z, 0, 1, 0));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);

        mesh_num_indices_field = 6;

        glGenVertexArrays(1, &vao_field);
		glBindVertexArray(vao_field);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1, &vbo_field);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_field);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&ibo_field);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_field);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
    }

    void createCircularMesh(unsigned int& vao, unsigned int& vbo,
                            unsigned int& ibo, unsigned int& mesh_num_indices,
                            const float& razaLeft, const float& razaRight, 
                            const float& offset, const float& height,
                            glm::vec3& normalLeft, glm::vec3& normalRight)
    {
        const int width = razaLeft - razaRight;

        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        float angleDegrees = 2;

        //partea de jos a meshului

        //partea stanga a mesh-ului
        glm::vec3 centruSemicercSus = glm::vec3(0, 0, -offset);
        glm::vec3 semicercPoint = glm::vec3(razaLeft, 0, 0);
        glm::vec3 p = (semicercPoint + centruSemicercSus);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, normalLeft.x, normalLeft.y, normalLeft.z));

        for(float i = 0; i < 180; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercSus);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, normalLeft.x, normalLeft.y, normalLeft.z));
        }

        semicercPoint = glm::vec3(-razaLeft, 0, 0);
        glm::vec3 centruSemicercJos = glm::vec3(0, 0, offset);
        p = (semicercPoint + centruSemicercJos);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, normalLeft.x, normalLeft.y, normalLeft.z));
       
        for(float i = 180; i < 360; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercJos);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, normalLeft.x, normalLeft.y, normalLeft.z));
        }
        
        //partea dreapta a mesh-ului
        centruSemicercSus = glm::vec3(0, 0, -offset);
        semicercPoint = glm::vec3(razaRight, 0, 0);
        p = (semicercPoint + centruSemicercSus);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, normalRight.x, normalRight.y, normalRight.z));

        for(float i = 0; i < 180; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercSus);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, normalRight.x, normalRight.y, normalRight.z));
        }

        centruSemicercJos = glm::vec3(0, 0, offset);
        semicercPoint = glm::vec3(-razaRight, 0, 0);
        p = (semicercPoint + centruSemicercJos);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, normalRight.x, normalRight.y, normalRight.z));
       
        for(float i = 180; i < 360; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercJos);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, normalRight.x, normalRight.y, normalRight.z));
        }
        
        // partea de sus a meshului

        //partea stanga a mesh-ului
        centruSemicercSus = glm::vec3(0, height, -offset);
        semicercPoint = glm::vec3(razaLeft, height, 0);
        p = (semicercPoint + centruSemicercSus);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, normalLeft.x, normalLeft.y, normalLeft.z));

        for(float i = 0; i < 180; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercSus);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, normalLeft.x, normalLeft.y, normalLeft.z));
        }

        centruSemicercJos = glm::vec3(0, height, offset);
        semicercPoint = glm::vec3(-razaLeft, height, 0);
        p = (semicercPoint + centruSemicercJos);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, normalLeft.x, normalLeft.y, normalLeft.z));
       
        for(float i = 180; i < 360; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercJos);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, normalLeft.x, normalLeft.y, normalLeft.z));
        }
        
        //partea dreapta a mesh-ului
        centruSemicercSus = glm::vec3(0, height, -offset);
        semicercPoint = glm::vec3(razaRight, height, 0);
        p = (semicercPoint + centruSemicercSus);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, normalRight.x, normalRight.y, normalRight.z));

        for(float i = 0; i < 180; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercSus);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, normalRight.x, normalRight.y, normalRight.z));
        }

        centruSemicercJos = glm::vec3(0, height, offset);
        semicercPoint = glm::vec3(-razaRight, height, 0);
        p = (semicercPoint + centruSemicercJos);
        vertices.push_back(VertexFormat(p.x, p.y, p.z, normalRight.x, normalRight.y, normalRight.z));
       
        for(float i = 180; i < 360; i += angleDegrees)
        {
            semicercPoint = glm::rotateY(semicercPoint, angleDegrees);
            glm::vec3 p = (semicercPoint + centruSemicercJos);
            vertices.push_back(VertexFormat(p.x, p.y, p.z, normalRight.x, normalRight.y, normalRight.z));
        }


        mesh_num_indices = 0;
        // sunt 4 "linii"
        int line = vertices.size() / 4;
        for(int i = 0; i < line; i++)
        {
            // fatza stanga
            indices.push_back(i);
            indices.push_back(increment(i, 0, line));
            indices.push_back(i + 2 * line);

            indices.push_back(i + 2 * line);
            indices.push_back(increment(i + 2 * line, 2 * line, 3 * line));
            indices.push_back(increment(i, 0, line));

             // fatza dreapta
            indices.push_back(i + line);
            indices.push_back(increment(i + line, line, 2 * line));
            indices.push_back(i + 3 * line);

            indices.push_back(i + 3 * line);
            indices.push_back(increment(i + 3 * line, 3 * line, 4 * line));
            indices.push_back(increment(i + line, line, 2 * line));

            // fatza sus
            indices.push_back(i + 2 * line);
            indices.push_back(i + 3 * line);
            indices.push_back(increment(i + 3 * line, 3 * line, 4 * line));

            indices.push_back(increment(i + 3 * line, 3 * line, 4 * line));
            indices.push_back(increment(i + 2 * line, 2 * line, 3 * line));
            indices.push_back(i + 2 * line);

            // fatza jos
            indices.push_back(i);
            indices.push_back(i + line);
            indices.push_back(increment(i + line, line, 2 * line));

            indices.push_back(increment(i + line, line, 2 * line));
            indices.push_back(increment(i, 0, line));
            indices.push_back(i);

             mesh_num_indices += 24;
        }

        glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1,&vbo);
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

    int increment(const int& x, const int& start, const int& finish)
    {
        if(x + 1 == finish)
        {
            return start;
        }
        
        return x + 1;
    }

    void createStartLine()
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        glm::vec3 stSus = glm::vec3(razaSemicerc, 0.01f, -stripeWidth);
        glm::vec3 drSus = glm::vec3(razaUltimCuloar, 0.01f, -stripeWidth);
        glm::vec3 stJos = glm::vec3(razaSemicerc, 0.01f, 0);
        glm::vec3 drJos = glm::vec3(razaUltimCuloar, 0.01f, 0);


        vertices.push_back(VertexFormat(stSus.x, stSus.y, stSus.z, 0, 1, 0));
        vertices.push_back(VertexFormat(drSus.x, drSus.y, drSus.z, 0, 1, 0));
        vertices.push_back(VertexFormat(stJos.x, stJos.y, stJos.z, 0, 1, 0));
        vertices.push_back(VertexFormat(drJos.x, drJos.y, drJos.z, 0, 1, 0));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);

        mesh_num_inidices_start = 6;

        glGenVertexArrays(1, &vao_start);
		glBindVertexArray(vao_start);
		
		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1, &vbo_start);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_start);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&ibo_start);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_start);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
    }

public:
	//deseneaza stadion
	void draw()
    {
        // deseneaza semicercuri
        glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_kd"), semicerc_material_kd.x, semicerc_material_kd.y, semicerc_material_kd.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_ks"), semicerc_material_ks.x, semicerc_material_ks.y, semicerc_material_ks.z);

		glBindVertexArray(vao_semicerc_sus);
		glDrawElements(GL_TRIANGLES, mesh_num_indices_semicerc_sus, GL_UNSIGNED_INT, 0);

        glBindVertexArray(vao_semicerc_jos);
		glDrawElements(GL_TRIANGLES, mesh_num_indices_semicerc_jos, GL_UNSIGNED_INT, 0);
        
        // deseneaza camp
        glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_kd"), field_material_kd.x, field_material_kd.y, field_material_kd.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_ks"), field_material_ks.x, field_material_ks.y, field_material_ks.z);

        glBindVertexArray(vao_field);
		glDrawElements(GL_TRIANGLES, mesh_num_indices_field, GL_UNSIGNED_INT, 0);

        // deseneaza stripe-uri
        glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_kd"), stripe_material_kd.x, stripe_material_kd.y, stripe_material_kd.z);
	    glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_ks"), stripe_material_ks.x, stripe_material_ks.y, stripe_material_ks.z);
	
        for(int i = 0; i < 4; i++)
        {
            glBindVertexArray(vao_stripes[i]);
            glDrawElements(GL_TRIANGLES, mesh_num_indices_stripe[i], GL_UNSIGNED_INT, 0);
        }

        // deseneaza linie start
        glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_kd"), stripe_material_kd.x, stripe_material_kd.y, stripe_material_kd.z);
	    glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_ks"), stripe_material_ks.x, stripe_material_ks.y, stripe_material_ks.z);
	
        glBindVertexArray(vao_start);
        glDrawElements(GL_TRIANGLES, mesh_num_inidices_start, GL_UNSIGNED_INT, 0);

        // deseneaza trackuri
        glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_kd"), track_material_kd.x, track_material_kd.y, track_material_kd.z);
	    glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_ks"), track_material_ks.x, track_material_ks.y, track_material_ks.z);

        for(int i = 0; i < 4; i++)
        {
            glBindVertexArray(vao_track[i]);
            glDrawElements(GL_TRIANGLES, mesh_num_indices_track[i], GL_UNSIGNED_INT, 0);
        }

        //deseneaza tribuna
        glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_kd"), stage_material_kd.x, stage_material_kd.y, stage_material_kd.z);
	    glUniform3f(glGetUniformLocation(gl_program_shader_curent, "material_ks"), stage_material_ks.x, stage_material_ks.y, stage_material_ks.z);

        for(int i = 0; i < vao_stage.size(); i++)
        {
            glBindVertexArray(vao_stage[i]);
            glDrawElements(GL_TRIANGLES, mesh_num_indices_stage[i], GL_UNSIGNED_INT, 0);
        }
    }

private:

	//semicerc sus
	unsigned int vao_semicerc_sus, vbo_semicerc_sus, ibo_semicerc_sus, mesh_num_indices_semicerc_sus;

    //semicerc jos
	unsigned int vao_semicerc_jos, vbo_semicerc_jos, ibo_semicerc_jos, mesh_num_indices_semicerc_jos;

    // teren mijloc
    unsigned int vao_field, vbo_field, ibo_field, mesh_num_indices_field;

    // culoare de alergat
    unsigned int vao_track[4], vbo_track[4], ibo_track[4], mesh_num_indices_track[4];

    // white stripes
    unsigned int vao_stripes[4], vbo_stripes[4], ibo_stripes[4], mesh_num_indices_stripe[4];

    // linie start
    unsigned int vao_start, vbo_start, ibo_start, mesh_num_inidices_start;

    // tribune
    std::vector<unsigned int> vao_stage, vbo_stage, ibo_stage, mesh_num_indices_stage;

public:

    float midTrack[3];
    
    glm::vec3 centruSemicercSus;
    glm::vec3 centruSemicercJos;

    float razaSemicerc;
    float trackWidth;
    float outeTrackWidth;
    float stripeWidth;
    float razaMargineCuloar;
    float razaUltimCuloar;

private:

    // culori linii
    glm::vec3 stripe_material_kd;
	glm::vec3 stripe_material_ks;

    // culori camp
    glm::vec3 field_material_kd;
	glm::vec3 field_material_ks;

    // culori culoare
    glm::vec3 track_material_kd;
	glm::vec3 track_material_ks;

    // culori semicercuri
    glm::vec3 semicerc_material_kd;
	glm::vec3 semicerc_material_ks;

    // culori tribune
    glm::vec3 stage_material_kd;
	glm::vec3 stage_material_ks;

    unsigned int gl_program_shader_curent;
};
