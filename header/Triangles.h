#ifndef TRIANGLES_H
#define TRIANGLES_H
#include "DrawableObject.h"
class Triangles : public DrawableObject
{
protected:

	int newVertices, newLastPoint, newColors;

public:
	Triangles();
	~Triangles();

	void generateShaders();
	void draw();
};
#endif
