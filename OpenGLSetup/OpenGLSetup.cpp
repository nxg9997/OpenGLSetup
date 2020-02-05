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
    //std::cout << &shapes << std::endl;
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

    // loop thru and draw all the shapes
    for (int i = 0; i < shapes.size(); i++) {
        // specify the color for drawing
        glColor3f(COLORTOGL(shapes[i]->color[0]), COLORTOGL(shapes[i]->color[1]), COLORTOGL(shapes[i]->color[2]));
        glLineWidth(shapes[i]->lineWidth);
        glPointSize(shapes[i]->pointSize);
        glBegin(shapes[i]->GetMode());
        for (int j = 0; j < shapes[i]->verticies.size(); j++) {
            glVertex2fv(shapes[i]->verticies[j]);
            //std::cout << *(shapes[i]->verticies[j]) << std::endl;
        }
        glEnd();
    }

	// draw the intermediate shape (the one currently being drawn by the user)
	intermediateDrawing = (Drawing*)callbacks::currentPointer;
	if (intermediateDrawing != nullptr)
	{
        glColor3f(COLORTOGL(callbacks::color[0]), COLORTOGL(callbacks::color[1]), COLORTOGL(callbacks::color[2]));
        glLineWidth(callbacks::lineWidth);
        glPointSize(callbacks::pointSize);
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
    init();
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // initialize the window
    glutInitWindowSize((int)width, (int)height);
    glutCreateWindow("IGME740 - Project 1 - Nathan Glick");

    //register function to handle window resizes
    glutReshapeFunc(reshape);

    // setup callbacks
    glutDisplayFunc(display);
    glutMouseFunc(callbacks::Mouse);
	glutPassiveMotionFunc(callbacks::PassiveMouse);
	glutKeyboardFunc(callbacks::Keyboard);

    // create the right-click menu
    callbacks::CreateMenu();

    //start the glut main loop
    glutMainLoop();

    // clean up memory
    for (int i = 0; i < shapes.size(); i++) {
        delete shapes[i];
    }

    return 0;
}