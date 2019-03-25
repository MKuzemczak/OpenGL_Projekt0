#pragma once
#include "Triangles.h"
class Circ : public Triangles
{
	GLfloat radius;
	glm::vec2 location;

public:
	Circ(GLfloat rad, glm::vec2 loc);
	Circ(GLfloat rad, GLfloat locx, GLfloat locy);

	~Circ();
};

