#include "Rect.h"

LocMode Rect::locationMode = TOP_LEFT;



Rect::Rect(GLfloat w, GLfloat h, GLfloat locx, GLfloat locy, GLfloat rot)
{
	switch (locationMode)
	{
	case TOP_LEFT:
	{
		float tab[12] = {
			0.0f, 0.0f, 0.0f,
			0.0f, -h, 0.0f,
			w, -h, 0.0f,
			w, 0.0f, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case TOP_RIGHT:
	{
		float tab[12] = {
			0.0f, 0.0f, 0.0f,
			-w, 0.0f, 0.0f,
			-w, -h, 0.0f,
			0.0f, -h, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case BOTTOM_LEFT:
	{
		float tab[12] = {
			0.0f, 0.0f, 0.0f,
			w, 0.0f, 0.0f,
			w, h, 0.0f,
			0.0f, h, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case BOTTOM_RIGHT:
	{
		float tab[12] = {
			0.0f, 0.0f, 0.0f,
			0.0f, h, 0.0f,
			-w, h, 0.0f,
			-w, 0.0f, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case CENTER:
	{
		float tab[12] = {
			w / 2, h / 2, 0.0f,
			-w / 2, h / 2, 0.0f,
			-w / 2, -h / 2, 0.0f,
			w / 2, -h / 2, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case TOP_CENTER:
	{
		float tab[12] = {
			-w / 2, 0.0f, 0.0f,
			-w / 2, -h, 0.0f,
			w / 2, -h, 0.0f,
			w / 2, 0.0f, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case BOTTOM_CENTER:
	{
		float tab[12] = {
			w / 2, 0.0f, 0.0f,
			w / 2, h, 0.0f,
			-w / 2, h, 0.0f,
			-w / 2, 0.0f, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case LEFT_CENTER:
	{
		float tab[12] = {
			0.0f, -h / 2, 0.0f,
			w, -h / 2, 0.0f,
			w, h / 2, 0.0f,
			0.0f, h / 2, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case RIGHT_CENTER:
	{
		float tab[12] = {
			0.0f, h / 2, 0.0f,
			-w, h / 2, 0.0f,
			-w, -h / 2, 0.0f,
			0.0f, h / 2, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	}

	for (float f : vertices)
		std::cout << f << std::endl;

	width = w;
	height = h;
	setLocation(glm::vec2(locx, locy));

	rotate(rot);

	generateShaders();

	setDrawingMode(GL_TRIANGLE_FAN);
}

Rect::~Rect()
{
}

void Rect::setLocationMode(LocMode mode)
{
	locationMode = mode;
}

