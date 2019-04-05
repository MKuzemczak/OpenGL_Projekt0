#include <iostream>
#include <map>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Triangles.h"
#include "Rect.h"
#include "Circ.h"
#include "Curve.h"
#include "shader.hpp"
#include "texture.hpp"
#include "Text.h"

Window * window;

using namespace glm;

typedef std::chrono::high_resolution_clock Clock;

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
		startMovingLoop = false,
		doCalculations = false;

	std::vector<glm::vec2> vecs;
	std::vector<float> dists;
	float max = window->height()/2;
	unsigned int loopLength = 100,
		movingLoopCntr = 0;

	Curve * moving = NULL;
	Curve * standing = NULL;
	Curve * finalCurve = NULL;
	Curve * startingCurve = NULL;
	std::vector<glm::vec2> movingPoints,
		standingPoints,
		finalCurvePoints,
		startingCurvePoints;

	Text text("hehehello", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	text.setLocation(glm::vec2(10.0f, 542.0f));
	text.generateShaders();
	text.loadFont("fonts/arial.ttf", 48);

	auto prevTime = Clock::now();
	auto time = prevTime;

	do {
		window->clear();

		if (window->isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (!lastMouse)
			{
				lastMouse = true;

				curves.push_back(new Curve(10.0f, glm::vec4(0.5f, 0.7f, 0.3f, 1.0f)));
			}

			curves[curves.size() - 1]->addPoint(window->getCursorPos());
		}
		else if (lastMouse)
		{
			lastMouse = false;
			if (curves.size() == 2)
				doCalculations = true;
		}

		//c.draw();

		if (doCalculations && !startMovingLoop && curves.size() == 2)
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
			
			for (unsigned int i = 0; i < moving->getPoints().size(); i++)
			{
					vecs.push_back(finalCurvePoints[i*finalCurvePoints.size() / movingPoints.size()] - 
						startingCurvePoints[i*startingCurvePoints.size() / movingPoints.size()]);
			}
			for (unsigned int i = 0; i < finalCurvePoints.size(); i++)
			{
				dists.push_back(glm::length(vecs[i*movingPoints.size() / finalCurvePoints.size()]));
			}

			startMovingLoop = true;

			for (unsigned int i = 0; i < dists.size(); i++)
				if (dists[i] > max)
				{
					startMovingLoop = false;
					delete curves[1];
					curves.pop_back();
					curves[0]->show();
					vecs.clear();
					dists.clear();
					moving = NULL;
					standing = NULL;
					finalCurve = NULL;
					startingCurve = NULL;
					break;
				}

			if (startMovingLoop)
			{
				for (unsigned int i = 0; i < moving->getPoints().size(); i++)
					moving->setPoint(i, startingCurvePoints[i*startingCurvePoints.size() / movingPoints.size()]);
				movingPoints = moving->getPoints();
				standing->hide();
				moving->show();
			}
			doCalculations = false;
		}

		if (startMovingLoop && movingLoopCntr < loopLength)
		{
			float coef = (float)movingLoopCntr++ / loopLength;

			for (unsigned int j = 0; j < movingPoints.size(); j++)
			{
				moving->setPoint(j, movingPoints[j] + coef * vecs[j]);
			}

			if (movingLoopCntr == loopLength)
			{
				for (unsigned int i = 0; i < finalCurvePoints.size(); i++)
					finalCurve->setPointColor(i, glm::vec3(pow(dists[i] / max, 2), 0.0f, 0.0f));
				vecs.clear();
				dists.clear();
				finalCurve->show();
				delete curves[1];
				curves.pop_back();
				movingLoopCntr = 0;
				startMovingLoop = false;
				moving = NULL;
				standing = NULL;
				finalCurve = NULL;
				startingCurve = NULL;
			}
		}

		for(Curve * c : curves)
			c->draw();

		time = Clock::now();
		double interval = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(time - prevTime).count());
		int fps = 1 / (interval / 1000000);
		prevTime = time;
		text.setText(std::to_string(fps));
		text.draw();

		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));


	window->close();
	glDisable(GL_DEBUG_OUTPUT);
	return 0;
}