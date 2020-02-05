#include "callbacks.h"

// define variables from other files
extern std::vector<Drawing*> shapes;
extern Drawing intermediateDrawing;

// Handle mouse input
void callbacks::Mouse(int btn, int click, int x, int y)
{
	//std::cout << "mouse event: [ btn:" << btn << ", click:" << click << ", x:" << x << ", y:" << y << " ]" << std::endl;
	/*
		btns:
		0 - left
		1 - middle
		2 - right
		3 - scroll up
		4 - scroll down

		clicks:
		0 - down
		1 - up
	*/

	if (btn == 0) {
		static Drawing* currentDrawing = nullptr;
		if (currentDrawing == nullptr || (Drawing*)callbacks::currentPointer == nullptr) {
			//std::cout << "pointer in callback: " << (Drawing*)currentPointer << std::endl;
			currentDrawing = new Drawing();
			currentPointer = (int)currentDrawing;
		}
		if (click == 0) {
			points++;
			currentDrawing->AddVertex(MAP(x,0,600,0,10), abs(MAP(y,0,600,-10,0)));
			// add current vertex to the intermediate drawing, then set the mouse's current position to the next vertex, etc. need to add mouse move event
			//intermediateDrawing.AddVertex(currentDrawing->verticies[currentDrawing->verticies.size() - 1][0], currentDrawing->verticies[currentDrawing->verticies.size() - 1][1]);
			if ((points == 1 && callbacks::shape == 0) || (points == 2 && callbacks::shape == 1) || (points == 3 && callbacks::shape == 2) || (points == 4 && callbacks::shape == 3))
			{
				/*if (callbacks::shape == 4 && !callbacks::enterPressed) {
					return;
				}
				callbacks::enterPressed = false;*/
				/*if (points == 1) {
					std::cout << "single point v count: " << currentDrawing->verticies.size() << std::endl;
				}*/
				currentDrawing->SetColor();
				currentDrawing->SetSizes();
				shapes.push_back(currentDrawing);
				points = 0;
				currentDrawing = nullptr;
				glutPostRedisplay();
			}
		}
	}
	else if (btn == 2 && click == 0 && false)
	{
		int temp = (callbacks::shape +1);
		if (temp > 4)
		{
			callbacks::shape = 0;
		}
		else
		{
			callbacks::shape = temp;
		}
		//std::cout << "shape: " << callbacks::shape << std::endl;
	}
}

// Updates the mouse position when it is moved
void callbacks::PassiveMouse(int x, int y)
{
	//std::cout << "mousepos: [" << x << "," << y << "]" << std::endl;
	callbacks::mousePos[0] = MAP(x, 0, 600, 0, 10);
	callbacks::mousePos[1] = abs(MAP(y, 0, 600, -10, 0));
	glutPostRedisplay();
}

// Handle keyboard input
void callbacks::Keyboard(unsigned char key, int x, int y)
{
	//std::cout << "Key: " << (int)key << " X: "<< x <<  " Y: " << y << std::endl;
	if ((int)key == 13) {
		callbacks::enterPressed = true;
	}

	if (callbacks::shape == 4  && callbacks::enterPressed) {
		callbacks::enterPressed = false;
		((Drawing*)callbacks::currentPointer)->SetColor();
		((Drawing*)callbacks::currentPointer)->SetSizes();
		shapes.push_back((Drawing*)callbacks::currentPointer);
		points = 0;
		callbacks::currentPointer = (int)nullptr;
		glutPostRedisplay();
	}
}

// Initializes the callbacks class
void callbacks::Init()
{
	static bool initialized = false;
	if (initialized) return;
	//callbacks::shape = 1;
	//std::cout << "initializing callbacks..." << std::endl;
	//shapesRef = sr;
	//currentDrawing = nullptr;
	//std::cout << &shapes << std::endl;
	mousePos[0] = 0;
	mousePos[1] = 0;
	callbacks::color[0] = 255;
	callbacks::color[1] = 0;
	callbacks::color[2] = 0;
	//callbacks::CreateMenu();
	initialized = true;
}

// Creates the OpenGL menu and sets it to right-click
void callbacks::CreateMenu()
{
	int colorMenu = glutCreateMenu(callbacks::Menu);
	glutAddMenuEntry("Red", 0);
	glutAddMenuEntry("Green", 1);
	glutAddMenuEntry("Blue", 2);

	int shapeMenu = glutCreateMenu(callbacks::Menu);
	glutAddMenuEntry("Point", 3);
	glutAddMenuEntry("Line", 4);
	glutAddMenuEntry("Triangle", 5);
	glutAddMenuEntry("Quad", 6);
	glutAddMenuEntry("Polygon", 7);

	int lineMenu = glutCreateMenu(callbacks::Menu);
	glutAddMenuEntry("Small", 8);
	glutAddMenuEntry("Medium", 9);
	glutAddMenuEntry("Large", 10);

	int pointMenu = glutCreateMenu(callbacks::Menu);
	glutAddMenuEntry("Small", 11);
	glutAddMenuEntry("Medium", 12);
	glutAddMenuEntry("Large", 13);

	int mainMenu = glutCreateMenu(callbacks::Menu);
	glutAddSubMenu("Color", colorMenu);
	glutAddSubMenu("Shape", shapeMenu);
	glutAddSubMenu("Line Width", lineMenu);
	glutAddSubMenu("Point Size", pointMenu);
	glutAddMenuEntry("Clear", 14);
	glutAddMenuEntry("Quit", 15);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Handle menu options
void callbacks::Menu(int a)
{
	switch (a) {
		case 0:
			callbacks::color[0] = 255;
			callbacks::color[1] = 0;
			callbacks::color[2] = 0;
			break;
		case 1:
			callbacks::color[0] = 0;
			callbacks::color[1] = 255;
			callbacks::color[2] = 0;
			break;
		case 2:
			callbacks::color[0] = 0;
			callbacks::color[1] = 0;
			callbacks::color[2] = 255;
			break;
		case 3:
			callbacks::shape = 0;
			break;
		case 4:
			callbacks::shape = 1;
			break;
		case 5:
			callbacks::shape = 2;
			break;
		case 6:
			callbacks::shape = 3;
			break;
		case 7:
			callbacks::shape = 4;
			break;
		case 8:
			callbacks::lineWidth = 1;
			break;
		case 9:
			callbacks::lineWidth = 5;
			break;
		case 10:
			callbacks::lineWidth = 10;
			break;
		case 11:
			callbacks::pointSize = 1;
			break;
		case 12:
			callbacks::pointSize = 5;
			break;
		case 13:
			callbacks::pointSize = 10;
			break;
		case 14:
			for (int i = 0; i < shapes.size(); i++) {
				delete shapes[i];
			}
			shapes.clear();
			break;
		case 15:
			exit(0);
			break;
		default:
			break;
	}
	//glLineWidth(callbacks::lineWidth);
	glutPostRedisplay();
}

// clean up memory (destructor)
callbacks::~callbacks()
{
	delete[] mousePos;
	delete[] callbacks::color;
}

// initialize static variables
int callbacks::shape = 0;
float* callbacks::mousePos = new float(2);
int callbacks::currentPointer = 0;
bool callbacks::enterPressed = false;
int callbacks::points = 0;
int callbacks::menu = -1;
int callbacks::pointSize = 1;
int callbacks::lineWidth = 1;
int* callbacks::color = new int[3];