#pragma once
#include "DrawableObject.h"
class Triangles : public DrawableObject
{
protected:
	static GLuint transformationMatrixID;

	glm::mat4 transformationMatrix;

	int newVertices, newLastPoint, newColors;

public:
	Triangles();
	~Triangles();

	void generateShaders();
	void draw();
	void rotate(float angle);
	void move(glm::vec2 vec);
	void move(glm::vec3 vec);
	void setLocation(glm::vec2 point);
	void setLocation(glm::vec3 point);
};

