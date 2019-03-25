#pragma once

#include "Circ.h"
#include "Rect.h"

class Curve : Triangles
{
	std::vector<glm::vec2> points;
	Circ *startCirc;
	Circ *endCirc;
	double width;

public:
	Curve(double w = 0.01);
	~Curve();

	void draw();
	void addPoint(glm::vec2 pt);
	glm::vec2 & getPoint(int index);
	const glm::vec2 & getPoint(int index) const;

};

