#include "DrawableObject.h"

unsigned int DrawableObject::objectCntr = 0;
unsigned int DrawableObject::everCreatedObjectCntr = 0;
GLuint DrawableObject::VertexArrayID;
GLuint DrawableObject::programID = 0;
glm::mat4 DrawableObject::orthoMatrix = glm::mat4(1.0f);

DrawableObject::DrawableObject()
{
	if (objectCntr == 0)
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
	}

	objectCntr++;
	everCreatedObjectCntr++;
	objectNumber = everCreatedObjectCntr;
	visible = true;
	drawingMode = GL_POINTS;
}


DrawableObject::~DrawableObject()
{
	if (objectCntr == 1)
	{
		glDeleteVertexArrays(1, &VertexArrayID);
	}
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

void DrawableObject::setAspectRatio(GLuint w, GLuint h)
{
	GLfloat aspect = (float)w / h;
	orthoMatrix = glm::ortho((float)-aspect, aspect, -1.0f, 1.0f);
}

void DrawableObject::show()
{
	visible = true;
}

void DrawableObject::hide()
{
	visible = false;
}