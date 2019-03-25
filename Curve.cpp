#include "Curve.h"



Curve::Curve()
{
}


Curve::~Curve()
{
}


void draw();
void addPoint(glm::vec2 & pt);
glm::vec2 & getPoint(int index);
const glm::vec2 & getPoint(int index) const;