#include "Circ.h"

#define PI 3.14159265359
#define DEG_TO_RAD PI/180

Circ::Circ(GLfloat rad, glm::vec2 loc)
{
	vertices.push_back(loc[0]);
	vertices.push_back(loc[1]);
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
		vertices.push_back(loc[0]+rad*cos(DEG_TO_RAD*i));
		vertices.push_back(loc[1] + rad*sin(DEG_TO_RAD*i));
		vertices.push_back(0.0f);
	}

	vertices.push_back(loc[0] + rad);
	vertices.push_back(loc[1]);
	vertices.push_back(0.0f);

	radius = rad;
	location = loc;

	generateShaders();

	setDrawingMode(GL_TRIANGLE_FAN);
}


Circ::Circ(GLfloat rad, GLfloat locx, GLfloat locy)
{
	vertices.push_back(locx);
	vertices.push_back(locy);
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
		vertices.push_back(locx + rad*cos(DEG_TO_RAD*i));
		vertices.push_back(locy + rad*sin(DEG_TO_RAD*i));
		vertices.push_back(0.0f);
	}
	vertices.push_back(locx + rad);
	vertices.push_back(locy);
	vertices.push_back(0.0f);

	radius = rad;
	location[0] = locx;
	location[1] = locy;

	generateShaders();

	setDrawingMode(GL_TRIANGLE_FAN);
}

Circ::~Circ()
{
}
