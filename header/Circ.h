#ifndef CIRC_H
#define CIRC_H

#include "Triangles.h"
class Circ : public Triangles
{
	GLfloat radius;
	glm::vec4 color;

public:
	
	Circ(GLfloat rad, glm::vec2 loc, glm::vec4 clr = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) :
		Circ(rad, loc[0], loc[1], clr)
	{
	}
	Circ(GLfloat rad, GLfloat locx, GLfloat locy, glm::vec4 clr = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	void setColor(glm::vec4 clr);

	~Circ();
};

#endif