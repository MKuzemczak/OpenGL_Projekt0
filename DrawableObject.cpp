#include "DrawableObject.h"

unsigned int DrawableObject::objectCntr = 0;
GLuint DrawableObject::VertexArrayID;

DrawableObject::DrawableObject()
{
	if (objectCntr == 0)
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
	}

	objectCntr++;
	objectNumber = objectCntr;

	drawingMode = GL_POINTS;
}


DrawableObject::~DrawableObject()
{
	objectCntr--;
}

void DrawableObject::addIndex(unsigned int i)
{
	indices.push_back(i);
}

void DrawableObject::addVertix(GLfloat v)
{
	vertices.push_back(v);
}

void DrawableObject::addColor(GLfloat c)
{
	colors.push_back(c);
}

std::vector<unsigned int> * DrawableObject::indicesVectorPtr()
{
	return &indices;
}

std::vector<GLfloat> * DrawableObject::verticesVectorPtr()
{
	return &vertices;
}

std::vector<GLfloat> * DrawableObject::colorsVectorPtr()
{
	return &colors;
}

void DrawableObject::setDrawingMode(int mode)
{
	drawingMode = mode;
}