#include "Mesh.h"

vector<GLuint> vert_num, tri_num, vao, vbo, nbo, ibo;
vector<vec3*> vertices, fnormals, vnormals;
vector<uvec3*> triangles;
vector<mat4> modelMat;

void computeNormals(GLuint a_id)
{
	fnormals.push_back(new vec3[tri_num[a_id]]);
	vnormals.push_back(new vec3[vert_num[a_id]]);

	vec3 a, b, n;

	// Compute face normals
	for (unsigned int i = 0; i < tri_num[a_id]; i++) {
		
		a = vertices[a_id][triangles[a_id][i][1]] - vertices[a_id][triangles[a_id][i][0]];
		b = vertices[a_id][triangles[a_id][i][2]] - vertices[a_id][triangles[a_id][i][0]];
		
		n = cross(a, b);
		fnormals[a_id][i] = normalize(n);
	}

	// Compute vertex normals
	for (unsigned int i = 0; i < vert_num[a_id]; i++) {
		vnormals[a_id][i] = vec3(0.0f);
	}

	for (unsigned int i = 0; i < tri_num[a_id]; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			vnormals[a_id][triangles[a_id][i][j]] += fnormals[a_id][i];
		}
	}

	for (unsigned int i = 0; i < vert_num[a_id]; i++) {
		vnormals[a_id][i] = normalize(vnormals[a_id][i]);
	}

}

void prepareVBOandShaders(GLuint a_id, const char* v_shader_file, const char* f_shader_file)
{
	GLuint vShader_id = createShader(v_shader_file, GL_VERTEX_SHADER);
	GLuint fShader_id = createShader(f_shader_file, GL_FRAGMENT_SHADER);
	//fShader.create(f_shader_file, GL_FRAGMENT_SHADER);
	GLuint program_id = createProgram();
	linkShader(shaderProg[program_id], vShader[vShader_id], GL_VERTEX_SHADER);
	linkShader(shaderProg[program_id], fShader[fShader_id], GL_FRAGMENT_SHADER);
	/*shaderProg.create();
	shaderProg.link(vShader);
	shaderProg.link(fShader);*/

	destroyShader(vShader_id, GL_VERTEX_SHADER);
	destroyShader(fShader_id, GL_FRAGMENT_SHADER);
	/*vShader.destroy();
	fShader.destroy();*/

	// create vbo 
	vbo.push_back(0);
	// generate a new VBO and get the associated ID
	glGenBuffers(1, &vbo[a_id]);

	// bind VBO in order to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo[a_id]);

	// upload data to VBO - data went to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vert_num[a_id], vertices[a_id], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up
	// delete[] vertices; // commented out, since it's handled by the destructor


	// repeat for normals and indices
	nbo.push_back(0);
	glGenBuffers(1, &nbo[a_id]);
	glBindBuffer(GL_ARRAY_BUFFER, nbo[a_id]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vert_num[a_id], vnormals[a_id], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up
	// delete[] vnormals; // commented out, since it's handled by the destructor

	ibo.push_back(0);
	glGenBuffers(1, &ibo[a_id]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[a_id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * tri_num[a_id], triangles[a_id], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // clean up
	// delete[] triangles; // commented out, since it's handled by the destructor
	
	// so far, we transferred the position and index data to GPU. There will be no data transfer calls at drawing time

	// Define VAO
	// There are 2 buffer objects (vertices and normals), which should be input "attribute" variables to the vertex shader. 
	// We set up the layout of both of these with vertex array object. 
	// The actual vertices and normal vectors have already been kept on the GPU memory by VBOs. 
	// The purpose of VAO is to have vertices and normal vectors as separate attributes in the vertex shader. 
	// So VAO's attrobites point to these data on the GPU, rather than referring back to any CPU data. 

	vao.push_back(0);
	glGenVertexArrays(1, &vao[a_id]);
	glBindVertexArray(vao[a_id]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[a_id]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // 0 - the layout id in vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, nbo[a_id]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL); // 1 - the layout id in fragment shader

	// Ettributes are disabled by default in OpenGL 4. 
	// We have to explicitly enable each one.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void deleteMeshes()
{
	for (int i = 0; i < vertices.size(); i++)
	{
		delete[] vertices[i];
		delete[] triangles[i];
		delete[] fnormals[i];
		delete[] vnormals[i];
	}
}

GLuint createMesh(const char* filename, const char* v_shader_file, const char* f_shader_file) {

	static int mesh_id = -1; mesh_id++;

	modelMat.push_back(mat4());

	vector<vec3> ori_vertices;
	vector<uvec3> ori_triangles;

    ifstream fs(filename);

    char c;
    vec3 pos;
    int index[3];
	int vid = 0;

    std::string line;
    while (std::getline(fs, line)) {
        std::istringstream iss(line);

        iss >> c;

        switch (c) {
        case 'v':
			{
				// read a vertex
				iss >> pos.x;
				iss >> pos.y;
				iss >> pos.z;
				ori_vertices.push_back(pos);
				break;
			}
        case 'f':
			{
				// read a triangle's vertex indices
				iss >> index[0];
				iss >> index[1];
				iss >> index[2];
				// NOTE: index in obj files starts from 1
				ori_triangles.push_back(uvec3(index[0] - 1, index[1] -1, index[2] -1));
				break;
			}
        default:
            // skip the line
            break;
        }
    }
    fs.close();

	vert_num.push_back(ori_vertices.size());
	tri_num.push_back(ori_triangles.size());
	vertices.push_back(new vec3[ori_vertices.size()]);
	triangles.push_back(new uvec3[ori_triangles.size()]);

	// Use arrays to store vertices and triangles, instead of using c++ vectors.
	// This is because we have to use arrays when sending data to GPUs. 
	for (uint i = 0; i < vert_num[mesh_id]; i++) {
		vertices[mesh_id][i] = ori_vertices[i];
	}
	for (uint i = 0; i < tri_num[mesh_id]; i++) {
		triangles[mesh_id][i] = ori_triangles[i];
	}
	
	computeNormals(mesh_id);
	prepareVBOandShaders(mesh_id, v_shader_file, f_shader_file);

	return mesh_id;
}

void drawMesh(GLuint a_id, mat4 viewMat, mat4 projMat, vec3 lightPos, float time, vec3 objPos) {

	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	
	if (vert_num[a_id] <= 0 && tri_num[a_id] <= 0)
		return;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_FILL);


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();


	glUseProgram(shaderProg[a_id]);
	mat4 m = translate(mat4(1.0), /*vec3(0.0f, 2.0f, 0.0f) +*/ objPos);
	modelMat[a_id] = scale(m, vec3(0.3f, 0.3f, 0.3f));
	setMatrix4fv(shaderProg[a_id], "modelMat", 1, value_ptr(modelMat[a_id]));
	setMatrix4fv(shaderProg[a_id], "viewMat", 1, value_ptr(viewMat));
	setMatrix4fv(shaderProg[a_id], "projMat", 1, value_ptr(projMat));
	setFloat3V(shaderProg[a_id], "lightPos", 1, value_ptr(lightPos));
	setFloat(shaderProg[a_id], "time", time);

	//cout << glm::to_string(modelMat) << endl;
	//cout << glm::to_string(viewMat) << endl;
	//cout << glm::to_string(projMat) << endl;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[a_id]);
	glDrawElements(GL_TRIANGLES, tri_num[a_id] * 3, GL_UNSIGNED_INT, NULL);

	glPopMatrix();
	glDisable(GL_POLYGON_OFFSET_FILL);

    glPopMatrix();
}