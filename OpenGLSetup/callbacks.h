#pragma once
#include "includes.h"

class callbacks
{
public:
	static void Mouse(int a, int b, int c, int d);
	static void PassiveMouse(int x, int y);
	static void Keyboard(unsigned char key, int x, int y);
	static void Init();
	static int shape;
	static float* mousePos;
	static int currentPointer;

	~callbacks();

private:
};

