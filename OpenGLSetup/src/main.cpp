// This OpenGL project demonstrates a simple interactive camera implementation and a few vertex and fragment shaders. 
// Two modes of camera controls (press c to switch between two modes): 
// 1. Focus Mode: Holding ALT and Left Mouse Button (LMB) and moving the mouse will rotate the camera about the LookAt Position
//                Holding ALT and MMB and moving the mouse will pan the camera.
//                Holding ALT and RMB and moving the mouse will zoom the camera.
// 2. First-Person Mode: Pressing W, A, S, or D will move the camera
//                       Holding LMB and moving the mouse will roate the camera.
// A displacement shader - a special fireball visual effects with Perlin noise function
// A toon toon shading shader - catoonish rendering effects
// Per-vertex shading v.s. per-fragment shading = visual comparison between two types of shading 

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Camera.h"
#include "Text.h"
#include "Mesh.h"

#include <iostream>
using namespace std;
using namespace glm;

int g_winWidth  = 1024;
int g_winHeight = 512;

Camera g_cam;
Text g_text;

unsigned char g_keyStates[256];

char v_shader_file[] =
//".\\shaders\\displacement.vert"; // vertex displacement shader with perlin noise
//".\\shaders\\perVert_lambert.vert"; // basic lambert lighting  
// ".\\shaders\\perFrag_lambert.vert"; // basic lambert lighting with per-fragment implementation
 ".\\shaders\\toon_shading.vert"; // basic toon shading with per-fragment implementation

char f_shader_file[] =
// ".\\shaders\\displacement.frag"; // vertex displacement shader with perlin noise
// ".\\shaders\\perVert_lambert.frag"; // basic lambert shading 
// ".\\shaders\\perFrag_lambert.frag"; // basic lambert shading with per-fragment implementation
 ".\\shaders\\toon_shading.frag"; // basic toon shading with per-fragment implementation

const char meshFile[128] = "Mesh/sphere.obj";
Mesh g_mesh;

vec3 g_lightPos = vec3(3, 3, 3);
float g_time = 0.0f;

// fps counter
float elapsed = 0.f;
float g_time_start = 0.f;
int g_fps = 0;
int f_count = 0;

void initialization() 
{    
    g_cam.set(1.0f, 2.0f, 4.0f, 0.0f, 1.0f, -0.5f, g_winWidth, g_winHeight);
	g_text.setColor(0.0f, 0.0f, 0.0f);

	g_mesh.create(meshFile, v_shader_file, f_shader_file);
	// add any stuff you want to initialize ...
}

/****** GL callbacks ******/
void initialGL()
{    
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	glClearColor (1.0f, 1.0f, 1.0f, 0.0f);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{
    // add any stuff to update at runtime ....
	f_count++;
	elapsed += ((float)glutGet(GLUT_ELAPSED_TIME)) - g_time_start;
	if (elapsed >= 1000.f)
	{
		g_fps = (int)(f_count);
		elapsed = 0.f;
		f_count = 0;
		g_time_start = (float)glutGet(GLUT_ELAPSED_TIME);
	}
	/*float t = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	g_fps = f_count / t;*/
	//cout << "FPS: " << g_fps << endl;

    g_cam.keyOperation(g_keyStates, g_winWidth, g_winHeight);

	glutPostRedisplay();
}

void display()
{	 
	

	

	glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// add any stuff you'd like to draw	

	glUseProgram(0);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	g_cam.drawGrid();
    g_cam.drawCoordinateOnScreen(g_winWidth, g_winHeight);
    g_cam.drawCoordinate();

	// display the text
	string str;
	if(g_cam.isFocusMode()) {
        str = "Cam mode: Focus";
		g_text.draw(10, 30, const_cast<char*>(str.c_str()), g_winWidth, g_winHeight);
	} else if(g_cam.isFPMode()) {
        str = "Cam mode: FP";
		g_text.draw(10, 30, const_cast<char*>(str.c_str()), g_winWidth, g_winHeight);
	}
	str = "FPS: " + std::to_string(g_fps);
	g_text.draw(10, 45, const_cast<char*>(str.c_str()), g_winWidth, g_winHeight);
	str = "triangle count: " + std::to_string(g_mesh.tri_num);
	g_text.draw(10, 60, const_cast<char*>(str.c_str()), g_winWidth, g_winHeight);
		
	/*str = "FPS: " + g_fps;
	g_text.draw(10, 100, const_cast<char*>(str.c_str()), g_winWidth, g_winHeight);*/

	g_time = (float)glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	g_mesh.draw(g_cam.viewMat, g_cam.projMat, g_lightPos, g_time);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
	g_winWidth = w;
	g_winHeight = h;
	if (h == 0) {
		h = 1;
	}
	g_cam.setProjectionMatrix(g_winWidth, g_winHeight);
    g_cam.setViewMatrix();
    glViewport(0, 0, w, h);
}

void mouse(int button, int state, int x, int y)
{
    g_cam.mouseClick(button, state, x, y, g_winWidth, g_winHeight);
}

void motion(int x, int y)
{
    g_cam.mouseMotion(x, y, g_winWidth, g_winHeight);
}

void keyup(unsigned char key, int x, int y)
{
    g_keyStates[key] = false;
}

void keyboard(unsigned char key, int x, int y)
{
    g_keyStates[key] = true;
	switch(key) { 
		case 27:
			exit(0);
			break;
        case 'c': // switch cam control mode
            g_cam.switchCamMode();
			glutPostRedisplay();
            break;
        case ' ':
            g_cam.PrintProperty();
            break;
	}
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(g_winWidth, g_winHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("VertFrag Shader Example");
	
	glewInit();
	initialGL();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
    glutKeyboardUpFunc(keyup);
    glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	initialization();


	g_time_start = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;


	glutMainLoop();
	/*g_time_start = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	while (true)
	{
		f_count++;
		elapsed += ((float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f) - g_time_start;
		if (elapsed >= 60.f)
		{
			g_fps = (int)(f_count / elapsed);
			elapsed = 0.f;
			f_count = 0;
			g_time_start = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
		}
		//glutPostRedisplay();
		cout << "FPS: " << g_fps << endl;
		glutMainLoopEvent();

	}*/
    return EXIT_SUCCESS;
}