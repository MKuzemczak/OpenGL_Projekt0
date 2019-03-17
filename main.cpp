#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "shader.hpp"
#include "texture.hpp"

Window * window;

using namespace glm;

int main()
{
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
	window->setBgcolor(0, 0, 0.4, 0);

	do {
		window->clear();

		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));

	window->close();

	return 0;
}