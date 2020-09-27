#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "ShaderClass.h"
using namespace std;

extern vector<GLuint> shaderProg;



	GLuint createProgram();
	void linkShader(GLuint a_id, GLuint a_sid, GLenum targetType);
	
	void useProgram(GLuint a_id);

	//GLuint getAttributeLoc(const GLcharARB* name); // this is called beforre glBegin() ..
	//void setAttribute4f(const GLuint loc, const float* floatPtr); // this is called between glBegin() and glEnd()..
	//void setAttribute4i(const GLuint loc, const int* intPtr); // this is called between glBegin() and glEnd()..
	//void setAttributeMat4(const GLuint loc, const float* floatPtr); // must not trust this func !!!!


	//functions to set uniform variables - after use this shader program 
	void setFloat(GLuint a_id, const char* name, float value);
	void setInt(GLuint a_id, const char* name, int value);
	void setFloat1V(GLuint a_id, const char* name, unsigned int count, const float* floatPtr);
	void setFloat3V(GLuint a_id, const char* name, unsigned int count, const float* floatPtr);
	void setMatrix4fv(GLuint a_id, const char* name, unsigned int count, const float* floatPtr);
	// ... feel free to add more functions to enable the setting of other data types

	// setup shader sampler texturre data 
	void createDataTexture(GLuint a_id, unsigned  int& pGLTexID, float* pData, unsigned int pMaxWidth, unsigned int pMaxHeight);

	void setSampler(GLuint a_id, const char* sampleName, GLuint textureUnit);



	void printProgramInfoLog(unsigned int prog_id);



#endif