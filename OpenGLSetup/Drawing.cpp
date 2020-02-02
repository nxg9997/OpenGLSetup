#include "Drawing.h"

void Drawing::AddVertex(float x, float y)
{
	float* v = new float[2];
	v[0] = x;
	v[1] = y;
	verticies.push_back(v);
	std::cout << v[0] << " " << v[1] << std::endl;
}

Drawing::~Drawing()
{
	for (int i = 0; i < verticies.size(); i++) {
		delete[] verticies[i];
	}
}

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

void Drawing::Sort()
{

}
