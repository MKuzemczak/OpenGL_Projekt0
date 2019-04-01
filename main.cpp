#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Triangles.h"
#include "Rect.h"
#include "Circ.h"
#include "Curve.h"
#include "shader.hpp"
#include "texture.hpp"

Window * window;

using namespace glm;



int main()
{
	glEnable(GL_DEBUG_OUTPUT);
	window = new Window();
	window->init(800, 600, "Okno");

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	window->enableKeyboard();
	window->setBgcolor(0, 0, 0.4f, 0);

	Rect::setLocationMode(LEFT_CENTER);
	DrawableObject::setAspectRatio(window->width(), window->height());

	//Circ c(0.1f, 0.5f, 0.5f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	std::vector<Curve*> curves;

	bool lastMouse = false,
		startMovingLoop = false;
	int movingLoopCntr = 0;

	std::vector<glm::vec2> vecs;
	std::vector<float> dists;
	float max = 0.5;
	unsigned int loopLength = 100;

	Curve * moving = NULL;
	Curve * standing = NULL;
	Curve * finalCurve = NULL;
	Curve * startingCurve = NULL;
	std::vector<glm::vec2> movingPoints,
		standingPoints,
		finalCurvePoints,
		startingCurvePoints;

	do {
		window->clear();

		if (window->isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (!lastMouse)
			{
				lastMouse = true;

				curves.push_back(new Curve(0.05f, glm::vec4(0.5f, 0.7f, 0.3f, 1.0f)));
			}

			curves[curves.size() - 1]->addPoint(window->getCursorPos());
		}
		else
			lastMouse = false;

		//c.draw();

		if (window->isPressed(GLFW_KEY_SPACE) && !startMovingLoop && curves.size() == 2)
		{
			finalCurve = curves[0];
			startingCurve = curves[1];

			if (finalCurve->getPoints().size() > startingCurve->getPoints().size())
			{
				moving = finalCurve;
				standing = startingCurve;
			}
			else
			{
				standing = finalCurve;
				moving = startingCurve;
			}

			movingPoints = moving->getPoints();
			standingPoints = standing->getPoints();
			finalCurvePoints = finalCurve->getPoints();
			startingCurvePoints = startingCurve->getPoints();
			
			float coef = (float)standingPoints.size() / movingPoints.size();
			for (int i = 0; i < moving->getPoints().size(); i++)
			{
					vecs.push_back(finalCurvePoints[i*finalCurvePoints.size() / movingPoints.size()] - 
						startingCurvePoints[i*startingCurvePoints.size() / movingPoints.size()]);
					moving->setPoint(i, startingCurvePoints[i*startingCurvePoints.size() / movingPoints.size()]);
				
			}
			for (int i = 0; i < finalCurvePoints.size(); i++)
			{
				dists.push_back(glm::length(vecs[i*movingPoints.size() / finalCurvePoints.size()]));
			}

			standing->hide();
			moving->show();
			movingPoints = moving->getPoints();
			startMovingLoop = true;

			for (int i = 0; i < dists.size(); i++)
				if (dists[i] > max)
				{
					startMovingLoop = false;
					delete curves[1];
					curves.pop_back();
					curves[0]->show();
					vecs.clear();
					dists.clear();
					break;
				}
		}

		if (startMovingLoop && movingLoopCntr < loopLength)
		{
			float coef = (float)movingLoopCntr++ / loopLength;

			for (int j = 0; j < movingPoints.size(); j++)
			{
				moving->setPoint(j, movingPoints[j] + coef * vecs[j]);
			}

			if (movingLoopCntr == loopLength)
			{
				for (int i = 0; i < finalCurvePoints.size(); i++)
					finalCurve->setPointColor(i, glm::vec3(pow(dists[i] / max, 3), 0.0f, 0.0f));
				vecs.clear();
				dists.clear();
				finalCurve->show();
				delete curves[1];
				curves.pop_back();
				movingLoopCntr = 0;
				startMovingLoop = false;
			}
		}

		for(Curve * c : curves)
			c->draw();

		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));


	window->close();
	glDisable(GL_DEBUG_OUTPUT);
	return 0;
}