#pragma once
#include "includes.h"

class Drawing
{
public:
	std::vector<float*> verticies;
	void AddVertex(float x, float y);
	~Drawing();
	GLenum GetMode();
private:
	void Sort();
};

