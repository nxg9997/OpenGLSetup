#include "Drawing.h"

// Adds a new vertex to the shape
void Drawing::AddVertex(float x, float y)
{
	float* v = new float[2];
	v[0] = x;
	v[1] = y;
	verticies.push_back(v);
	//std::cout << v[0] << " " << v[1] << std::endl;
}

// Constructor, sets the color and size properties
Drawing::Drawing()
{
	SetColor();
	SetSizes();
}

// Clean up memory (destructor)
Drawing::~Drawing()
{
	for (int i = 0; i < verticies.size(); i++) {
		delete[] verticies[i];
	}
	delete[] color;
}

// Returns the OpenGL drawing mode of the shape based on the number of verticies
GLenum Drawing::GetMode(int offset)
{
	switch (verticies.size() + offset) {
	case 0:
		return GL_POINTS;
	case 1:
		return GL_POINTS;
	case 2:
		return GL_LINES;
	case 3:
		return GL_TRIANGLES;
	case 4:
		return GL_QUADS;
	default:
		return GL_POLYGON;
	}
	return GL_POINT;
}

// Grabs the color from the callbacks class and sets it as a property
void Drawing::SetColor()
{
	color[0] = callbacks::color[0];
	color[1] = callbacks::color[1];
	color[2] = callbacks::color[2];
}

// Grabs the sizes of lines and points from the callbacks class and sets those as properties
void Drawing::SetSizes()
{
	lineWidth = callbacks::lineWidth;
	pointSize = callbacks::pointSize;
}

void Drawing::Sort()
{

}
