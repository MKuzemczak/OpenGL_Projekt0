#include "Circ.h"



Circ::Circ(GLfloat rad, glm::vec2 loc)
{
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	float div;
	if (rad < 0.001)
		div = 90;
	else if (rad >= 1)
		div = 1;
	else
		div = 2 + 1 / (11.2 * rad);

	std::cout << div << std::endl;

	for (int i = 360; i >= 0; i-=div)
	{
		vertices.push_back(rad*cos(DEG_TO_RAD*i));
		vertices.push_back(rad*sin(DEG_TO_RAD*i));
		vertices.push_back(0.0f);
	}

	vertices.push_back(rad);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	radius = rad;
	setLocation(loc);

	generateShaders();

	setDrawingMode(GL_TRIANGLE_FAN);
}


Circ::Circ(GLfloat rad, GLfloat locx, GLfloat locy)
{
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	float div;
	if (rad < 0.001)
		div = 90;
	else if (rad >= 1)
		div = 1;
	else
		div = 2 + 1 / (11.2 * rad);

	for (int i = 360; i >= 0; i-=div)
	{
		vertices.push_back(rad*cos(DEG_TO_RAD*i));
		vertices.push_back(rad*sin(DEG_TO_RAD*i));
		vertices.push_back(0.0f);
	}
	vertices.push_back(rad);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	radius = rad;
	setLocation(glm::vec2(locx, locy));

	generateShaders();

	setDrawingMode(GL_TRIANGLE_FAN);
}

Circ::~Circ()
{
}
