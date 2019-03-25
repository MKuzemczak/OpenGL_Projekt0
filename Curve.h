#pragma once

#include "Rect.h"

class Curve
{
	std::vector<Rect> rects;
	std::vector<glm::vec2> points;
	

public:
	Curve();
	~Curve();

	void draw();
	void addPoint(glm::vec2 & pt);
	glm::vec2 & getPoint(int index);
	const glm::vec2 & getPoint(int index) const;

};

