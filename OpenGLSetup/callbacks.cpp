#include "callbacks.h"
extern std::vector<Drawing*> shapes;
extern Drawing intermediateDrawing;

void callbacks::Mouse(int btn, int click, int x, int y)
{
	static int points = 0;
	static Drawing* currentDrawing = nullptr;
	if (currentDrawing == nullptr) {
		currentDrawing = new Drawing();
	}
	//callbacks::Init();
	std::cout << "mouse event: [ btn:" << btn << ", click:" << click << ", x:" << x << ", y:" << y << " ]" << std::endl;
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
			if (points == 3) {
				shapes.push_back(currentDrawing);
				points = 0;
				currentDrawing = nullptr;
				glutPostRedisplay();
			}
		}
	}
}

void callbacks::Init()
{
	static bool initialized = false;
	if (initialized) return;
	std::cout << "initializing callbacks..." << std::endl;
	//shapesRef = sr;
	//currentDrawing = nullptr;
	std::cout << &shapes << std::endl;

	initialized = true;
}
