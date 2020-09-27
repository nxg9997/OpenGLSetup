#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

extern vector<GLuint> vShader, fShader;





	GLuint createShader(const char* shaderFileName, GLenum targetType);
	void destroyShader(GLuint a_id, GLenum targetType);


	char* loadShaderFile(GLuint a_id, const char* fn); // load the shader text file
	void printShaderInfoLog(unsigned int shader_id); // print the shader info log to the console



#endif