// OpenGLSetup.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "includes.h"

// the window's width and height
int width, height;

// create vector to store shapes
std::vector<Drawing*> shapes;

//used to draw shape before it is finalized
Drawing* intermediateDrawing = nullptr;

//test function for tri drawing
void createTriangle()
{
    Drawing* tri = new Drawing();

    tri->AddVertex(0.f, 0.f);
    tri->AddVertex(5.f, 0.f);
    tri->AddVertex(2.5f, 3.f);
    shapes.push_back(tri);
}

//test function for drawing rects
void createRectangle()
{
    Drawing* rect = new Drawing();

    rect->AddVertex(5.f, 0.f);
    rect->AddVertex(5.f, 2.5f);
    rect->AddVertex(7.5f, 2.5f);
    rect->AddVertex(7.5f, 0.f);
    shapes.push_back(rect);
}

//initiallize application
void init(void)
{
    // initialize the size of the window
    std::cout << &shapes << std::endl;
    callbacks::Init();
    width = 600;
    height = 600;
    //createTriangle();
    //createRectangle();
}

// called when the GL context need to be rendered
void display(void)
{
    //std::cout << "called display" << std::endl;
    //std::cout << shapes.size() << std::endl;
    // clear the screen to white, which is the background color
    glClearColor(1.0, 1.0, 1.0, 0.0);

    // clear the buffer stored for drawing
    glClear(GL_COLOR_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // specify the color for drawing
    glColor3f(1.0, 0.0, 0.0);

    // this is immedidate mode of OpenGL usded prior to OpenGL 3.0
    /*glBegin(tri.GetMode());
    /*glVertex2fv(v0);
    glVertex2fv(v1);
    glVertex2fv(v2);
    glVertex2fv(tri.verticies[0]);
    glVertex2fv(tri.verticies[1]);
    glVertex2fv(tri.verticies[2]);
    glEnd();*/

    for (int i = 0; i < shapes.size(); i++) {
        glBegin(shapes[i]->GetMode());
        for (int j = 0; j < shapes[i]->verticies.size(); j++) {
            glVertex2fv(shapes[i]->verticies[j]);
            //std::cout << *(shapes[i]->verticies[j]) << std::endl;
        }
        glEnd();
    }

	//intermediate
	
	intermediateDrawing = (Drawing*)callbacks::currentPointer;
	if (intermediateDrawing != nullptr)
	{
		glBegin(intermediateDrawing->GetMode(1));
		for (int j = 0; j < intermediateDrawing->verticies.size(); j++)
		{
			glVertex2fv(intermediateDrawing->verticies[j]);
			//std::cout << *(shapes[i]->verticies[j]) << std::endl;
		}
		glVertex2fv(callbacks::mousePos);
		glEnd();
	}
	
    

    // specify the color for new drawing
    glColor3f(0.0, 0.0, 1.0);

    // draw the origin of the canvas
    /*glPointSize(30.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPointSize(1.0f);*/

    glutSwapBuffers();
}

// called when window is first created or when window is resized
void reshape(int w, int h)
{
    // update thescreen dimensions
    width = w;
    height = h;

    //do an orthographic parallel projection, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 10.0, 0.0, 10.0);
    //gluOrtho2D(-5.0, 5.0, -5.0, 5.0);

    /* tell OpenGL to use the whole window for drawing */
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    //glViewport((GLsizei) width/2, (GLsizei) height/2, (GLsizei) width, (GLsizei) height);

    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    // before create a glut window,
    // initialize stuff not opengl/glut dependent
    init();

    //initialize GLUT, let it extract command-line GLUT options that you may provide
    //NOTE that the '&' before argc
    glutInit(&argc, argv);

    // specify as double bufferred can make the display faster
    // Color is speicfied to RGBA, four color channels with Red, Green, Blue and Alpha(depth)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    //set the initial window size */
    glutInitWindowSize((int)width, (int)height);

    // create the window with a title
    glutCreateWindow("First OpenGL Program");

    /* --- register callbacks with GLUT --- */

    //register function to handle window resizes
    glutReshapeFunc(reshape);

    //register function that draws in the window
    glutDisplayFunc(display);

    glutMouseFunc(callbacks::Mouse);
	glutPassiveMotionFunc(callbacks::PassiveMouse);
	glutKeyboardFunc(callbacks::Keyboard);
	/*glLineWidth(10.f);
	glPointSize(10.f);*/

    //start the glut main loop
    glutMainLoop();

    for (int i = 0; i < shapes.size(); i++) {
        delete shapes[i];
    }

    return 0;
}