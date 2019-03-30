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

	bool lastMouse = false;

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

		if (window->isPressed(GLFW_KEY_SPACE))
		{
			curves[0]->setPoint(curves[0]->pointsSize() / 2, glm::vec2(0.5f, 0.5f));
			curves[0]->setPointColor(curves[0]->pointsSize() / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		}

		for(Curve * c : curves)
			c->draw();

		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));


	window->close();
	glDisable(GL_DEBUG_OUTPUT);
	return 0;
}