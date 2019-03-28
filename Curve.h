#pragma once

#include "Circ.h"
#include "Rect.h"

class Curve : Triangles
{
	static GLuint programID;
	static GLuint curveCntr;

	std::vector<glm::vec2> points;
	Circ *startCirc;
	Circ *endCirc;
	double width;

public:
	Curve(double w = 0.01);
	~Curve();

	void draw();
	void generateShaders();
	void addPoint(glm::vec2 pt);
	glm::vec2 & getPoint(int index);
	const glm::vec2 & getPoint(int index) const;

};

