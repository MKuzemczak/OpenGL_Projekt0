#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "shader.hpp"

class DrawableObject
{
protected:
	static GLuint VertexArrayID;
	static unsigned int objectCntr;

	std::vector<unsigned int> indices;
	std::vector<GLfloat> vertices,
		colors;

	std::string vertexShaderFileName,
		fragmentShaderFileName;
	int objectNumber;
	GLuint programID, vertexBuffer, colorBuffer;

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

