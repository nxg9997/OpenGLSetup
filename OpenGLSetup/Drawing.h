#pragma once
#include "includes.h"

class Drawing
{
public:
	std::vector<float*> verticies;
	void AddVertex(float x, float y);
	Drawing();
	~Drawing();
	GLenum GetMode(int offset = 0);
	void SetColor();
	void SetSizes();
	int* color = new int[3];
	int lineWidth = 1;
	int pointSize = 1;
private:
	void Sort();
};

