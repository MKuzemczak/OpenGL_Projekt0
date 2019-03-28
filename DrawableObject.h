#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "shader.hpp"
#include "vectorUtils.h"

#define PI 3.14159265359
#define DEG_TO_RAD PI/180

class DrawableObject
{
protected:
	static GLuint VertexArrayID;
	static unsigned int objectCntr;
	static unsigned int everCreatedObjectCntr;

	std::vector<unsigned int> indices;
	std::vector<GLfloat> vertices,
		colors;

	std::string vertexShaderFileName,
		fragmentShaderFileName;
	int objectNumber;
	GLuint programID, vertexBuffer, colorBuffer;
	unsigned int vertexBufferSize;

	int drawingMode;

public:
	DrawableObject();
	virtual ~DrawableObject();

	virtual void generateShaders() {}
	virtual void draw() {}

	void addIndex(unsigned int i);
	void addVertix(GLfloat v);
	void addColor(GLfloat c);

	std::vector<unsigned int> * indicesVectorPtr();
	std::vector<GLfloat> * verticesVectorPtr();
	std::vector<GLfloat> * colorsVectorPtr();

	void setDrawingMode(int mode);
};

