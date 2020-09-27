#include "ShaderProgram.h"
#include <iostream>
#include <GL/glew.h>
using namespace std;

vector<GLuint> shaderProg;

GLuint createProgram()
{
	static GLuint program_id = -1; program_id++;

	shaderProg.push_back(glCreateProgram());

	return program_id;
}

void linkShader(GLuint a_id, GLuint a_sid, GLenum targetType)
{
	glAttachShader(a_id, a_sid);
	glLinkProgram(a_id);

	int status;
	glGetProgramiv(a_id, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) 
	{
		printProgramInfoLog(a_id);
		return;
	}
}

void setInt(GLuint a_id, const char* name, int value)
{
	unsigned int loc = glGetUniformLocation(a_id, name);
	if (loc == -1) {
		return;
		//cout << "Uniform integer: " << name <<"doesn't exist."<< endl;
	}
	else {
		glUniform1i(loc, value);
	}
}


void setFloat(GLuint a_id, const char* name, float value)
{
	unsigned int loc = glGetUniformLocation(a_id, name);
	if (loc == -1) {
		return;
		//cout << "Uniform float: " << name <<" doesn't exist."<< endl;
	}
	else {
		glUniform1f(loc, value);
	}
}
void setFloat1V(GLuint a_id, const char* name, unsigned int count, const float* floatPtr)
{
	unsigned int loc = glGetUniformLocation(a_id, name);
	if (loc == -1) {
		return;
		//cout << "Uniform float1 vector: " << name << " doesn't exist."<<endl;
	}
	else {
		glUniform1fv(loc, count, floatPtr);
	}
}

void setFloat3V(GLuint a_id, const char* name, unsigned int count, const float* floatPtr)
{
	unsigned int loc = glGetUniformLocation(a_id, name);
	if (loc == -1) {
		return;
		//cout << "Uniform float3 vector: " << name << " doesn't exist." << endl;
	}
	else {
		glUniform3fv(loc, count, floatPtr);
	}
}

void setMatrix4fv(GLuint a_id, const char* name, unsigned int count, const float* floatPtr)
{
	unsigned int loc = glGetUniformLocation(a_id, name);
	if (loc == -1) {
		return;
		//cout << "Uniform matrix: " << name << " doesn't exist."<<endl;
	}
	else {
		glUniformMatrix4fv(loc, count, GL_FALSE, floatPtr);
	}
}

void printProgramInfoLog (unsigned int prog_id)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0)
	{
		infoLog = new char [infoLogLen];
		glGetProgramInfoLog(prog_id, infoLogLen, &charsWritten, infoLog);
		cout << infoLog << endl;
		delete [] infoLog;
	}
}

// setup shader sampler texture data 
void createDataTexture(unsigned int& pGLTexID, float* pData, unsigned int pMaxWidth, unsigned int pMaxHeight)
{

	glGenTextures(1, &pGLTexID);
	glActiveTexture(GL_TEXTURE0 + pGLTexID);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pGLTexID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA_FLOAT32_ATI/*GL_RGBA32F_ARB*/, pMaxWidth, pMaxHeight, 0, GL_RGBA, GL_FLOAT, pData );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F/*GL_RGBA32F_ARB*/, pMaxWidth, pMaxHeight, 0, GL_RGBA, GL_FLOAT, pData);
	glDisable(GL_TEXTURE_2D);
}

void setSampler(GLuint a_id, const char* sampleName, GLuint textureUnit)
{
	GLint samplerLocation = glGetUniformLocation(a_id, sampleName);
	glUniform1i(samplerLocation, textureUnit);
}

