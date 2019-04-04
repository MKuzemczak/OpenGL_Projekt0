#ifndef CURVE_H
#define CURVE_H

#include "Circ.h"
#include "Rect.h"

class Curve : public Triangles
{
	static GLuint programID;
	static GLuint curveCntr;
	//static GLuint vertexColorUniformID;
	static GLuint orthoMatrixUniformID;

	bool updateAllVertices, updateAllColors;

	std::vector<glm::vec2> points;
	std::vector<Circ *> circles;
	glm::vec4 color;
	float width,length;

	std::vector<GLfloat> calculateRect(glm::vec2 start, glm::vec2 end);

public:
	Curve(float w = 0.01f, glm::vec4 clr = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	~Curve();

	void draw();
	void generateShaders();
	void addPoint(glm::vec2 pt);
	void setPoint(unsigned int  index, glm::vec2 loc);
	void setPointColor(unsigned int  index, glm::vec3 clr);

	glm::vec2 & getPoint(int index);
	const glm::vec2 & getPoint(int index) const;
	std::vector<glm::vec2> & getPoints();
	size_t pointsSize();

};

#endif