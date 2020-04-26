#ifndef BOX_H
#define BOX_H

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include "ShaderProgram.h"

using namespace std;
using namespace glm;

class Box{
    
public: // it would be better to have some kind of protection on members...
	unsigned int num;
    
	vec3 minPos ;
	vec3 maxPos;
    vec3 color;
    
    // rotation matrices
    mat4 rotMat;
    mat4 invRotMat;
    
	// material
	float ambient;
    float diffuse;
	float phong;

	ShaderProgram shader;
	GLuint vao, vbo;
    
public:
    Box()
	{
		/*float vertices[] = {
			// Top face (y = 1.0f)
			// Counter-clockwise (CCW) with normal pointing out
			maxPos.x, maxPos.y, minPos.z,
			minPos.x, maxPos.y, minPos.z,
			minPos.x, maxPos.y, maxPos.z,
			maxPos.x, maxPos.y, maxPos.z,

			// Bottom face (y = -1.0f)
			maxPos.x, minPos.y, maxPos.z,
			minPos.x, minPos.y, maxPos.z,
			minPos.x, minPos.y, minPos.z,
			maxPos.x, minPos.y, minPos.z,

			// Front face  (z = 1.0f)
			maxPos.x, maxPos.y, maxPos.z,
			minPos.x, maxPos.y, maxPos.z,
			minPos.x, minPos.y, maxPos.z,
			maxPos.x, minPos.y, maxPos.z,

			// Back face (z = -1.0f)
			maxPos.x, minPos.y, minPos.z,
			minPos.x, minPos.y, minPos.z,
			minPos.x, maxPos.y, minPos.z,
			maxPos.x, maxPos.y, minPos.z,

			// Left face (x = -1.0f)
			minPos.x, maxPos.y, maxPos.z,
			minPos.x, maxPos.y, minPos.z,
			minPos.x, minPos.y, minPos.z,
			minPos.x, minPos.y, maxPos.z,

			// Right face (x = 1.0f)
			maxPos.x, maxPos.y, minPos.z,
			maxPos.x, maxPos.y, maxPos.z,
			maxPos.x, minPos.y, maxPos.z,
			maxPos.x, minPos.y, minPos.z
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);*/
	};

    ~Box(){};

    void Draw()
    {
		//glBindVertexArray(vao);

		//glUseProgram(shader.id);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf((const float*)glm::value_ptr(rotMat));

		glDrawArrays(GL_QUADS, 0, 24);
		glutSwapBuffers();
        
        glColor3f(color.x, color.y, color.z);

        glBegin(GL_QUADS);
        
        // Top face (y = 1.0f)
        // Counter-clockwise (CCW) with normal pointing out
        glVertex3f(maxPos.x, maxPos.y, minPos.z);
        glVertex3f(minPos.x, maxPos.y, minPos.z);
		glVertex3f(minPos.x, maxPos.y, maxPos.z);
        glVertex3f(maxPos.x, maxPos.y, maxPos.z);
        
		// Bottom face (y = -1.0f)
        glVertex3f(maxPos.x, minPos.y, maxPos.z);
        glVertex3f(minPos.x, minPos.y, maxPos.z);
        glVertex3f(minPos.x, minPos.y, minPos.z);
        glVertex3f(maxPos.x, minPos.y, minPos.z);
        
        // Front face  (z = 1.0f)
        glVertex3f(maxPos.x, maxPos.y, maxPos.z);
        glVertex3f(minPos.x, maxPos.y, maxPos.z);
        glVertex3f(minPos.x, minPos.y, maxPos.z);
        glVertex3f(maxPos.x, minPos.y, maxPos.z);
        
        // Back face (z = -1.0f)
        glVertex3f(maxPos.x, minPos.y, minPos.z);
        glVertex3f(minPos.x, minPos.y, minPos.z);
        glVertex3f(minPos.x, maxPos.y, minPos.z);
        glVertex3f(maxPos.x, maxPos.y, minPos.z);
        
        // Left face (x = -1.0f)
        glVertex3f(minPos.x, maxPos.y, maxPos.z);
        glVertex3f(minPos.x, maxPos.y, minPos.z);
        glVertex3f(minPos.x, minPos.y, minPos.z);
        glVertex3f(minPos.x, minPos.y, maxPos.z);
        
        // Right face (x = 1.0f)
        glVertex3f(maxPos.x, maxPos.y, minPos.z);
        glVertex3f(maxPos.x, maxPos.y, maxPos.z);
        glVertex3f(maxPos.x, minPos.y, maxPos.z);
        glVertex3f(maxPos.x, minPos.y, minPos.z);
        glEnd(); 
 
        glPopMatrix();
    };
    
};

#endif