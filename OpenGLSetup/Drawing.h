#pragma once
#include "includes.h"

class Drawing
{
public:
	std::vector<float*> verticies;
	void AddVertex(float x, float y);
	~Drawing();
	GLenum GetMode(int offset = 0);
private:
	void Sort();
};

