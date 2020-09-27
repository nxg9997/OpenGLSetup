#ifndef MESH_H
#define MESH_H

#define GLM_FORCE_RADIANS

#include "ShaderProgram.h"

#include <vector>
#include <set>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using namespace glm;

extern vector<GLuint> vert_num, tri_num, vao, vbo, nbo, ibo;
extern vector<vector<vec3>> vertices, fnormals, vnormals;
extern vector<vector<uvec3>> triangles;
extern vector<mat4> modelMat;







    /* Load the mesh from an '.obj' file. All faces need to be triangles.
	   Attributes like edge lengths and curvatures should be computed when simplifying the mesh.
    */
    GLuint createMesh (const char* filename, const  char* v_shader_file, const char* f_shader_file);

    void drawMesh(GLuint a_id, mat4 viewMat, mat4 projMat, vec3 lightPos, float time);


	void computeNormals(GLuint a_id); // compute both face and vertex normals
	void prepareVBOandShaders(GLuint a_id, const char* v_shader_file, const char* f_shader_file);


#endif
