#include "ShaderClass.h"
#include <iostream>
#include <GL/glew.h>
using namespace std;

vector<GLuint> vShader, fShader;

GLuint createShader(const char* shaderFileName, GLenum targetType)
{
	static GLuint vshader_id = -1;
	static GLuint fshader_id = -1;

	GLuint currID = -1;

	if (targetType == GL_VERTEX_SHADER)
	{
		vshader_id++;
		currID = vshader_id;
	}
	else if (targetType == GL_FRAGMENT_SHADER)
	{
		fshader_id++;
		currID = fshader_id;
	}

	//  target types: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		cout<<"Ready for GLSL\n";
	else {
		cout<<"No GLSL support\n";
		exit(1);
	}
	//GLuint id = glCreateProgram();
	char* source = NULL;
	int status;
	if (shaderFileName)
	{
		source = loadShaderFile(currID, shaderFileName);
		if (source == NULL)
		{
			cout << "The shader of '"<< shaderFileName << "' isn't created!" << endl;
			return 0;
		}
		GLuint id = glCreateShader(targetType);
		glShaderSource(id, 1, const_cast<const GLchar * *>(&source), NULL);
		glCompileShader(id);

		if (targetType == GL_VERTEX_SHADER)
		{
			vShader.push_back(id);
		}
		else if (targetType == GL_FRAGMENT_SHADER)
		{
			fShader.push_back(id);
		}

		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			cout << "Error: The shader of '" << shaderFileName << "' is created but incorrect, see the shader info log below!" << endl;
			printShaderInfoLog(id);
		}

		delete[] source;

		
	}
	
	return currID;

	//glLinkProgram(id);

	//glGetProgramiv(id, GL_LINK_STATUS, &status);
	//if (status != GL_TRUE)
	//	printProgramInfoLog(id);

	
	//glDeleteShader(vShader);
	//glDeleteShader(fShader);
	//glDeleteShader(cShader);
}

// delete the shader after it's linked into a sahder program id, as it's no longer needed
void destroyShader(GLuint a_id, GLenum targetType)
{
	if (targetType == GL_VERTEX_SHADER)
	{
		glDeleteShader(vShader[a_id]);
	}
	else if (targetType == GL_FRAGMENT_SHADER)
	{
		glDeleteShader(fShader[a_id]);
	}
}

char* loadShaderFile(GLuint a_id, const char* fn)
{
	ifstream file;
	char* content = NULL;
	int count = 0;

	file.open(fn, ios::in, ifstream::binary); // open the shader file
	if (!file)
	{
		cout << fn << " doesn't exist!" << endl;
		return NULL;
	}
	if (!file.good()) 
	{
		cout << fn << " is empty!" << endl;
		return NULL;
	}
	file.seekg(0, ios::end);
	unsigned long len = file.tellg();
	file.seekg(ios::beg);
	if (len == 0)
	{
		cout<< fn << "is empty!" << endl;
		return NULL;
	}

	// create the shader source content and read the file
	content = new char[len+1];
	content[len] = '\0';

	
	unsigned int i =0;

	while (!file.eof())
	{
		content[i] = file.get(); // get a character from the shader file
		i++;
	}
	content[i-1] = '\0';
	file.close();
	return content;

/*
	if (fn != NULL) {
		fp = fopen_s(fn, "rt");

		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		else {
			cout << fn << "does not exsit !" << endl;
		}
	}
	else {
		cout << "file name is null !" << endl;
	}*/
	//return content;
}

void printShaderInfoLog(unsigned int shader_id)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0)
	{
		infoLog = new char [infoLogLen];
		glGetShaderInfoLog(shader_id, infoLogLen, &charsWritten, infoLog);
		cout << infoLog << endl;;
		delete [] infoLog;
	}
}
