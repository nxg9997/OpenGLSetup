#include "callbacks.h"
extern std::vector<Drawing*> shapes;
extern Drawing intermediateDrawing;

void callbacks::Mouse(int btn, int click, int x, int y)
{
	static int points = 0;
	static Drawing* currentDrawing = nullptr;
	if (currentDrawing == nullptr) {
		std::cout << "pointer in callback: " << (Drawing*)currentPointer << std::endl;
		currentDrawing = new Drawing();
		currentPointer = (int)currentDrawing;
	}
	//callbacks::Init();
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
		if (click == 0) {
			points++;
			currentDrawing->AddVertex(MAP(x,0,600,0,10), abs(MAP(y,0,600,-10,0)));
			// add current vertex to the intermediate drawing, then set the mouse's current position to the next vertex, etc. need to add mouse move event
			//intermediateDrawing.AddVertex(currentDrawing->verticies[currentDrawing->verticies.size() - 1][0], currentDrawing->verticies[currentDrawing->verticies.size() - 1][1]);
			if ((points == 1 && callbacks::shape == 0) || (points == 2 && callbacks::shape == 1) || (points == 3 && callbacks::shape == 2) || (points == 4 && callbacks::shape == 3) || (points > 4 && callbacks::shape == 4))
			{
				shapes.push_back(currentDrawing);
				points = 0;
				currentDrawing = nullptr;
				glutPostRedisplay();
			}
		}
	}
	else if (btn == 2 && click == 0)
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
		std::cout << "shape: " << callbacks::shape << std::endl;
	}
}

void callbacks::PassiveMouse(int x, int y)
{
	//std::cout << "mousepos: [" << x << "," << y << "]" << std::endl;
	callbacks::mousePos[0] = MAP(x, 0, 600, 0, 10);
	callbacks::mousePos[1] = abs(MAP(y, 0, 600, -10, 0));
	glutPostRedisplay();
}

void callbacks::Keyboard(unsigned char key, int x, int y)
{
	std::cout << "Key: " << (int)key << " X: "<< x <<  " Y: " << y << std::endl;
}

void callbacks::Init()
{
	static bool initialized = false;
	if (initialized) return;
	//callbacks::shape = 1;
	std::cout << "initializing callbacks..." << std::endl;
	//shapesRef = sr;
	//currentDrawing = nullptr;
	std::cout << &shapes << std::endl;
	mousePos[0] = 0;
	mousePos[1] = 0;
	initialized = true;
}

callbacks::~callbacks()
{
	delete[] mousePos;
}

int callbacks::shape = 0;
float* callbacks::mousePos = new float(2);
int callbacks::currentPointer = 0;