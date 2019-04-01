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
		startMovingLoop0 = false,
		startMovingLoop1 = false;
	int movingLoopCntr = 0;

	std::vector<glm::vec2> p0;
	std::vector<glm::vec2> p1;
	std::vector<glm::vec2> vecs;
	std::vector<float> dists;
	float max = 0.5;
	unsigned int loopLength = 100;

	Curve * moving;
	Curve * standing;
	Curve * finalCurve;
	Curve * startingCurve;

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

		if (window->isPressed(GLFW_KEY_SPACE) && !startMovingLoop0 && !startMovingLoop1 && curves.size() == 2)
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
			}// TODO

			p0 = curves[0]->getPoints();
			p1 = curves[1]->getPoints();
			

			if (p0.size() > p1.size())
			{
				float coef = (float)p1.size() / p0.size();
				for (int i = 0; i < p0.size(); i++)
				{
					if ((int)(i*coef) < p1.size())
					{
						vecs.push_back(p0[i] - p1[(int)(i*coef)]);
						dists.push_back(glm::length(vecs[i]));
					}
					else
					{
						vecs.push_back(p0[i] - p1[p1.size() - 1]);
						dists.push_back(glm::length(vecs[i]));
					}
				}
				delete curves[1];
				curves.pop_back();
				startMovingLoop0 = true;
			}
			else
			{
				float coef = (float)p0.size() / p1.size();
				for (int i = 0; i < p1.size(); i++)
				{
					if ((int)(i*coef) < p0.size())
					{
						vecs.push_back(p0[(int)i*coef] - p1[i]);
					}
					else
					{
						vecs.push_back(p0[p0.size() - 1] - p1[i]);
					}
				}
				coef = 1 / coef;
				for (int i = 0; i < p0.size(); i++)
				{
					if((int)i*coef < vecs.size())
						dists.push_back(glm::length(vecs[(int)i*coef]));
					else
						dists.push_back(glm::length(vecs[vecs.size() - 1]));
				}
				startMovingLoop1 = true;
				curves[0]->hide();
			}

			for (int i = 0; i < dists.size(); i++)
				if (dists[i] > max)
				{
					startMovingLoop0 = false;
					startMovingLoop1 = false;
					delete curves[1];
					curves.pop_back();
					curves[0]->show();
					p0.clear();
					p1.clear();
					vecs.clear();
					dists.clear();
					break;
				}
		}

		if (startMovingLoop0 && movingLoopCntr < loopLength)
		{
			float coef = (float)movingLoopCntr++ / loopLength;

			for (int j = 0; j < p0.size(); j++)
			{
				curves[0]->setPoint(j, p0[j] - (1 - coef)*vecs[j]);

				if (movingLoopCntr == loopLength)
					curves[0]->setPointColor(j, glm::vec3(pow(dists[j] / max, 3), 0.0f, 0.0f));
			}

			if (movingLoopCntr == loopLength)
			{
				p0.clear();
				p1.clear();
				vecs.clear();
				dists.clear();
				movingLoopCntr = 0;
				startMovingLoop0 = false;
			}
		}

		if (startMovingLoop1 && movingLoopCntr < loopLength)
		{
			float coef = (float)movingLoopCntr++ / loopLength;

			for (int j = 0; j < p1.size(); j++)
			{
				curves[1]->setPoint(j, p1[j] + coef*vecs[j]);
			}

			if (movingLoopCntr == loopLength)
			{
				for (int i = 0; i < p0.size(); i++)
				{
					curves[0]->setPointColor(i, glm::vec3(pow(dists[i] / max, 3), 0.0f, 0.0f));
				}
				delete curves[1];
				curves.pop_back();
				p0.clear();
				p1.clear();
				vecs.clear();
				dists.clear();
				movingLoopCntr = 0;
				startMovingLoop1 = false;
				curves[0]->show();
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