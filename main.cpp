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

	Rect s(0.5f, glm::vec2(-0.25f, 0.25f));
	Rect s0(0.5f, 0.1f, glm::vec2(-0.25f, 0.75f));
	s0.rotate(-(float)PI / 2);
	
	Circ c(0.01f, glm::vec2(-0.25f, 0.75f));
	Circ c0(0.1f, glm::vec2(0.0f, 0.0f));
	std::vector<Curve*> curves;
	curves.push_back(new Curve(0.1f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	curves[0]->addPoint(glm::vec2(0.5f, 0.0f));
	curves[0]->addPoint(glm::vec2(0.0f, 0.5f));
	curves[0]->addPoint(glm::vec2(-0.5f, 0.0f));
	curves[0]->addPoint(glm::vec2(0.0f, -0.5f));
	curves[0]->addPoint(glm::vec2(0.5f, 0.0f));

	float rot = 0.0;

	bool lastMouse = false;

	do {
		window->clear();

		rot += 0.01f;

		c.draw();
		//c0.draw();
		s0.rotate(rot);
		if (window->isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (!lastMouse)
			{
				lastMouse = true;

				curves.push_back(new Curve(0.05f));
			}

			double xpos, ypos;
			glfwGetCursorPos(window->getGLFWWindowPtr(), &xpos, &ypos);
			xpos = ((2 * xpos / window->width()) - 1)*window->width()/window->height();
			ypos = (2 * (window->height() - ypos) / window->height()) - 1;
			curves[curves.size() - 1]->addPoint(glm::vec2(xpos, ypos));
		}
		else
			lastMouse = false;

		for(Curve * c : curves)
			c->draw();

		//s.draw();
		s0.draw();
		
		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));


	window->close();
	glDisable(GL_DEBUG_OUTPUT);
	return 0;
}