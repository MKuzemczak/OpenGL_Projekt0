#pragma once
#include "Triangles.h"

enum LocMode
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	CENTER,
	TOP_CENTER,
	BOTTOM_CENTER,
	LEFT_CENTER,
	RIGHT_CENTER
};

class Rect : public Triangles
{
	static LocMode locationMode;

	GLfloat width, height, rotation;

public:
	Rect(GLfloat sL, glm::vec2 loc, GLfloat rot = 0) :
		Rect(sL, sL, loc[0], loc[1], rot) {}
	Rect(GLfloat sL, GLfloat locx, GLfloat locy, GLfloat rot = 0) :
		Rect(sL, sL, locx, locy, rot) {}
	Rect(GLfloat w, GLfloat h, glm::vec2 loc, GLfloat rot = 0) :
		Rect(w, h, loc[0], loc[1], rot) {}
	Rect(GLfloat w, GLfloat h, GLfloat locx, GLfloat locy, GLfloat rot = 0);

	static void setLocationMode(LocMode mode);
	~Rect();
};

