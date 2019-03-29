#include "Circ.h"

Circ::Circ(GLfloat rad, GLfloat locx, GLfloat locy, glm::vec4 clr)
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
		div = 2 + 1 / (11.2f * rad);

	for (int i = 360; i >= 0; i-=(int)div)
	{
		vertices.push_back(rad*cos((float)DEG_TO_RAD*i));
		vertices.push_back(rad*sin((float)DEG_TO_RAD*i));
		vertices.push_back(0.0f);
	}
	vertices.push_back(rad);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	radius = rad;
	setLocation(glm::vec2(locx, locy));

	color[0] = clr[0];
	color[1] = clr[1];
	color[2] = clr[2];
	color[3] = clr[3];
	for (int i = 0; i < vertices.size() / 3; i++)
	{
		colors.push_back(color[0]);
		colors.push_back(color[1]);
		colors.push_back(color[2]);
		colors.push_back(color[3]);
	}

	generateShaders();

	setDrawingMode(GL_TRIANGLE_FAN);
}

Circ::~Circ()
{
}
