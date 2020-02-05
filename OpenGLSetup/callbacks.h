#pragma once
#include "includes.h"

class callbacks
{
public:
	static void Mouse(int a, int b, int c, int d);
	static void PassiveMouse(int x, int y);
	static void Keyboard(unsigned char key, int x, int y);
	static void Init();
	static void CreateMenu();
	static int shape;
	static float* mousePos;
	static int currentPointer;
	static bool enterPressed;

	static int lineWidth;
	static int pointSize;
	static int* color;
	static void Menu(int a);

	~callbacks();

private:
	static int points;
	static int menu;
};

